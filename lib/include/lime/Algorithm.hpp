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

	typedef double Threshold;

	struct Point2D 
	{
	public:
		unsigned int x;
		unsigned int y;
	};

	// Forward-declaration of the Segmentation class
	template<typename U> class Segmentation;

	template<typename T = int> class Algorithm{

		// friend declaration for the Segmentation class
		friend class Segmentation<T>;

	public:

		Algorithm(bool _applyMedian = false, unsigned int _medianSize = 3, bool _applyGrow = false, unsigned int _growCount = 1, unsigned int _growSize = 2, bool _applyShrink = false,
			unsigned int _shrinkCount = 1, unsigned int _shrinkSize = 2, bool _applyFixedGrowShrink = false, unsigned int _fixedGrowShrinkCount = 1,
			unsigned int _fixedGrowShrinkSize = 2, bool _applyGrowBeforeShrink = true)
			:applyMedian(_applyMedian),medianSize(_medianSize), applyGrow(_applyGrow), growCount(_growCount), growSize(_growSize), applyShrink(_applyShrink), shrinkCount(_shrinkCount),
			shrinkSize(_shrinkSize),applyFixedGrowShrink(_applyFixedGrowShrink), fixedGrowShrinkCount(_fixedGrowShrinkCount), fixedGrowShrinkSize(_fixedGrowShrinkSize), applyGrowBeforeShrink(_applyGrowBeforeShrink){}
		virtual ~Algorithm(){}

		//Getter / Setter


		virtual bool ApplyMedian() const { return applyMedian; }
		virtual void ApplyMedian(bool val) { applyMedian = val; }

		virtual unsigned int MedianSize() const { return medianSize; }
		virtual void MedianSize(unsigned int val) { medianSize = val; }

		virtual bool ApplyGrow() const { return applyGrow; }
		virtual void ApplyGrow(bool val) { applyGrow = val; }

		virtual unsigned int GrowCount() const { return growCount; }
		virtual void GrowCount(unsigned int val) { growCount = val; }

		virtual unsigned int GrowSize() const { return growSize; }
		virtual void GrowSize(unsigned int val) { growSize = val; }

		virtual bool ApplyShrink() const { return applyShrink; }
		virtual void ApplyShrink(bool val) { applyShrink = val; }

		virtual unsigned int ShrinkCount() const { return shrinkCount; }
		virtual void ShrinkCount(unsigned int val) { shrinkCount = val; }

		virtual unsigned int ShrinkSize() const { return shrinkSize; }
		virtual void ShrinkSize(unsigned int val) { shrinkSize = val; }

		virtual bool ApplyGrowBeforeShrink() const { return applyGrowBeforeShrink; }
		virtual void ApplyGrowBeforeShrink(bool val) { applyGrowBeforeShrink = val; }

		virtual bool ApplyFixedGrowShrink() const { return applyFixedGrowShrink; }
		virtual void ApplyFixedGrowShrink(bool val) { applyFixedGrowShrink = val; }

		virtual unsigned int FixedGrowShrinkCount() const { return fixedGrowShrinkCount; }
		virtual void FixedGrowShrinkCount(unsigned int val) { fixedGrowShrinkCount = val; }

		virtual unsigned int FixedGrowShrinkSize() const { return fixedGrowShrinkSize; }
		virtual void FixedGrowShrinkSize(unsigned int val) { fixedGrowShrinkSize = val; }

		virtual bool ApplyRegionClearing() const { return applyRegionClearing; }
		void ApplyRegionClearing(bool val) { applyRegionClearing = val; }

		virtual bool ApplyRegionPreDilute() const { return applyRegionPreDilute; }
		virtual void ApplyRegionPreDilute(bool val) { applyRegionPreDilute = val; }

		virtual unsigned int PreDiluteCount() const { return preDiluteCount; }
		virtual void PreDiluteCount(unsigned int val) { preDiluteCount = val; }

		virtual unsigned int PreDiluteSize() const { return preDiluteSize; }
		virtual void PreDiluteSize(unsigned int val) { preDiluteSize = val; }

	protected:

		// Abstract functions

		/** 
		* @brief Processes the image and generating a bit mask out of it
		*/
		virtual CImg<bool>* processImage(const CImg<T> &img);

		/** 
		* @brief Transforms the image data from RGB the target color space or performs other transformations
		*/
		virtual CImg<double>* transformImage(const CImg<T> &img) = 0;

		/** 
		* @brief Uses some thresholds to determine whether a pixel is skin or non-skin
		*/
		virtual bool skinThresholds(double c1, double c2, double c3) = 0;

		// Variables

		/** 
		* @brief Determines if a median filter should be applied before the transformation or not
		*/
		bool applyMedian;
		/** 
		* @brief Size of the median filter
		*/
		unsigned int medianSize;

		bool applyGrow;
		unsigned int growCount;
		unsigned int growSize;

		bool applyShrink;
		unsigned int shrinkCount;
		unsigned int shrinkSize;

		bool applyFixedGrowShrink;
		unsigned int fixedGrowShrinkCount;
		unsigned int fixedGrowShrinkSize;

		bool applyGrowBeforeShrink;

		bool applyRegionClearing;

		bool applyRegionPreDilute;
		unsigned int preDiluteCount;
		unsigned int preDiluteSize;

	private:

		void growShrinkAlgorithm(CImg<bool> *img, const unsigned int count, const unsigned int size);

		void growAlgorithm(CImg<bool> *img, const unsigned int count, const unsigned int size);

		void shrinkAlgorithm(CImg<bool> *img, const unsigned int count, const unsigned int size);

		void pixelLabeling(CImg<bool> *img, int x, int y);

		void deleteMinorRegions(CImg<bool> *img);

		// Member

		unsigned int regionCount;

		unsigned int biggestRegion;

		std::vector< std::vector <Point2D> > labeledPixels;

		std::vector<unsigned int> regionSizes;

		CImg<unsigned int> labelMask;
	};

	template<typename T>
	CImg<bool>* lime::Algorithm<T>::processImage( const CImg<T> &img )
	{

		int _width = img.width();
		int _height = img.height();

		CImg<T> medianImg;

		// Applying a median filter
		if (this->applyMedian)
		{
			medianImg = img.get_blur_median(this->medianSize);
		}
		else
		{
			medianImg.assign(img);
		}		

		// Changing img to accept the new data with only 1 (binary) channel, in-place
		CImg<bool> *resImg = new CImg<bool>(_width,_height,1,1);

		CImg<double> *transformedImg = this->transformImage(medianImg);

		double c1,c2,c3;

		// Depending whether or not region clearing is active (which means that only the biggest region will remain at the end) some labeling is added to the loop
		if (!this->applyRegionClearing)
		{
			for (int y = 0; y < _height; y++)
			{
				for (int x = 0; x < _width; x++)
				{
					c1 = (*transformedImg)(x,y,0,0);
					c2 = (*transformedImg)(x,y,0,1);
					c3 = (*transformedImg)(x,y,0,2);

					(*resImg)(x,y,0,0) = this->skinThresholds(c1,c2,c3);
				}
			}
		}
		else
		{
			// Reset of the corresponding member variables
			this->regionCount = 0;
			this->biggestRegion = 0;
			this->regionSizes = std::vector<unsigned int>();
			this->regionSizes.reserve((_width * _height)/4);
			this->labeledPixels = std::vector< std::vector <Point2D> >();
			this->labeledPixels.reserve((_width*_height)/4);
			this->labelMask = CImg<unsigned int>(_width,_height,1,1,0);

			for (int y = 0; y < _height; y++)
			{
				for (int x = 0; x < _width; x++)
				{
					c1 = (*transformedImg)(x,y,0,0);
					c2 = (*transformedImg)(x,y,0,1);
					c3 = (*transformedImg)(x,y,0,2);

					if (this->skinThresholds(c1,c2,c3))
					{
						(*resImg)(x,y,0,0) = true;
						pixelLabeling(resImg,x,y);
					}
					else
					{
						(*resImg)(x,y,0,0) = false;
					}
				}
			}

			this->deleteMinorRegions(resImg);
		}


		// Applying Grow and / or Shrink Algorithm

		if (this->applyGrowBeforeShrink)
		{
			if (this->applyGrow)
			{
				this->growAlgorithm(resImg, this->growCount, this->growSize);
			}

			if (this->applyShrink)
			{
				this->shrinkAlgorithm(resImg, this->growCount, this->growSize);
			}
		}
		else
		{
			if (this->applyShrink)
			{
				this->shrinkAlgorithm(resImg, this->growCount, this->growSize);
			}

			if (this->applyGrow)
			{
				this->growAlgorithm(resImg, this->growCount, this->growSize);
			}
		}

		//Applying a fixed GrowShrink-Algorithm
		if (this->applyFixedGrowShrink)
		{
			this->growShrinkAlgorithm(resImg, this->fixedGrowShrinkCount, this->fixedGrowShrinkSize);
		}

		// some cleanup
		delete transformedImg;

		return resImg;
	}

	template<typename T>
	void lime::Algorithm<T>::growShrinkAlgorithm( CImg<bool> *img, const unsigned int count, const unsigned int size )
	{
		for (unsigned int i = 0; i < count; i++ )
		{
			if (this->applyFixedGrowShrink)
			{
				this->growAlgorithm(img, 1, size);
				this->shrinkAlgorithm(img, 1, size);
			}
			else
			{				
				this->shrinkAlgorithm(img, 1, size);
				this->growAlgorithm(img, 1, size);
			}
		}
	}

	template<typename T>
	void lime::Algorithm<T>::growAlgorithm( CImg<bool> *img, const unsigned int count, const unsigned int size)
	{
		for(unsigned int i = 0; i < count; i++)
		{
			img->dilate(size);
		}
	}

	template<typename T>
	void lime::Algorithm<T>::shrinkAlgorithm( CImg<bool> *img, const unsigned int count, const unsigned int size)
	{
		for(unsigned int i = 0; i < count; i++)
		{
			img->erode(size);
		}
	}

	template<typename T>
	inline void lime::Algorithm<T>::pixelLabeling(CImg<bool> *img, int x, int y )
	{
		// Pixel not yet labeled
		if (this->labelMask(x,y,0,0) == 0)
		{
			this->labelMask(x,y,0,0) = this->regionCount++;

			std::vector<Point2D> tempVect;
			Point2D tempPoint= {x,y};
			tempVect.push_back(tempPoint);
			this->labeledPixels.push_back(tempVect);
			this->regionSizes.push_back(1);
		}

		// Compare labeling with the adjacent pixels (left-up, left, up, right-up)
		for (int k = 0; k < 4; k++)
		{
			int tempX, tempY;

			switch (k)
			{
				case 0: tempX = x-1;
						tempY = y-1;
						break;
				case 1: tempX = x-1;
						tempY = y;
						break;
				case 2: tempX = x;
						tempY = y-1;
						break;
				case 3: tempX = x+1;
						tempY = y-1;
						break;
				default: break;
			}

			if (tempX < 0 || tempY < 0)
			{
				continue;
			}

			// If there is a skin pixel at the adjacent position
			if ((*img)(tempX,tempY,0,0))
			{
				

				unsigned int adjacentLabel = this->labelMask(tempX,tempY,0,0);
				unsigned int ownLabel = this->labelMask(x,y,0,0);

				if (adjacentLabel != ownLabel)
				{
					unsigned int smallerLabel;
					unsigned int biggerLabel;

					if (adjacentLabel < ownLabel)
					{
						smallerLabel = adjacentLabel;
						biggerLabel = ownLabel;
					}
					else
					{
						smallerLabel = ownLabel;
						biggerLabel = adjacentLabel;
					}

					std::vector<Point2D> *positionVect = &(this->labeledPixels.at(biggerLabel));

					// Change the labels to the new labels

					for (unsigned int i = 0; i < positionVect->size();i++)
					{
						Point2D *posPoint = &(positionVect->at(i));

						this->labelMask(posPoint->x,posPoint->y,0,0) = smallerLabel;							
					}

					this->regionSizes.at(smallerLabel) += this->regionSizes.at(biggerLabel);
					this->regionSizes.at(biggerLabel) = 0;

					for(unsigned int i = 0; i < positionVect->size();i++)
					{
						this->labeledPixels.at(smallerLabel).push_back(positionVect->at(i));
					}

					this->labeledPixels.at(biggerLabel).clear();

					if (this->regionSizes.at(smallerLabel) > this->regionSizes.at(this->biggestRegion))
					{
						this->biggestRegion = smallerLabel;
					}
				}
			}
		}
	}

	template<typename T>
	void lime::Algorithm<T>::deleteMinorRegions( CImg<bool> *img )
	{
		for (unsigned int i = 0; i < this->labeledPixels.size();i++)
		{
			// Region is biggest region or empty
			if (this->biggestRegion == i || this->regionSizes.at(i) == 0)
			{
				continue;
			}

			for (unsigned int j = 0; j < this->labeledPixels.at(i).size();j++)
			{
				Point2D *posPoint = &this->labeledPixels.at(i).at(j);

				(*img)(posPoint->x,posPoint->y,0,0) = false;
			}
		}
	}

}