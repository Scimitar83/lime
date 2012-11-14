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
/// \file    Segmentation.hpp
/// \class   Segmentation
///
/// \package lime
/// \version 0.2.0
///
/// \brief   base class for image segmentation
///
/// \details this is the base class of segmentations using the lime library
///
/// \author  Alexandru Duliu, Alexander Schoch
/// \date    Oct 29, 2012 - first creation
/// \data    Nov 13, 2012 - basic structure
///


#include <lime/util.hpp>
#include <lime/Algorithm.hpp>
#include <CImg.h>
#include <string>

using namespace cimg_library;

namespace lime
{

template<typename T = int> class Segmentation
{
public:
	Segmentation(Algorithm<T>* _algorithm):algorithm(_algorithm){}
	  virtual ~Segmentation(){}
	
	//Inline functions

	/**
	* @brief Can be used to switch the algorithm at runtime (Strategy Pattern)
	*/
	inline void switchAlgorithm(Algorithm<T>* _algorithm){algorithm = _algorithm;}

	/**
	* @brief Returns the current recall tendency (how much the Segmentation is aimed at recall vs. precision (might have no effect, see hasRecallTendency)
	*/
	inline float getRecallTendency(){return algorithm->getDrift();}

	/** 
	* @brief Returns true if the Segmentation currently supports the recall tendency mechanism (dependent on the used Algorithm)
	*/
	inline bool hasRecallTendency(){return algorithm->hasDrift();}

	/** 
	* @brief Used to change the recall tendency of the Segmentation (might have no effect, see hasRecallTendency)
	*/
	inline void changeRecallTendency(float tendency){algorithm->changeDrift(tendency);}

	/** 
	* @brief Delivers a binary mask (1 == skin pixel, 0 == no-skin pixel) with the width and height of the original image
	*/
	inline void retrieveMaskOfImage(CImg<T> *img){algorithm->processImage(img);}

	/** 
	* @brief Delivers a binary mask (1 == skin pixel, 0 == no-skin pixel) with the width and height of the original image
	*/
	inline CImg<T>* retrieveMaskOfImage(const CImg<T> &img){return algorithm->processImage(img);}

	/** 
	* @brief Delivers a binary mask (1 == skin pixel, 0 == no-skin pixel) with the width and height of the original image
	*/
	inline CImg<T>* retrieveMaskOfImage(const std::string filename)
	{

		CImg<T> tempImg;

		loadImage(filename,tempImg);

		return algorithm->processImage(tempImg);
	}

protected:
	/** 
	* @brief The internal algorithm that is used to process an image and generate a bit mask
	*/
	Algorithm<T>* algorithm;


};

} // end namespace lime
