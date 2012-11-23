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

#include <lime/Algorithm.hpp>

#ifndef M_PI
#define M_PI 3.14159265358979323846f
#endif

namespace lime{

	///
	/// @file ColorimetricYCbCrAlgorithm1.hpp
	/// @brief Contains the ColorimetricYCbCrAlgorithm1 class
	/// @author Alexander Schoch
	/// @date Nov 13, 2012 - First creation
	/// @package lime
	///

	///
	/// @class   ColorimetricYCbCrAlgorithm1
	///
	/// @version 0.2.0
	///
	/// @brief Subclass of Algorithm
	///
	/// @detail Subclass of Algorithm which uses a transformation to and fixed thresholds of the YCbCr color space for colorimetric skin segmentation 
	///
	/// @author  Aleander Schoch
	/// @date    Nov 14, 2012 - First creation and implementation
	///
	template<typename T> class ColorimetricYCbCrAlgorithm1: public Algorithm<T>{

	public:

		ColorimetricYCbCrAlgorithm1(bool _applyMedian = false, unsigned int _medianSize = 3, bool _applyGrow = false, unsigned int _growCount = 1, unsigned int _growSize = 2, bool _applyShrink = false,
			unsigned int _shrinkCount = 1, unsigned int _shrinkSize = 2, bool _applyFixedGrowShrink = false, unsigned int _fixedGrowShrinkCount = 1,
			unsigned int _fixedGrowShrinkSize = 2, bool _applyGrowBeforeShrink = true, bool _applyRegionClearing = false)
			:Algorithm<T>(_applyMedian,_medianSize, _applyGrow, _growCount, _growSize, _applyShrink, _shrinkCount, _shrinkSize, _applyFixedGrowShrink, _fixedGrowShrinkCount, _fixedGrowShrinkSize,
			_applyGrowBeforeShrink, _applyRegionClearing)
		{
			this->cb_lower = 77.0;
			this->cb_higher = 127.0;
			this->cr_lower = 133.0;
			this->cr_higher = 173.0;
		}
		virtual ~ColorimetricYCbCrAlgorithm1(){}

		// Getter / Setter

		virtual lime::Threshold Cb_lower() const { return this->cb_lower; }
		virtual void Cb_lower(lime::Threshold val) { this->cb_lower = val; }
		virtual lime::Threshold Cb_higher() const { return this->cb_higher; }
		virtual void Cb_higher(lime::Threshold val) { this->cb_higher = val; }
		virtual lime::Threshold Cr_lower() const { return this->cr_lower; }
		virtual void Cr_lower(lime::Threshold val) { this->cr_lower = val; }
		virtual lime::Threshold Cr_higher() const { return this->cr_higher; }
		virtual void Cr_higher(lime::Threshold val) { this->cr_higher = val; }

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

		Threshold cb_lower;
		Threshold cb_higher;
		Threshold cr_lower;
		Threshold cr_higher;


	};

}

template<typename T>
CImg<double>* lime::ColorimetricYCbCrAlgorithm1<T>::transformImage(const CImg<T> &img )
{
	CImg<double> *resImg = new CImg<double>();
	*resImg = img.get_RGBtoYCbCr();

	return resImg;
}

template<typename T>
bool lime::ColorimetricYCbCrAlgorithm1<T>::skinThresholds( double c1, double c2, double c3 )
{
	if(this->cb_lower <= c2 <= this->cb_higher)
	{
		return (this->cr_lower <= c3 && c3 <= this->cr_higher);
	}

	return false;
}