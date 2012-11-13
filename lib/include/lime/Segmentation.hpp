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
/// \version 0.1.0
///
/// \brief   base class for image segmentation
///
/// \details this is the base class of segmentations using the lime library
///
/// \author  Alexandru Duliu
/// \date    Oct 29, 2012
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
	inline void switchAlgorithm(Algorithm<T>* _algorithm){algorithm = _algorithm;}
	inline float getRecallTendency(){return algorithm->getDrift();}
	inline bool hasRecallTendency(){return algorithm->hasDrift();}
	inline void changeRecallTendency(float tendency){algorithm->changeDrift(tendency);}
	inline void retrieveMaskOfImage(CImg<T> *img){algorithm->processImage(img);}
	inline CImg<T>* retrieveMaskOfImage(const CImg<T> &img){return algorithm->processImage(img);}

	inline CImg<T>* retrieveMaskOfImage(const std::string filename)
	{

		CImg<T> tempImg;

		loadImage(filename,tempImg);

		return algorithm->processImage(tempImg);
	}

protected:
	Algorithm<T>* algorithm;


};

} // end namespace lime
