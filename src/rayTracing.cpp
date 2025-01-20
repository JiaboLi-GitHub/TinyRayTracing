#include "rayTracing.h"
#include <qdebug.h>
#include "sphere.h"
#include "bvh.h"
#include "quad.h"

RayTracing::RayTracing(QObject *parent)
	: QObject(parent)
{
	initScene();
	initCamera();
}

RayTracing::~RayTracing()
{}

void RayTracing::slotRender()
{
    emit sigRenderStart();

    m_camera->render(m_scene);

    emit sigRenderEnd();
}

void RayTracing::initScene()
{
    switch (CurrentSceneType)
    {
    case SceneType::OneWeekend:
    {
        initOneWeekendScene();
        break;
    }
    case SceneType::NextWeek:
    {
        initNextWeekScene();
        break;
    }
    case SceneType::CornellBox:
    {
        initCornellBoxScene();
        break;
    }
    default:
        break;
    }
}

void RayTracing::initCamera()
{
	m_camera = new Camera(this);
    m_camera->init(m_cameraConfig);
	connect(m_camera, &Camera::sigRender, this, &RayTracing::sigRender);
}

void RayTracing::initOneWeekendScene()
{
    m_scene = std::make_shared<HittableList>();

    auto ground_material = std::make_shared<Lambertian>(glm::dvec3(0.5, 0.5, 0.5));
    m_scene->add(std::make_shared<Sphere>(glm::dvec3(0, -1000, 0), 1000, ground_material));

    for (int a = -11; a < 11; a++)
    {
        for (int b = -11; b < 11; b++)
        {
            auto choose_mat = randomDouble();
            glm::dvec3 center(a + 0.9 * randomDouble(), 0.2, b + 0.9 * randomDouble());

            if ((center - glm::dvec3(4, 0.2, 0)).length() > 0.9)
            {
                std::shared_ptr<Material> sphere_material;

                if (choose_mat < 0.8)
                {
                    auto albedo = randomVec3(0.5, 1);
                    sphere_material = std::make_shared<Lambertian>(albedo);
                    m_scene->add(std::make_shared<Sphere>(center, 0.2, sphere_material));
                }
                else if (choose_mat < 0.95)
                {
                    auto albedo = randomVec3(0.5, 1);
                    auto fuzz = randomDouble(0, 0.5);
                    sphere_material = std::make_shared<Metal>(albedo, fuzz);
                    m_scene->add(std::make_shared<Sphere>(center, 0.2, sphere_material));
                }
                else
                {
                    sphere_material = std::make_shared<Dielectric>(1.5);
                    m_scene->add(std::make_shared<Sphere>(center, 0.2, sphere_material));
                }
            }
        }
    }

    auto material1 = std::make_shared<Dielectric>(1.5);
    m_scene->add(std::make_shared<Sphere>(glm::dvec3(0.0, 1.0, 0.0), 1.0, material1));
    auto material1_1 = std::make_shared<Dielectric>(1.00 / 1.50);
    m_scene->add(std::make_shared<Sphere>(glm::dvec3(0.0, 1.0, 0.0), 0.8, material1_1));

    auto material2 = std::make_shared<Lambertian>(glm::dvec3(0.4, 0.2, 0.1));
    m_scene->add(std::make_shared<Sphere>(glm::dvec3(-4.0, 1.0, 0.0), 1.0, material2));

    auto material3 = std::make_shared<Metal>(glm::dvec3(0.7, 0.6, 0.5), 0.0);
    m_scene->add(std::make_shared<Sphere>(glm::dvec3(4.0, 1.0, 0.0), 1.0, material3));


    auto bvhRoot = std::make_shared<BvhNode>(m_scene);
    m_scene = std::make_shared<HittableList>(bvhRoot);

    AspectRatio = 16.0 / 9.0;
    CanvasWidth = 1080;
    m_cameraConfig.cameraCenter = glm::dvec3(13, 2, 3);
    m_cameraConfig.fov = 20;
    m_cameraConfig.look = glm::dvec3(0, 0, 0);
    m_cameraConfig.up = glm::dvec3(0, 1, 0);
    m_cameraConfig.samplesPerPixel = 100;
    m_cameraConfig.maxDepth = 10;
    m_cameraConfig.defocusAngle = 0.6;
    m_cameraConfig.focusDist = 10.0;
    m_cameraConfig.backgroundColor = glm::dvec3(0.86, 0.91, 1.00);
}

