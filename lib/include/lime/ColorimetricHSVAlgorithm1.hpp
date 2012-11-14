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
/// \file    ColorimetricHSVAlgorithm1.hpp
/// \class   ColorimetricHSVAlgorithm1
///
/// \package lime
/// \version 0.2.0
///
/// \brief Subclass of Algorithm
///
/// \detail Subclass of Algorithm which uses a transformation to and fixed thresholds of the HSV color space for colorimetric skin segmentation 
///
/// \author  Aleander Schoch
/// \date    Nov 14, 2012 - first creation and implementation
///

#include <lime/Algorithm.hpp>

#ifndef M_PI
#define M_PI 3.14159265358979323846f
#endif

namespace lime{

	template<typename T> class ColorimetricHSVAlgorithm1: public Algorithm<T>{

	public:

		ColorimetricHSVAlgorithm1(bool _applyMedian = false, unsigned int _medianSize = 3)
			:Algorithm<T>(_applyMedian,_medianSize){}
		virtual ~ColorimetricHSVAlgorithm1(){}

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
CImg<double>* lime::ColorimetricHSVAlgorithm1<T>::transformImage(const CImg<T> &img )
{
	CImg<double> *resImg = new CImg<double>();
	*resImg = img.get_RGBtoHSV();

	return resImg;
}

template<typename T>
bool lime::ColorimetricHSVAlgorithm1<T>::skinThresholds( double c1, double c2, double c3 )
{
	c2 *= 100;
	c3 *= 100;

	if (c2 < 10 || c3 < 40)
	{
		return false;
	}

	if (c2 > -c1 - 0.1*c3 + 110)
	{
		return false;
	}

	if (c1 > -0.4*c3 + 75)
	{
		return false;
	}

	if (c1 >= 0)
	{
		if (c2 > 0.08*(100-c3)*c1 + 0.5*c3)
		{
			return false;
		}
	} 
	else
	{
		if (c2 > 0.5*c1 + 35)
		{
			return false;
		}
	}

	return true;
}