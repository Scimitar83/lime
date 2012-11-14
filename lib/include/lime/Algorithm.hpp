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
/// \file    Algorithm.hpp
/// \class   Algorithm
///
/// \package lime
/// \version 0.2.0
///
/// \brief Abstract base class for all Skin Segmentation algorithms
///
/// \detail Abstract base class for all Skin Segmentation algorithms
///
/// \author  Aleander Schoch
/// \date    Nov 13, 2012 - first creation and implementation
///

#ifndef _USE_MATH_DEFINES
#define _USE_MATH_DEFINES
#endif

#include <lime/util.hpp>
#include <CImg.h>
#include <cmath>

using namespace cimg_library;

namespace lime{

	// Forward-declaration of the Segmentation class
	template<typename U> class Segmentation;

	template<typename T = int> class Algorithm{

		// friend declaration for the Segmentation class
		friend class Segmentation<T>;

	public:
		Algorithm(bool _applyMedian = false, unsigned int _medianSize = 3)
			:applyMedian(_applyMedian),medianSize(_medianSize){}
		virtual ~Algorithm(){}

	protected:
		
		// Abstract functions
		
		/** 
		* @brief Processes the image and generating a bit mask out of it
		*/
		virtual CImg<T>* processImage(const CImg<T> &img);

		/** 
		* @brief Processes the image and generating a bit mask out of it
		*/
		virtual void processImage(CImg<T> *img);

		/** 
		* @brief Transforms the image data from RGB the target color space or performs other transformations
		*/
		virtual CImg<double>* transformImage(const CImg<T> &img) = 0;

		/** 
		* @brief Uses some thresholds to determine whether a pixel is skin or non-skin
		*/
		virtual bool skinThresholds(double c1, double c2, double c3) = 0;

		// Members


		/** 
		* @brief Determines if a median filter should be applied before the transformation or not
		*/
		bool applyMedian;

		/** 
		* @brief Size of the median filter
		*/
		unsigned int medianSize;
	};

	template<typename T>
	CImg<T>* lime::Algorithm<T>::processImage( const CImg<T> &img )
	{
		CImg<T> tmpImg(img);

		this->processImage(&tmpImg);

		return new CImg<T>(tmpImg);
	}

	template<typename T>
	void lime::Algorithm<T>::processImage( CImg<T> *img )
	{

		int _width = img->width();
		int _height = img->height();

		CImg<T> medianImg;

		// Applying a median filter
		if (this->applyMedian)
		{
			medianImg = img->get_blur_median(this->medianSize);
		}
		else
		{
			medianImg.assign(*img);
		}

		// Changing img to accept the new data with only 1 (binary) channel, in-place
		img->assign(_width,_height,1,1);

		CImg<double> *transformedImg = this->transformImage(medianImg);

		double c1,c2,c3;

		for (int y = 0; y < _height; y++)
		{
			for (int x = 0; x < _width; x++)
			{
				c1 = (*transformedImg)(x,y,0,0);
				c2 = (*transformedImg)(x,y,0,1);
				c3 = (*transformedImg)(x,y,0,2);


				if (this->skinThresholds(c1,c2,c3))
				{
					(*img)(x,y,0,0) = 1;
				}
				else
				{
					(*img)(x,y,0,0) = 0;
				}
			}
		}

		// some cleanup
		delete transformedImg;
	}
}