void RayTracing::initCornellBoxScene()
{
    m_scene = std::make_shared<HittableList>();

    auto red = std::make_shared<Lambertian>(glm::dvec3(.65, .05, .05));
    auto white = std::make_shared<Lambertian>(glm::dvec3(.73, .73, .73));
    auto green = std::make_shared<Lambertian>(glm::dvec3(.12, .45, .15));
    auto light = std::make_shared<DiffuseLightTexture>(glm::dvec3(15, 15, 15));

    m_scene->add(std::make_shared<Quad>(glm::dvec3(555, 0, 0), glm::dvec3(0, 555, 0), glm::dvec3(0, 0, 555), green));
    m_scene->add(std::make_shared<Quad>(glm::dvec3(0, 0, 0), glm::dvec3(0, 555, 0), glm::dvec3(0, 0, 555), red));
    m_scene->add(std::make_shared<Quad>(glm::dvec3(343, 554, 332), glm::dvec3(-130, 0, 0), glm::dvec3(0, 0, -105), light));
    m_scene->add(std::make_shared<Quad>(glm::dvec3(0, 0, 0), glm::dvec3(555, 0, 0), glm::dvec3(0, 0, 555), white));
    m_scene->add(std::make_shared<Quad>(glm::dvec3(555, 555, 555), glm::dvec3(-555, 0, 0), glm::dvec3(0, 0, -555), white));
    m_scene->add(std::make_shared<Quad>(glm::dvec3(0, 0, 555), glm::dvec3(555, 0, 0), glm::dvec3(0, 555, 0), white));

    Hittable::Ptr box1 = Quad::box(glm::dvec3(0, 0, 0), glm::dvec3(165, 330, 165), white);
    box1 = std::make_shared<RotateY>(box1, 15);
    box1 = std::make_shared<Translate>(box1, glm::dvec3(265, 0, 295));
    m_scene->add(box1);

    Hittable::Ptr box2 = Quad::box(glm::dvec3(0, 0, 0), glm::dvec3(165, 165, 165), white);
    box2 = std::make_shared<RotateY>(box2, -18);
    box2 = std::make_shared<Translate>(box2, glm::dvec3(130, 0, 65));
    m_scene->add(box2);

    auto bvhRoot = std::make_shared<BvhNode>(m_scene);
    m_scene = std::make_shared<HittableList>(bvhRoot);

    AspectRatio = 1.0;
    CanvasWidth = 300;
    m_cameraConfig.samplesPerPixel = 100;
    m_cameraConfig.maxDepth = 5;
    m_cameraConfig.fov = 40;
    m_cameraConfig.cameraCenter = glm::dvec3(278, 278, -800);
    m_cameraConfig.look = glm::dvec3(278, 278, 0);
    m_cameraConfig.up = glm::vec3(0, 1, 0);
    m_cameraConfig.defocusAngle = 0;
    m_cameraConfig.focusDist = 10.0;
}

void RayTracing::initNextWeekScene()
{
    m_scene = std::make_shared<HittableList>();

    HittableList boxes1;
    auto ground = std::make_shared<Lambertian>(glm::dvec3(0.48, 0.83, 0.53));

    int boxes_per_side = 20;
    for (int i = 0; i < boxes_per_side; i++) {
        for (int j = 0; j < boxes_per_side; j++) {
            auto w = 100.0;
            auto x0 = -1000.0 + i * w;
            auto z0 = -1000.0 + j * w;
            auto y0 = 0.0;
            auto x1 = x0 + w;
            auto y1 = randomDouble(1, 101);
            auto z1 = z0 + w;

            boxes1.add(Quad::box(glm::dvec3(x0, y0, z0), glm::dvec3(x1, y1, z1), ground));
        }
    }


    m_scene->add(std::make_shared<BvhNode>(boxes1));

    auto light = std::make_shared<DiffuseLightTexture>(glm::dvec3(7, 7, 7));
    m_scene->add(std::make_shared<Quad>(glm::dvec3(123, 554, 147), glm::dvec3(300, 0, 0), glm::dvec3(0, 0, 265), light));

    auto center1 = glm::dvec3(400, 400, 200);
    auto center2 = center1 + glm::dvec3(30, 0, 0);
    auto sphere_material = std::make_shared<Lambertian>(glm::dvec3(0.7, 0.3, 0.1));
    m_scene->add(std::make_shared<Sphere>(center1, 50, glm::dvec3(1.0, 1.0, 1.0), sphere_material));

    m_scene->add(std::make_shared<Sphere>(glm::dvec3(260, 150, 45), 50, std::make_shared<Dielectric>(1.5)));
    m_scene->add(std::make_shared<Sphere>(
        glm::dvec3(0, 150, 145), 50, std::make_shared<Metal>(glm::dvec3(0.8, 0.8, 0.9), 1.0)
    ));

    auto boundary = std::make_shared<Sphere>(glm::dvec3(360, 150, 145), 70, std::make_shared<Dielectric>(1.5));
    m_scene->add(boundary);

    auto emat = std::make_shared <Lambertian>(std::make_shared<ImageTexture>("D:\\Users\\Desktop\\earthmap.jpg"));
    m_scene->add(std::make_shared<Sphere>(glm::dvec3(400, 200, 400), 100, emat));

    HittableList boxes2;
    auto white = std::make_shared<Lambertian>(glm::dvec3(.73, .73, .73));
    int ns = 1000;
    for (int j = 0; j < ns; j++) {
        boxes2.add(std::make_shared<Sphere>(randomVec3(0, 165), 10, white));
    }

    m_scene->add(std::make_shared<Translate>(
        std::make_shared<RotateY>(
            std::make_shared<BvhNode>(boxes2), 15),
        glm::dvec3(-100, 270, 395)
    )
    );

    AspectRatio = 1.0;
    CanvasWidth = 500;
    m_cameraConfig.samplesPerPixel = 5000;
    m_cameraConfig.maxDepth = 50;
    m_cameraConfig.fov = 40;
    m_cameraConfig.cameraCenter = glm::dvec3(478, 278, -600);
    m_cameraConfig.look = glm::dvec3(278, 278, 0);
    m_cameraConfig.up = glm::vec3(0, 1, 0);
    m_cameraConfig.defocusAngle = 0;
    m_cameraConfig.focusDist = 10.0;
}
