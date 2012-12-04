////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of lime, a lightweight C++ segmentation library          //
//                                                                            //
// Copyright (C) 2012 Alexandru Duliu                                         //
//                                                                            //
// lime is free software; you can redistribute it and/or                      //
// modify it under the terms of the GNU Lesser General Public                 //
// License as published by the Free Software Foundation; either               //
// version 3 of the License, or (at your option) any later version.           //
//                                                                            //
// lime is distributed in the hope that it will be useful, but WITHOUT ANY    //
// WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS  //
// FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License or the //
// GNU General Public License for more details.                               //
//                                                                            //
// You should have received a copy of the GNU Lesser General Public           //
// License along with lime. If not, see <http://www.gnu.org/licenses/>.       //
//                                                                            //
///////////////////////////////////////////////////////////////////////////////


#pragma once

///
/// @file Segmentation.hpp
/// @brief Contains the Segmentation class
/// @author Alexandru Duliu, Alexander Schoch
/// @date Nov 13, 2012 - First creation
/// @package lime
///


#include <lime/util.hpp>
#include <lime/Algorithm.hpp>
#include <CImg.h>
#include <string>

using namespace cimg_library;

namespace lime
{

///
/// @class Segmentation
///
/// @version 0.3.0
///
/// @brief Base class for image segmentation. Needs an algorithm to work.
///
/// @details this is the base class of segmentations using the lime library
///
/// @author Alexandru Duliu, Alexander Schoch
/// @date Oct 29, 2012 - First creation
/// @date Nov 13, 2012 - Basic structure
/// @date Nov 23, 2012 - Some small adjustments including the possibility to retrieve a mask as alpha channel
/// @tparam T - Can be of any basic data type and should be the same as the one of the input image (e.g. double or char).
///
template<typename T = int> class Segmentation
{

public:

	///
	/// @brief The constructor of Segmentation that needs a skin segmentation algorithm passed to (Strategy pattern)
	///
	Segmentation(Algorithm<T>* _algorithm):algorithm(_algorithm){}
	
	///
	/// @brief The basic destructor
	///
	virtual ~Segmentation(){}
	
	//Inline functions

	///
	/// @brief Can be used to switch the algorithm at runtime (Strategy Pattern)
	/// @param algorithm The new algorithm that should be used to process the image data for skin segmentation
	///
	inline void switchAlgorithm(Algorithm<T>* _algorithm){algorithm = _algorithm;}

	///
	/// @brief Processes the image and then adds the skin segmentation as an alpha channel (255 == skin, 0 == no-skin-pixel) to the original image.
	/// @param img The image data that should get the binary mask added as an alpha channel to it
	///
	inline void retrieveMask_asAlphaChannel(CImg<T> *img)
	{
		CImg<bool> *mask = algorithm->processImage(*img);

		fuseBinaryMaskWithRGBImage(img,mask);

		delete mask;
	}

	///
	/// @brief Processes the image and the delivers a binary mask (1 == skin pixel, 0 == no-skin pixel) with the width and height of the original image.
	/// @param img The image data that should be processed
	/// @return The new bit mask
	///
	inline CImg<bool>* retrieveMask_asBinaryChannel(const CImg<T> &img){return algorithm->processImage(img);}

	///
	/// @brief Processes the image and then adds the skin segmentation as an alpha channel (255 == skin, 0 == no-skin-pixel) to the original image.
	/// @param img The image data that should be processed
	/// @return The new image with 4 channels
	///
	inline CImg<T>* retrieveMask_asAlphaChannel(const CImg<T> &img)
	{
		CImg<bool> *mask = algorithm->processImage(img);

		CImg<T> *resImg = new CImg<T>(img);

		fuseBinaryMaskWithRGBImage(resImg,mask);

		delete mask;

		return resImg;
	}

	///
	/// @brief Processes the image and the delivers a binary mask (1 == skin pixel, 0 == no-skin pixel) with the width and height of the original image.
	/// @param filename The full path to the image
	///
	inline CImg<bool>* retrieveMask_asBinaryChannel(const std::string filename)
	{
		CImg<T> tempImg;

		loadImage(filename,tempImg);

		return algorithm->processImage(tempImg);
	}

	///
	/// @brief Processes the image and then adds the skin segmentation as an alpha channel (255 == skin, 0 == no-skin-pixel) to the original image.
	/// @param filename The full path to the image
	///
	inline CImg<T>* retrieveMask_asAlphaChannel(const std::string filename)
	{
		CImg<T> *resImg;

		loadImage(filename,*resImg);
	    
		CImg<bool> *mask = algorithm->processImage(*resImg);

		fuseBinaryMaskWithRGBImage(resImg,mask);

		delete mask;

		return resImg;
	}

	///
	/// @brief Uses a bit mask to automatically determine seed points for skin pixels
	/// @mask The bit mask
	/// @singleRegion Only determines seed pixels on the border of the first region that is being detected
	/// @applyErosion True if a shrink algorithm should be used prior to the seed pixel detection
	/// @erosionCount Number of times the shrink algorithm should be used
	/// @erosionSize Size of the kernel for the shrink algorithm
	///
	inline std::vector<BinarySeed>* retrieveSkinSeedsOfMask(const CImg<bool> &mask, bool singleRegion = false, bool applyErosion = true, unsigned int erosionCount = 1, unsigned int erosionSize = 3)
	{
		return algorithm->getSeeds(true,singleRegion,mask,applyErosion,erosionCount,erosionSize);
	}

	///
	/// @brief Uses a bit mask to automatically determine seed points for non-skin pixels
	/// @mask The bit mask
	/// @singleRegion Only determines seed pixels on the border of the first region that is being detected
	/// @applyDilation True if a grow algorithm should be used prior to the seed pixel detection
	/// @dilationCount Number of times the grow algorithm should be used
	/// @dilationSize Size of the kernel for the grow algorithm
	///
	inline std::vector<BinarySeed>* retrieveNonSkinSeedsOfMask(const CImg<bool> &mask, bool singleRegion = false, bool appyDilation = true, unsigned int dilationCount = 1, unsigned int dilationSize = 3)
	{
		return algorithm->getSeeds(false,singleRegion,mask,appyDilation,dilationCount,dilationSize);
	}

protected:

	///
	/// @brief The internal algorithm that is used to process an image and generate a bit mask
	///
	Algorithm<T>* algorithm;


};

} // end namespace lime
