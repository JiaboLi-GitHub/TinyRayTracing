#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"
#include <chrono>
#include "global.cuh"
#include "camera.cuh"
#include "hittable.cuh"
#include "material.cuh"

void initGlobal();

void flipImage(int width, int height, std::vector<unsigned char>& image_data);

__global__ void initRand();

__global__ void initCamera(Camera** camera, vec3* image);

__global__ void initScene(Hittable** scene);

__global__ void render(Camera** camera, Hittable** scene);

int main()
{
    // 获取开始时间
    auto start = std::chrono::high_resolution_clock::now();

    // 初始化全局数据
    initGlobal();

    // 初始化摄像机
    Camera** cameraD;
    vec3* imageD;

    cudaMalloc((void**)&cameraD, sizeof(Camera*));
    cudaMalloc((void**)&imageD, sizeof(vec3) * ImageSize);

    initCamera<<<1,1>>>(cameraD, imageD);
    cudaDeviceSynchronize();

    // 初始化场景
    Hittable** sceneD;
    cudaMalloc((void**)&sceneD, sizeof(Hittable*));
    initScene<<<1,1>>>(sceneD);

    // 渲染
    render<<<GridSize,BlockSize>>>(cameraD, sceneD);
    cudaDeviceSynchronize();
    auto err = cudaGetLastError();
    if (err != cudaSuccess)
    {
        std::cerr << "Kernel launch failed: " << cudaGetErrorString(err) << std::endl;
        return -1;
    }

    // 将图片从设备内存拷贝到主机内存
    std::vector<vec3> imageH(ImageSize);
    cudaMemcpy(imageH.data(), imageD, ImageSize * sizeof(vec3), cudaMemcpyDeviceToHost);

    // 保存图片
    std::vector<unsigned char> stbImage;
    stbImage.reserve(ImageWidth * ImageHeight * 3);
    for (const auto& v : imageH)
    {
        // std::cout << v << std::endl;
        stbImage.push_back(static_cast<unsigned char>(v.r() * 255));
        stbImage.push_back(static_cast<unsigned char>(v.g() * 255));
        stbImage.push_back(static_cast<unsigned char>(v.b() * 255));
    }
    flipImage(ImageWidth, ImageHeight, stbImage);
    if (stbi_write_png("output.png", ImageWidth, ImageHeight, 3, stbImage.data(), ImageWidth * 3))
    {
        std::cout << "图片保存成功" << std::endl;
    }
    else
    {
        std::cerr << "图片保存失败" << std::endl;
    }

    // 获取结束时间
    auto end = std::chrono::high_resolution_clock::now();

    // 计算持续时间
    std::chrono::duration<double, std::milli> duration = end - start;

    std::cout << "光线追踪耗时: " << duration.count() << " ms" << std::endl;

    return 0;
}

void initGlobal()
{
    cudaMemcpyToSymbol(AspectRatioD, &AspectRatio, sizeof(double));
    cudaMemcpyToSymbol(ImageWidthD, &ImageWidth, sizeof(int));
    cudaMemcpyToSymbol(ImageHeightD, &ImageHeight, sizeof(int));
    cudaMemcpyToSymbol(ImageSizeD, &ImageSize, sizeof(int));

    curandState* randState;
    cudaMalloc((void**)&randState, ImageSize * sizeof(curandState));
    cudaMemcpyToSymbol(RandStateD, &randState, sizeof(curandState*));

    initRand<<<GridSize,BlockSize>>>();
    cudaDeviceSynchronize();
}

void flipImage(int width, int height, std::vector<unsigned char>& image_data)
{
    int bytes_per_pixel = 3;
    for (int y = 0; y < height / 2; ++y) {
        for (int x = 0; x < width; ++x) {
            int idx1 = (y * width + x) * bytes_per_pixel;
            int idx2 = ((height - 1 - y) * width + x) * bytes_per_pixel;

            // 交换行
            for (int b = 0; b < bytes_per_pixel; ++b) {
                std::swap(image_data[idx1 + b], image_data[idx2 + b]);
            }
        }
    }
}

__global__ void initRand()
{
    int x = threadIdx.x + blockIdx.x * blockDim.x;
    int y = threadIdx.y + blockIdx.y * blockDim.y;
    if (x >= ImageWidthD || y >= ImageHeightD)
        return;

    auto index = x + y * ImageWidthD;
    curand_init(1984 + index, 0, 0, &RandStateD[index]);
}

__global__ void initCamera(Camera** camera, vec3* image)
{
    *camera = new Camera(image);

    printf("%lf\n", AspectRatioD);
    printf("%d\n", ImageWidthD);
    printf("%d\n", ImageHeightD);
    printf("%d\n", ImageSizeD);
}

__global__ void initScene(Hittable** scene)
{
    Hittable** hittables = new Hittable*[1 + 22 * 22 + 3];
    int index = 0;

    auto ground_material = new Lambertian(vec3(0.5, 0.5, 0.5));
    hittables[index++] = new Sphere(vec3(0, -1000, 0), 1000, ground_material);

    for (int a = -11; a < 11; a++)
    {
        for (int b = -11; b < 11; b++)
        {
            auto choose_mat = randomDouble();
            vec3 center(a + 0.9 * randomDouble(), 0.2, b + 0.9 * randomDouble());

            if ((center - vec3(4, 0.2, 0)).length() > 0.9)
            {
                Material* sphere_material;

                if (choose_mat < 0.8)
                {
                    auto albedo = randomVec3(0.5, 1);
                    sphere_material = new Lambertian(albedo);
                    hittables[index++] = new Sphere(center, 0.2, sphere_material);
                }
                else if (choose_mat < 0.95)
                {
                    auto albedo = randomVec3(0.5, 1);
                    auto fuzz = randomDouble(0, 0.5);
                    sphere_material = new Metal(albedo, fuzz);
                    hittables[index++] = new Sphere(center, 0.2, sphere_material);
                }
                else
                {
                    sphere_material = new Dielectric(1.5);
                    hittables[index++] = new Sphere(center, 0.2, sphere_material);
                }
            }
        }
    }

    auto material1 = new Dielectric(1.5);
    hittables[index++] = new Sphere(vec3(0.0, 1.0, 0.0), 1.0, material1);
    auto material1_1 = new Dielectric(1.00 / 1.50);
    hittables[index++] = new Sphere(vec3(0.0, 1.0, 0.0), 0.8, material1_1);

    auto material2 = new Lambertian(vec3(0.4, 0.2, 0.1));
    hittables[index++] =  new Sphere(vec3(-4.0, 1.0, 0.0), 1.0, material2);

    auto material3 = new Metal(vec3(0.7, 0.6, 0.5), 0.0);
    hittables[index++] =  new Sphere(vec3(4.0, 1.0, 0.0), 1.0, material3);

    *scene = new HittableList(hittables, index);

    printf("initScene OK\n");
}

__global__ void render(Camera** camera, Hittable** scene)
{
    int x = threadIdx.x + blockIdx.x * blockDim.x;
    int y = threadIdx.y + blockIdx.y * blockDim.y;
    if (x >= ImageWidthD || y >= ImageHeightD)
        return;

    (*camera)->render(*scene, x, y);
}

