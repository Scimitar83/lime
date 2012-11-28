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
/// @file ColorimetricHSVAlgorithm1.hpp
/// @brief Contains the ColorimetricHSVAlgorithm1 class
/// @author Alexander Schoch
/// @date Nov 13, 2012 - First creation
/// @package lime
///

#include <lime/Algorithm.hpp>

#ifndef M_PI
#define M_PI 3.14159265358979323846f
#endif

namespace lime{

	///
	/// @class   ColorimetricHSVAlgorithm1
	///
	/// @version 0.3.0
	///
	/// @brief Subclass of Algorithm
	///
	/// @detail Subclass of Algorithm which uses a transformation to and fixed thresholds of the HSV color space for colorimetric skin segmentation 
	///
	/// @author  Aleander Schoch
	/// @date    Nov 14, 2012 - First creation and implementation
	///
	template<typename T> class ColorimetricHSVAlgorithm1: public Algorithm<T>{

	public:

		///
		/// @brief The constructor that passes all arguments to the constructor of the base class
		///
		ColorimetricHSVAlgorithm1(bool _applyMedian = false, unsigned int _medianSize = 3, bool _applyGrow = false, unsigned int _growCount = 1, unsigned int _growSize = 2, bool _applyShrink = false,
			unsigned int _shrinkCount = 1, unsigned int _shrinkSize = 2, bool _applyFixedGrowShrink = false, unsigned int _fixedGrowShrinkCount = 1,
			unsigned int _fixedGrowShrinkSize = 2, bool _applyGrowBeforeShrink = true, bool _applyRegionClearing = false)
			:Algorithm<T>(_applyMedian,_medianSize, _applyGrow, _growCount, _growSize, _applyShrink, _shrinkCount, _shrinkSize, _applyFixedGrowShrink, _fixedGrowShrinkCount, _fixedGrowShrinkSize,
			_applyGrowBeforeShrink, _applyRegionClearing)
		{
			s_lower_1 = 10;
			v_lower_1 = 40;
			v_multiplier_1 = 0.1;
			v_addend_1 = 110;
			v_multiplier_2 = -0.4;
			v_addend_2 = 75;
			v_multiplier_3 = 0.08;
			v_addend_3 = 100;
			v_multiplier_4 = 0.5;
			h_multiplier_1 = 0.5;
			h_addend_1 = 35;
		}

		///
		/// @brief Basis destructor
		///
		virtual ~ColorimetricHSVAlgorithm1(){}

		// Getter / Setter

		virtual lime::Threshold S_Lower_1() const { return this->s_lower_1; }
		virtual void S_Lower_1(lime::Threshold val) { this->s_lower_1 = val; }
		virtual lime::Threshold V_Lower_1() const { return this->v_lower_1; }
		virtual void V_Lower_1(lime::Threshold val) { this->v_lower_1 = val; }
		virtual lime::Threshold V_Multiplier_1() const { return this->v_multiplier_1; }
		virtual void V_Multiplier_1(lime::Threshold val) { this->v_multiplier_1 = val; }
		virtual lime::Threshold V_Addend_1() const { return this->v_addend_1; }
		virtual void V_Addend_1(lime::Threshold val) { this->v_addend_1 = val; }
		virtual lime::Threshold V_Multiplier_2() const { return this->v_multiplier_2; }
		virtual void V_Multiplier_2(lime::Threshold val) { this->v_multiplier_2 = val; }
		virtual lime::Threshold V_Addend_2() const { return this->v_addend_2; }
		virtual void V_Addend_2(lime::Threshold val) { this->v_addend_2 = val; }
		virtual lime::Threshold V_Multiplier_3() const { return this->v_multiplier_3; }
		virtual void V_Multiplier_3(lime::Threshold val) { this->v_multiplier_3 = val; }
		virtual lime::Threshold V_Addend_3() const { return this->v_addend_3; }
		virtual void V_Addend_3(lime::Threshold val) { this->v_addend_3 = val; }
		virtual lime::Threshold V_Multiplier_4() const { return this->v_multiplier_4; }
		virtual void V_Multiplier_4(lime::Threshold val) { this->v_multiplier_4 = val; }
		virtual lime::Threshold H_Multiplier_1() const { return this->h_multiplier_1; }
		virtual void H_Multiplier_1(lime::Threshold val) { this->h_multiplier_1 = val; }
		virtual lime::Threshold H_Addend_1() const { return this->h_addend_1; }
		virtual void H_Addend_1(lime::Threshold val) { this->h_addend_1 = val; }

	protected:

		// virtual functions
		
		///
		/// @brief Transforms the image data from the RGB color space to the HSV color space.
		///
		virtual CImg<double>* transformImage(const CImg<T> &img);

		///
		/// @brief Uses algorithm-specific thresholds in the HSV color space to determine whether a pixel is skin or non-skin.
		/// @param c1 The first channel of the image data (here: H)
		/// @param c2 The second channel of the image data (here: S)
		/// @param c3 The third channel of the image data (here: V)
		/// @return true = skin, false = no skin
		///
		virtual bool skinThresholds(double c1, double c2, double c3);

		//Thresholds
		Threshold s_lower_1;
		Threshold v_lower_1;
		Threshold v_multiplier_1;
		Threshold v_addend_1;
		Threshold v_multiplier_2;
		Threshold v_addend_2;
		Threshold v_multiplier_3;
		Threshold v_addend_3;
		Threshold v_multiplier_4;
		Threshold h_multiplier_1;
		Threshold h_addend_1;
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

	if (c2 < this->s_lower_1 || c3 < this->v_lower_1)
	{
		return false;
	}


	if (c2 > -c1 - this->v_multiplier_1*c3 + this->v_addend_1)
	{
		return false;
	}



	if (c1 > this->v_multiplier_2	* c3 + this->v_addend_2)
	{
		return false;
	}



	if (c1 >= 0)
	{
		if (c2 > this->v_multiplier_3*(this->v_addend_3 - c3)*c1 + this->v_multiplier_4*c3)
		{
			return false;
		}
	} 
	else
	{
		if (c2 > this->h_multiplier_1*c1 + this->h_addend_1)
		{
			return false;
		}
	}

	return true;
}