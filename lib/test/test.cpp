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
	const std::string sourcePath = "IMG_0267.jpg";
	const std::string destPath = "test.bmp";

	// Basic procedure to use the library
	ColorimetricYCbCrAlgorithm1<NumType> algo = ColorimetricYCbCrAlgorithm1<NumType>();

	// Algorithm configuration
	algo.ApplyMedian(false);
	algo.MedianSize(10);
	algo.ApplyGrow(false);
	algo.GrowCount(100);
	algo.GrowSize(2);
	algo.ApplyShrink(false);
	algo.ShrinkCount(100);
	algo.ShrinkSize(2);

    Segmentation<NumType> segm = Segmentation<NumType>(&algo);
	CImg<bool> *testImg = segm.retrieveMask_asBinaryChannel(sourcePath);

	CImg<char> *resImg = changeBinaryMaskToRGBImage(*testImg);

	resImg->save(destPath.c_str());

	delete testImg;
	delete resImg;

    return 0;
}

