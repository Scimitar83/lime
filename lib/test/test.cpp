#include <iostream>
#include <CImg.h>
#include <lime/Segmentation.hpp>
#include <lime/ColorimetricHSIAlgorithm1.hpp>
#include <lime/ColorimetricYCbCrAlgorithm1.hpp>
#include <lime/ColorimetricHSVAlgorithm1.hpp>

using namespace lime;

typedef int NumType;

void changeBinaryToRGB(cimg_library::CImg<NumType>*);

int main(int argc, char** argv)
{
	

	ColorimetricHSVAlgorithm1<NumType> algo = ColorimetricHSVAlgorithm1<NumType>(0.0f);
    Segmentation<NumType> segm = Segmentation<NumType>(&algo);

	const std::string sourcePath = "IMG_0267.jpg";
	const std::string destPath = "test.bmp";

	CImg<NumType> *testImg = segm.retrieveMaskOfImage(sourcePath);

	changeBinaryToRGB(testImg);

	testImg->save(destPath.c_str());

	delete testImg;

    return 0;
}

void changeBinaryToRGB(CImg<NumType> *img){

	int _width = img->width();
	int _height = img->height();

	CImg<int> tempCpy(*img);

	img->assign(_width,_height,1,3);

	for (int y = 0; y < _height; y++)
	{
		for (int x = 0; x < _width; x++)
		{
			if (tempCpy(x,y,0,0) == 0)
			{
				(*img)(x,y,0,0)=0;
				(*img)(x,y,0,1)=0;
				(*img)(x,y,0,2)=0;
			}
			else
			{
				(*img)(x,y,0,0)=255;
				(*img)(x,y,0,1)=255;
				(*img)(x,y,0,2)=255;
			}
		}
	}
}
