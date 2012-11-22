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
	ColorimetricHSVAlgorithm1<NumType> algo = ColorimetricHSVAlgorithm1<NumType>();

	// Algorithm configuration
	algo.ApplyMedian(true);
	algo.MedianSize(5);
	algo.ApplyGrow(true);
	algo.GrowCount(20);
	algo.GrowSize(3);
	algo.ApplyShrink(true);
	algo.ShrinkCount(25);
	algo.ShrinkSize(3);
	algo.ApplyFixedGrowShrink(false);
	algo.FixedGrowShrinkCount(10);
	algo.FixedGrowShrinkSize(5);
	algo.ApplyRegionClearing(true);

    Segmentation<NumType> segm = Segmentation<NumType>(&algo);
	CImg<bool> *testImg = segm.retrieveMask_asBinaryChannel(sourcePath);

	CImg<int> *resImg = changeBinaryMaskToRGBImage(*testImg);

	resImg->save(destPath.c_str());

	delete testImg;
	delete resImg;

    return 0;
}

