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
/// \file    ColorimetricHSIAlgorithm1.hpp
/// \class   ColorimetricHSIAlgorithm1
///
/// \package lime
/// \version 0.2.0
///
/// \brief Subclass of Algorithm
///
/// \detail Subclass of Algorithm which uses a transformation to and fixed thresholds of the HSI color space for colorimetric skin segmentation 
///
/// \author  Aleander Schoch
/// \date    Nov 13, 2012 - first creation and implementation
///

#include <lime/Algorithm.hpp>

#ifndef M_PI
#define M_PI 3.14159265358979323846f
#endif

// Thresholds
static const double threshold_H_lower1 = 1.0;
static const double threshold_H_higher1 = 28.0;
static const double threshold_H_lower2 = 332.0;
static const double threshold_H_higher2 = 360.0;
static const double threshold_H_lower3 = 309.0;
static const double threshold_H_higher3 = 331.0;
static const double threshold_I = 0.4;
static const double threshold_S_lower = 13.0/255.0;
static const double threshold_S_higher1 = 110.0/255.0;
static const double threshold_S_higher2 = 75.0/255.0;

namespace lime{

	template<typename T> class ColorimetricHSIAlgorithm1: public Algorithm<T>{

	public:

		ColorimetricHSIAlgorithm1(float _driftValue = -1.0, bool _applyMedian = false):Algorithm<T>(_driftValue,_applyMedian,false){}
		virtual ~ColorimetricHSIAlgorithm1(){}

	protected:

		// virtual functions
		
		/** 
		* @brief Transforms the image data from RGB to HSI color space
		*/
		virtual CImg<double>* transformImage(const CImg<T> &img);

		/** 
		* @brief Uses some thresholds to determine whether a pixel is skin or non-skin
		*/
		virtual bool skinThresholds(double c1, double c2, double c3);
		
	};

}

template<typename T>
CImg<double>* lime::ColorimetricHSIAlgorithm1<T>::transformImage(const CImg<T> &img )
{
	CImg<double> *resImg = new CImg<double>();
	*resImg = img.get_RGBtoHSI();
	
	return resImg;
}

template<typename T>
bool lime::ColorimetricHSIAlgorithm1<T>::skinThresholds( double c1, double c2, double c3 )
{
	if (c3 < threshold_I)
	{
		return false;
	}

	if (threshold_S_lower && c2 < threshold_S_higher2)
	{
		return (c1 > threshold_H_lower3 && c1 < threshold_H_higher3);
	}

	if (c2 > threshold_S_lower && c2 < threshold_S_higher1)
	{
		return ((threshold_H_lower1 < c1 && c1 < threshold_H_higher1) || (threshold_H_lower2 < c1 && c1 < threshold_H_higher2));
	}

	return false;
}