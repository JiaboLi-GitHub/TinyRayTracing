#include "rayTracing.h"
#include "sphere.h"

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
    m_scene = std::make_shared<HittableList>();

    auto ground_material = std::make_shared<Lambertian>(glm::dvec3(0.5, 0.5, 0.5));
    m_scene->add(std::make_shared<Sphere>(glm::dvec3(0, -1000, 0), 1000, ground_material));

    for (int a = -11; a < 11; a++) 
    {
        for (int b = -11; b < 11; b++) 
        {
            auto choose_mat = randomdouble();
            glm::dvec3 center(a + 0.9 * randomdouble(), 0.2, b + 0.9 * randomdouble());

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
                    auto fuzz = randomdouble(0, 0.5);
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
}

void RayTracing::initCamera()
{
	m_camera = new Camera(this);
	connect(m_camera, &Camera::sigRender, this, &RayTracing::sigRender);
}
