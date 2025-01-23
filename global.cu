#include "global.cuh"

double AspectRatio = 16.0 / 9.0;
int ImageWidth = 800;
int ImageHeight = ImageWidth / AspectRatio;
int ImageSize = ImageWidth * ImageHeight;
const dim3 BlockSize = dim3(16, 16);
const dim3 GridSize = dim3(ImageWidth / BlockSize.x + 1, ImageHeight / BlockSize.y + 1);

__device__ double AspectRatioD;
__device__ int ImageWidthD;
__device__ int ImageHeightD;
__device__ int ImageSizeD;
__device__ curandState* RandStateD = nullptr;
