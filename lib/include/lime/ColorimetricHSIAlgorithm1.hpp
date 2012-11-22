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


namespace lime{

	template<typename T> class ColorimetricHSIAlgorithm1: public Algorithm<T>{

	public:

		ColorimetricHSIAlgorithm1(bool _applyMedian = false, unsigned int _medianSize = 3, bool _applyGrow = false, unsigned int _growCount = 1, unsigned int _growSize = 2, bool _applyShrink = false,
			unsigned int _shrinkCount = 1, unsigned int _shrinkSize = 2, bool _applyFixedGrowShrink = false, unsigned int _fixedGrowShrinkCount = 1,
			unsigned int _fixedGrowShrinkSize = 2, bool _applyGrowBeforeShrink = true)
			:Algorithm<T>(_applyMedian,_medianSize, _applyGrow, _growCount, _growSize, _applyShrink, _shrinkCount, _shrinkSize, _applyFixedGrowShrink, _fixedGrowShrinkCount, _fixedGrowShrinkSize,
			_applyGrowBeforeShrink)
		{
			// Initial values for all thresholds

			this->h_lower_1 = 1.0;
			this->h_higher_1 = 28.0;
			this->h_lower_2 = 332.0;
			this->h_higher_2 = 360.0;
			this->h_lower_3 = 309.0;
			this->h_higher_3 = 331.0;
			this->i_lower = 0.4;
			this->s_lower = 13.0/255.0;
			this->s_higher_1 = 110.0/255.0;
			this->s_higher_2 = 75.0/255.0;
		}
		virtual ~ColorimetricHSIAlgorithm1(){}

		// Getter / Setter

		virtual lime::Threshold H_Lower_1() const { return h_lower_1; }
		virtual void H_Lower_1(lime::Threshold val) { h_lower_1 = val; }
		virtual lime::Threshold H_Higher_1() const { return h_higher_1; }
		virtual void H_Higher_1(lime::Threshold val) { h_higher_1 = val; }
		virtual lime::Threshold H_Lower_2() const { return h_lower_2; }
		virtual void H_Lower_2(lime::Threshold val) { h_lower_2 = val; }
		virtual lime::Threshold H_Higher_2() const { return h_higher_2; }
		virtual void H_Higher_2(lime::Threshold val) { h_higher_2 = val; }
		virtual lime::Threshold H_Lower_3() const { return h_lower_3; }
		virtual void H_Lower_3(lime::Threshold val) { h_lower_3 = val; }
		virtual lime::Threshold H_Higher_3() const { return h_higher_3; }
		virtual void H_Higher_3(lime::Threshold val) { h_higher_3 = val; }
		virtual lime::Threshold I_Lower() const { return i_lower; }
		virtual void I_Lower(lime::Threshold val) { i_lower = val; }
		virtual lime::Threshold S_Lower() const { return s_lower; }
		virtual void S_Lower(lime::Threshold val) { s_lower = val; }
		virtual lime::Threshold S_Higher_1() const { return s_higher_1; }
		virtual void S_Higher_1(lime::Threshold val) { s_higher_1 = val; }
		virtual lime::Threshold S_Higher_2() const { return s_higher_2; }
		virtual void S_Higher_2(lime::Threshold val) { s_higher_2 = val; }

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

		// Thresholds

		Threshold h_lower_1;
		Threshold h_higher_1;
		Threshold h_lower_2;
		Threshold h_higher_2;
		Threshold h_lower_3;
		Threshold h_higher_3;
		Threshold i_lower;
		Threshold s_lower;
		Threshold s_higher_1;
		Threshold s_higher_2;
		
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
	if (c3 < this->i_lower)
	{
		return false;
	}

	if (this->s_lower && c2 < this->s_higher_2)
	{
		return (c1 > this->h_lower_3 && c1 < this->h_higher_3);
	}

	if (c2 > this->s_lower && c2 < this->s_higher_1)
	{
		return ((this->h_lower_1 < c1 && c1 < this->h_higher_1) || (this->h_lower_2 < c1 && c1 < this->h_higher_2));
	}

	return false;
}