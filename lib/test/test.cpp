#include <iostream>
#include <CImg.h>
#include <lime/Segmentation.hpp>
#include <lime/ColorimetricHSIAlgorithm1.hpp>
#include <lime/ColorimetricYCbCrAlgorithm1.hpp>
#include <lime/ColorimetricHSVAlgorithm1.hpp>

using namespace lime;

typedef unsigned char NumType;

int main(int argc, char** argv)
{
	const std::string sourcePath = "IMG_4021.jpg";
	const std::string destPath = "test.bmp";

	// Basic procedure to use the library
	ColorimetricHSVAlgorithm1<NumType> algo = ColorimetricHSVAlgorithm1<NumType>(0.0f);
    Segmentation<NumType> segm = Segmentation<NumType>(&algo);
	CImg<NumType> *testImg = segm.retrieveMask_asAlphaChannel(sourcePath);

	//changeBinaryMaskToRGBChannel(testImg);

	testImg->save(destPath.c_str());

	delete testImg;

    return 0;
}

