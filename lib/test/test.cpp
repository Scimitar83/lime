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
	algo.ApplyMedian(true);
	algo.MedianSize(3);
	algo.ApplyGrow(true);
	algo.GrowCount(20);
	algo.GrowSize(3);
	algo.ApplyShrink(true);
	algo.ShrinkCount(22);
	algo.ShrinkSize(3);
	algo.ApplyFixedGrowShrink(false);
	algo.FixedGrowShrinkCount(10);
	algo.FixedGrowShrinkSize(5);
	algo.ApplyRegionClearing(true);

    Segmentation<NumType> segm = Segmentation<NumType>(&algo);
	CImg<bool> *mask = segm.retrieveMask_asBinaryChannel(sourcePath);

	CImg<int> *distMap = segm.retrieveDistanceMapOfMask(*mask);

	CImg<unsigned char> *resImg = distanceMapToRGB(distMap);

	//CImg<int> *resImg = changeBinaryMaskToRGBImage(*mask);

	/*std::vector<BinarySeed> *skinSeeds = segm.retrieveSkinSeedsOfMask(*testImg, true, true, 30, 5);
	std::vector<BinarySeed> *nonSkinSeeds = segm.retrieveNonSkinSeedsOfMask(*testImg, true, true, 30, 5);

	addSeedsToRGBImage(resImg,skinSeeds,nonSkinSeeds);*/

	resImg->save(destPath.c_str());

	delete mask;
	delete resImg;
	delete distMap;

    return 0;
}

