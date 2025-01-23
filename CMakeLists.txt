cmake_minimum_required(VERSION 3.15)
project(TinyRayTracing CUDA)

set(CMAKE_CUDA_STANDARD 17)

add_executable(TinyRayTracing main.cu
        ray.cu
        ray.cuh
        global.cu
        global.cuh
        camera.cu
        camera.cuh
        hittable.cu
        hittable.cuh
        interval.cu
        interval.cuh
        material.cu
        material.cuh
        vec3.cuh
)

set_target_properties(TinyRayTracing PROPERTIES
        CUDA_SEPARABLE_COMPILATION ON)

target_include_directories(TinyRayTracing PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})