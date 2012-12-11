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
/// @file Algorithm.hpp
/// @brief Contains the Algorithm base class as well as the Threshold typedef and the Point2D struct
/// @author Alexander Schoch
/// @date Nov 13, 2012 - First creation
/// @package lime
///


#ifndef _USE_MATH_DEFINES
#define _USE_MATH_DEFINES
#endif

#include <lime/util.hpp>
#include <CImg.h>
#include <cmath>
#include <opencv2/opencv.hpp>
#include <opencv2/flann/flann.hpp>

using namespace cimg_library;

///
/// @namespace lime
/// @brief The default namespace of the lime library
///
namespace lime{

	///
	/// @typedef Threshold
	/// @brief Simple typedef that defines a Threshold as a floating point variable.
	///
	typedef double Threshold;

	typedef cv::flann::GenericIndex<cv::flann::L2_Simple<double> > SearchTree;

#ifndef DOXYGEN_SHOULD_SKIP_THIS // This template forward declaration produces some problems in combination with doxygen so I disables doxygen for it

	template<typename U> class Segmentation;

#endif

	///
	/// @class Algorithm
	///
	/// @version 0.3.0
	///
	/// @brief Abstract base class for all Skin Segmentation algorithms.
	///
	/// @detail This abstract template class features a lot of the functionality of the skin segmentation process and provides it to the Segmentation class.
	/// The main function of its subclasses is to implement a threshold function and a function to transform the Image from the RGB space into another color space.
	/// Virtually everything else is handled here. Because of the fact that it is an abstract class it builds a "Strategic Pattern" together with Segmentation and its subclasses.
	///
	/// @author Aleander Schoch
	/// @date Nov 13, 2012 - First creation and implementation
	/// @date Nov 23, 2012 - Region grow/shrink and region clearing (only the largest region remains) implemented
	/// @tparam T - Can be of any basic data type and should be the same as the one of the input image (e.g. double or char).
	///
	template<typename T = int> class Algorithm{

		friend class Segmentation<T>; ///< Friend declaration of the Segmentation class

	public:

		///
		/// @brief A constructor with default values for each parameter. Can be used to set all the possible variables (except the thresholds of the base class) right at the start.
		///
		Algorithm(bool _applyMedian = false, unsigned int _medianSize = 3, bool _applyGrow = false, unsigned int _growCount = 1, unsigned int _growSize = 2, bool _applyShrink = false,
			unsigned int _shrinkCount = 1, unsigned int _shrinkSize = 2, bool _applyFixedGrowShrink = false, unsigned int _fixedGrowShrinkCount = 1,
			unsigned int _fixedGrowShrinkSize = 2, bool _applyGrowBeforeShrink = true, bool _applyRegionClearing = false)
			:applyMedian(_applyMedian),medianSize(_medianSize), applyGrow(_applyGrow), growCount(_growCount), growSize(_growSize), applyShrink(_applyShrink), shrinkCount(_shrinkCount),
			shrinkSize(_shrinkSize),applyFixedGrowShrink(_applyFixedGrowShrink), fixedGrowShrinkCount(_fixedGrowShrinkCount), fixedGrowShrinkSize(_fixedGrowShrinkSize), 
			applyGrowBeforeShrink(_applyGrowBeforeShrink), applyRegionClearing(_applyRegionClearing){}
		///
		/// @brief The destructor of this class.
		///
		virtual ~Algorithm(){}

		virtual bool ApplyMedian() const { return applyMedian; } ///< Returns if the median filter is used or not.
		virtual void ApplyMedian(bool val) { applyMedian = val; } ///< Can activate / deactivate the use of the median filter.

		virtual unsigned int MedianSize() const { return medianSize; } ///< Returns the size of the median filter  (only meaningful if the median filter is activated).
		virtual void MedianSize(unsigned int val) { medianSize = val; } ///< Can be used to set the size of the median filter (only meaningful if the median filter is activated).

		virtual bool ApplyGrow() const { return applyGrow; } ///< Returns if the region grow algorithm is used.
		virtual void ApplyGrow(bool val) { applyGrow = val; } ///< Can activate / deactivate the region grow algorithm which can help to repair holes in the skin structure.

		virtual unsigned int GrowCount() const { return growCount; } ///< Returns the number of times the region grow algorithm is getting activated (only meaningful if the region grow algorithm is activated).
		virtual void GrowCount(unsigned int val) { growCount = val; } ///< Can be used to set the number of times the region grow algorithm should activate (only meaningful if the region grow algorithm is activated).

		virtual unsigned int GrowSize() const { return growSize; } ///< Returns the size of the kernel of the region grow algorithm (only meaningful if the region grow algorithm is activated).
		virtual void GrowSize(unsigned int val) { growSize = val; } ///< Can set the size of the kernel of the region grow algorithm (only meaningful if the region grow algorithm is activated).

		virtual bool ApplyShrink() const { return applyShrink; } ///< Returns if the region shrink algorithm is used.
		virtual void ApplyShrink(bool val) { applyShrink = val; } ///< Can activate / deactivate the region shrink algorithm which can help to remove small falsely detected skin areas.

		virtual unsigned int ShrinkCount() const { return shrinkCount; } ///< Returns the number of times the region shrink algorithm is getting activated (only meaningful if the region shrink algorithm is activated).
		virtual void ShrinkCount(unsigned int val) { shrinkCount = val; } ///< Can be used to set the number of times the region shrink algorithm should activate (only meaningful if the region shrink algorithm is activated).

		virtual unsigned int ShrinkSize() const { return shrinkSize; } ///< Returns the size of the kernel of the region shrink algorithm (only meaningful if the region shrink algorithm is activated).
		virtual void ShrinkSize(unsigned int val) { shrinkSize = val; }///< Can set the size of the kernel of the region shrink algorithm (only meaningful if the region shrink algorithm is activated).

		virtual bool ApplyGrowBeforeShrink() const { return applyGrowBeforeShrink; } ///< Returns if the grow algorithm (ALL cycles of it) is used before the shrink algorithm or the other way around.
		virtual void ApplyGrowBeforeShrink(bool val) { applyGrowBeforeShrink = val; } ///< Can be used to determine if the grow algorithm (ALL cycles of it) should be used before the shrink algorithm or the other way around.

		virtual bool ApplyFixedGrowShrink() const { return applyFixedGrowShrink; } ///< Returns if the fixed region grow-shrink algorithm is used.
		virtual void ApplyFixedGrowShrink(bool val) { applyFixedGrowShrink = val; } ///< Can be used to activate / deactivate the fixed region grow-shrink algorithm.

		virtual unsigned int FixedGrowShrinkCount() const { return fixedGrowShrinkCount; } ///< Returns the number of times the fixed region grow-shrink algorithm is getting activated (only meaningful if it is activated).
		virtual void FixedGrowShrinkCount(unsigned int val) { fixedGrowShrinkCount = val; } ///< Can be used to set the number of times the fixed region grow-shrink algorithm should activate (only meaningful if it is activated).

		virtual unsigned int FixedGrowShrinkSize() const { return fixedGrowShrinkSize; } ///< Returns the size of the kernel of the fixed region grow-shrink algorithm (only meaningful if it is activated).
		virtual void FixedGrowShrinkSize(unsigned int val) { fixedGrowShrinkSize = val; } ///< Can set the size of the kernel of the fixed region grow-shrink algorithm (only meaningful if is activated).

		virtual bool ApplyRegionClearing() const { return applyRegionClearing; } ///< Returns if the region clearing algorithm is used (deletes all skin regions but the biggest one).
		void ApplyRegionClearing(bool val) { applyRegionClearing = val; } ///< Can be used to activate / deactivate the region clearing algorithm (deletes all skin regions but the biggest one).

	protected:

		// Abstract functions

		///
		/// @brief Processes the image and generates a bit mask out of it
		/// @param img A reference to the original image data in CImg format with an arbitrary numerical standard data type
		/// @return A bit mask in CImg<bool> format with the same width and height as the input image where true = skin and false = no skin
		///
		virtual CImg<bool>* processImage(const CImg<T> &img);

		///
		/// @brief Transforms the image data from the RGB color space to the target color space or performs other transformations. Has to be implemented by a specialized algorithm.
		///
		virtual CImg<double>* transformImage(const CImg<T> &img) = 0;

		///
		/// @brief Uses algorithm-specific thresholds to determine whether a pixel is skin or non-skin. Has to be implemented by a specialized algorithm.
		/// @param c1 The first channel of the image data (e.g. R in RGB)
		/// @param c2 The second channel of the image data (e.g. G in RGB)
		/// @param c3 The third channel of the image data (e.g. B in RGB)
		/// @return true = skin, false = no skin
		///
		virtual bool skinThresholds(double c1, double c2, double c3) = 0;

		virtual CImg<int>* getDistanceMapOfMask(CImg<bool> &mask, bool singleRegion = false);

		// Variables

		///
		/// @brief Determines if a median filter should be applied before the transformation or not.
		///
		bool applyMedian;

		///
		/// @brief Size (radius) of the median filter kernel
		///
		unsigned int medianSize;

		///
		/// @brief Determines if the region grow algorithm should be applied to the bit mask.
		///
		bool applyGrow;

		///
		/// @brief The number of times the grow algorithm should be applied (only has an effect if applyGrow = true).
		///
		unsigned int growCount;

		///
		/// @brief The size of the grow algorithm kernel (only has an effect if applyGrow = true).
		///
		unsigned int growSize;

		///
		/// @brief Determines if the region shrink algorithm should be applied to the bit mask.
		///
		bool applyShrink;

		///
		/// @brief The number of times the shrink algorithm should be applied (only has an effect if applyShrink = true).
		///
		unsigned int shrinkCount;

		///
		/// @brief The size of the shrink algorithm kernel (only has an effect if applyShrink = true).
		///
		unsigned int shrinkSize;

		///
		/// @brief Determines if a fixed region grow and shrink algorithm should be applied to the bit mask (always 1:1).
		///
		bool applyFixedGrowShrink;

		///
		/// @brief The number of times the fixed grow and shrink algorithm should be applied (only has an effect if applyFixedGrowShrink = true).
		///
		unsigned int fixedGrowShrinkCount;

		///
		/// @brief The size of the fixed grow and shrink algorithm kernel (only has an effect if applyFixedGrowShrink = true).
		///
		unsigned int fixedGrowShrinkSize;

		///
		/// @brief Determines if the region grow algorithm comes before the region shrink algorithm or the other way around.
		///
		bool applyGrowBeforeShrink;

		///
		/// @brief Determines if the region clearing algorithm should be used (which indexes regions and deletes all but the largest one)
		///
		bool applyRegionClearing;

	private:

		///
		/// @brief Applies a grow and shrink algorithm on the bit mask (grow:shrink = 1:1).
		/// @param img The bit mask
		/// @param count Number of times the algorithm should be used
		/// @param size Size of the algorithm kernel
		///
		virtual void growShrinkAlgorithm(CImg<bool> *img, const unsigned int count, const unsigned int size);

		///
		/// @brief Applies a grow algorithm on the bit mask.
		/// @param img The bit mask
		/// @param count Number of times the algorithm should be used
		/// @param size Size of the algorithm kernel
		///
		virtual void growAlgorithm(CImg<bool> *img, const unsigned int count, const unsigned int size);

		///
		/// @brief Applies a shrink algorithm on the bit mask.
		/// @param img The bit mask
		/// @param count Number of times the algorithm should be used
		/// @param size Size of the algorithm kernel
		///
		virtual void shrinkAlgorithm(CImg<bool> *img, const unsigned int count, const unsigned int size);

		///
		/// @brief Indexes and labels all pixels in a picture by comparing it with some of its neighbors.
		/// @param img The bit mask
		/// @param x X-coordinate of the current pixel
		/// @param y Y-coordinate of the current pixel
		///
		virtual void pixelLabeling(CImg<bool> *img, int x, int y);

		///
		/// @brief Used for the region clearing. Deletes all but the biggest skin region in the bit mask.
		/// @param img The bit mask
		///
		virtual void deleteMinorRegions(CImg<bool> *img);

		///
		/// @brief Uses a bit mask to automatically determine seed points for skin and non-skin pixels
		/// @param skin True if skin seed pixels should be retrieved, false if non-skin seed pixels should be retrieved 
		/// @singleRegion Only determines seed pixels on the border of the first region that is being detected
		/// @mask The bit mask
		/// @applyRegionChange True if a shrink or grow (based on skin / non-skin) algorithm should be used prior to the seed pixel detection
		/// @regionChangeCount Number of times the shrink / grow algorithm should be used
		/// @regionChangeSize Size of the kernel for the shrink / grow algorithm
		///
		virtual std::vector<BinarySeed>* getSeeds(bool skin, bool singleRegion, const CImg<bool> &mask, bool applyRegionChange, unsigned int regionChangeCount, unsigned int regionChangeSize);

		virtual cv::Mat_<double> findDistances(SearchTree& st, const cv::Mat_<double>& queryPoints);

		// Member

		///
		/// @brief Used for the region clearing. Keeps track of the number of different regions in the bit mask.
		///
		unsigned int regionCount;

		///
		/// @brief Used for the region clearing. Label of the biggest region in the bit mask.
		///
		unsigned int biggestRegion;

		///
		/// @brief Used for the region clearing. An outer vector for each label. An inner vector for the coordinates of each pixel of one label.
		///
		std::vector< std::vector <Point2D> > labeledPixels;

		///
		/// @brief Used for the region clearing. A vector for each label which contains the number of pixels belonging to this label.
		///
		std::vector<unsigned int> regionSizes;

		///
		/// @brief Used for the region clearing. A temporary mask that contains the label of each pixel. 
		///
		CImg<unsigned int> labelMask;
	};

	template<typename T>
	CImg<bool>* lime::Algorithm<T>::processImage( const CImg<T> &img )
	{

		int _width = img.width();
		int _height = img.height();

		CImg<T> medianImg;

		// Applying a median filter if applyMedian = true
		if (this->applyMedian)
		{
			medianImg = img.get_blur_median(this->medianSize);
		}
		else
		{
			medianImg.assign(img);
		}		

		// The bit mask should have the same width and height but only one channel and bool variables for each pixel
		CImg<bool> *resImg = new CImg<bool>(_width,_height,1,1);

		// Changes the color space of the image data from RGB to the target color space
		CImg<double> *transformedImg = this->transformImage(medianImg);

		// The 3 channels of the image data
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

					// Uses the data from the skinThresholds method passing all 3 channels to it to determined whether the pixel is skin or not
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

					// Uses the data from the skinThresholds method passing all 3 channels to it to determined whether the pixel is skin or not. Also if the pixel is a skin pixel
					// the labeling method will be applied to it.
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
			if (this->applyGrowBeforeShrink)
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
		// Some setups if the current pixel is not yet labeled
		if (this->labelMask(x,y,0,0) == 0)
		{
			// Gets a new label for that pixel
			this->labelMask(x,y,0,0) = this->regionCount++;

			// Stores the position data of the pixel to a new outer vector of labeledPixels
			std::vector<Point2D> tempVect;
			Point2D tempPoint(x,y);
			tempVect.push_back(tempPoint);
			this->labeledPixels.push_back(tempVect);
			// Stores the region size as a new entry to regionSizes
			this->regionSizes.push_back(1);
		}

		// Compare labeling with the adjacent pixels (left-up, left, up, right-up)
		for (int k = 0; k < 4; k++)
		{
			// Coordinates of the adjacent pixel that is being compared to the current pixel
			int adjX, adjY;

			switch (k)
			{
			case 0: adjX = x-1;
				adjY = y-1;
				break;
			case 1: adjX = x-1;
				adjY = y;
				break;
			case 2: adjX = x;
				adjY = y-1;
				break;
			case 3: adjX = x+1;
				adjY = y-1;
				break;
			default: break;
			}

			if (adjX < 0 || adjY < 0)
			{
				continue;
			}

			// If there is a skin pixel at the adjacent position
			if ((*img)(adjX,adjY,0,0))
			{

				unsigned int adjacentLabel = this->labelMask(adjX,adjY,0,0);
				unsigned int ownLabel = this->labelMask(x,y,0,0);

				// Compares the two labels and if they are not equal determines which one is smaller in number
				// Then the data of the bigger Label will be transferred to the smaller Label
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

					// Retrieves all the coordinates of the pixels of the bigger label
					std::vector<Point2D> *positionVect = &(this->labeledPixels.at(biggerLabel));

					// Changes the labels of the pixels of the bigger label to the new labels on the temporary label mask
					for (unsigned int i = 0; i < positionVect->size();i++)
					{
						Point2D *posPoint = &(positionVect->at(i));

						this->labelMask(posPoint->x,posPoint->y,0,0) = smallerLabel;							
					}

					// Adjusts the new region sizes
					this->regionSizes.at(smallerLabel) += this->regionSizes.at(biggerLabel);
					this->regionSizes.at(biggerLabel) = 0;

					// Transfers the pixel coordinates of the bigger label to the vector of the smaller label
					for(unsigned int i = 0; i < positionVect->size();i++)
					{
						this->labeledPixels.at(smallerLabel).push_back(positionVect->at(i));
					}

					this->labeledPixels.at(biggerLabel).clear();

					// Determines if the smaller Label is now (after the fusion) the currently biggest region in the bit mask
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
		// Goes through the labelPixels vector which stores for each label an inner vector with all the coordinates of the pixels belonging to the label.
		for (unsigned int i = 0; i < this->labeledPixels.size();i++)
		{
			// Continues if this region is the biggest region or completely empty
			if (this->biggestRegion == i || this->regionSizes.at(i) == 0)
			{
				continue;
			}

			// Otherwise the coordinate informations are used to set all pixels in the bit mask to false
			for (unsigned int j = 0; j < this->labeledPixels.at(i).size();j++)
			{
				Point2D *posPoint = &this->labeledPixels.at(i).at(j);

				(*img)(posPoint->x,posPoint->y,0,0) = false;
			}
		}
	}

	template<typename T>
	std::vector<BinarySeed>* lime::Algorithm<T>::getSeeds( bool skin, bool singleRegion, const CImg<bool> &mask, bool applyRegionChange, unsigned int regionChangeCount, unsigned int regionChangeSize )
	{
		std::vector<BinarySeed> *resVector = new std::vector<BinarySeed>;
		CImg<bool> maskCopy(mask);

		// Pre-Processing of the data of the mask (grow or shrink algorithm)
		if (applyRegionChange)
		{
			if (skin)
			{
				this->shrinkAlgorithm(&maskCopy, regionChangeCount,regionChangeSize);
			}
			else
			{
				this->growAlgorithm(&maskCopy, regionChangeCount,regionChangeSize);
			}
		}

		//Processing part

		CImg<bool> visitedMask(maskCopy.width(),maskCopy.height(),1,1, false); // tracks which pixels were visited if the singleRegion algorithm is used
		std::queue<Point2D> pixelQueue; // Queue of the pixels that should be evaluated next if the singleRegion algorithm is used

		unsigned int width = maskCopy.width();
		unsigned int height = maskCopy.height();

		bool initPixel = false; // Only important if the singleRegion algorithm is used. True when a first suitable pixel has been detected

		// Loops through all pixels (if singleRegion isn't used) or just until the first suitable pixel has been detected (singleRegion algorithm)
		CImg_3x3(I, bool);
		cimg_for3x3(maskCopy, x, y, 0, 0, I, bool) 
		{
			if (initPixel && singleRegion)
			{
				// If the first suitable pixel has been detected (singleRegion algorithm) the loop will be canceled
				goto singleRegionJump;
			}

			if (skin)
			{
				// True if the center pixel is a skin pixel and one of the neighbor pixels is non-skin
				if (Icc && !(Ipp & Icp & Inp & Ipc & Inc & Ipn & Icn & Inn))
				{
					BinarySeed seed(x,y,true);
					resVector->push_back(seed);

					if (singleRegion)
					{
						pixelQueue.push(Point2D(x,y));
						visitedMask(x,y,0,0) = true;
						initPixel = true;
					}
				}
			}
			else
			{
				// True if the center pixel is a non-skin pixel and one of the neighbor pixels is skin
				if (!Icc && (Ipp | Icp | Inp | Ipc | Inc | Ipn | Icn | Inn))
				{
					BinarySeed seed(x,y,true);
					resVector->push_back(seed);

					if (singleRegion)
					{
						pixelQueue.push(Point2D(x,y));
						visitedMask(x,y,0,0) = true;
						initPixel = true;
					}
				}
			}
		}

		// for the single Region a search over the neighbor pixels is performed
singleRegionJump: 
		if (singleRegion && initPixel)
		{
			Point2D point;
			unsigned int x;
			unsigned int y;

			// Loops while there is still a pixel in the queue that has to be evaluated
			while (!pixelQueue.empty())
			{
				point = pixelQueue.front();
				pixelQueue.pop();

				x = point.x;
				y = point.y;

				// Pixel has been visited
				bool test1 = visitedMask(x,y,0,0);

				// 8-neighborhood pixels
				bool Ipp = maskCopy.atXY(x-1,y-1,0,0);
				bool Icp = maskCopy.atXY(x,y-1,0,0);
				bool Inp = maskCopy.atXY(x+1,y-1,0,0);
				bool Ipc = maskCopy.atXY(x-1,y,0,0);
				bool Icc = maskCopy(x,y,0,0);
				bool Inc = maskCopy.atXY(x+1,y,0,0);
				bool Ipn = maskCopy.atXY(x-1,y+1,0,0);
				bool Icn = maskCopy.atXY(x,y+1,0,0);
				bool Inn = maskCopy.atXY(x+1,y+1,0,0);

				bool neighborsBool[8] = {Ipp, Icp, Inp, Ipc, Inc, Ipn, Icn, Inn};
				Point2D neighborCoordinates[8] = {Point2D(x-1,y-1), Point2D(x,y-1), Point2D(x+1,y-1), Point2D(x-1,y), Point2D(x+1,y), Point2D(x-1,y+1), Point2D(x,y+1), Point2D(x+1,y+1)};

				// Determines if the current pixel is a seed pixel (skin or non-skin) or not
				bool suitablePixel = false;

				// If the current pixel is the very first one skip this part
				if (!initPixel)
				{
					// Decide if the Pixel is a border / seed pixel
					if (skin)
					{
						// True if the center pixel is a skin pixel and one of the neighbor pixels is non-skin
						if (Icc && !(Ipp & Icp & Inp & Ipc & Inc & Ipn & Icn & Inn))
						{
							BinarySeed seed(x,y,true);
							resVector->push_back(seed);
							suitablePixel = true;
						}
					}
					else
					{
						// True if the center pixel is a non-skin pixel and one of the neighbor pixels is skin
						if (!Icc && (Ipp | Icp | Inp | Ipc | Inc | Ipn | Icn | Inn))
						{
							BinarySeed seed(x,y,false);
							resVector->push_back(seed);
							suitablePixel = true;
						}
					}
				}
				else
				{
					initPixel = false;
					suitablePixel = true;
				}

				// If the current Pixel is a seed pixel add all surrounding seed pixel candidates to the Queue
				if (suitablePixel)
				{
					for (int i = 0; i < 8; i++)
					{
						unsigned int tempX = neighborCoordinates[i].x;
						unsigned int tempY = neighborCoordinates[i].y;

						// If the coordinates are outside of the image skip
						if (tempX < 0 || tempY < 0 || tempX >= width || tempY >= height)
						{
							continue;
						}

						// If the neighbor pixel is the same as the skin bool and not visited yet add it to the Queue
						if (!visitedMask(tempX,tempY,0,0))
						{
							bool test1 = (neighborsBool[i] == skin);
							if (neighborsBool[i] == skin)
							{
								visitedMask(tempX,tempY,0,0) = true;
								pixelQueue.push(neighborCoordinates[i]);
							}
						}
					}
				}
			}
		}


		return resVector;
	}

	template<typename T>
	CImg<int>* lime::Algorithm<T>::getDistanceMapOfMask( CImg<bool> &mask, bool singleRegion )
	{

		CImg<bool> maskCopy(mask);

		// Phase 1: Sorting the pixels into three different categories (seed, internal, external)
		std::vector<BinarySeed> seeds;

		std::vector<Point2D> internal;

		std::vector<Point2D> external;

		// Deletes all minor regions if just a single region should be detected
		if (singleRegion)
		{
			deleteMinorRegions(&maskCopy);
		}

		unsigned int width = maskCopy.width();
		unsigned int height = maskCopy.height();

		// Loops through all pixels
		CImg_3x3(I, bool);
		cimg_for3x3(maskCopy, x, y, 0, 0, I, bool) 
		{

			if (Icc)
			{
				// True if the center pixel is a skin pixel and one of the neighbor pixels is non-skin
				if (!(Ipp & Icp & Inp & Ipc & Inc & Ipn & Icn & Inn))
				{
					BinarySeed seed(x,y,true);
					seeds.push_back(seed);
				} 
				else // Center pixel and all surrounding pixels are skin
				{
					internal.push_back(Point2D(x,y));
				}
			}
			else // Center pixel is a non-skin pixel
			{
				external.push_back(Point2D(x,y));
			}
		}

		// Phase 2: Preparation for the FLANN search algorithm

		unsigned int numb_seeds = seeds.size();
		unsigned int numb_int = internal.size();
		unsigned int numb_ext = external.size();

		cv::Mat_<double> contourPoints(numb_seeds,2);
		cv::Mat_<double> internalPoints(numb_int,2);
		cv::Mat_<double> externalPoints(numb_ext,2);

		// Matrices getting filled

		for (unsigned int i = 0; i < seeds.size();i++)
		{
			contourPoints[i][0] = seeds[i].x;
			contourPoints[i][1] = seeds[i].y;
		}

		for (unsigned int i = 0; i < internal.size();i++)
		{
			internalPoints[i][0] = internal[i].x;
			internalPoints[i][1] = internal[i].y;
		}

		for (unsigned int i = 0; i < external.size();i++)
		{
			externalPoints[i][0] = external[i].x;
 			externalPoints[i][1] = external[i].y;
		}

		int a = 5;
		// Initialization of the search tree

		SearchTree st(contourPoints,cvflann::KDTreeIndexParams(5));

		// Phase 3: Retrieving the distances

		cv::Mat_<double> internalDistances = this->findDistances(st,internalPoints);
		cv::Mat_<double> externalDistances = this->findDistances(st, externalPoints);

		// Phase 4: Construction of the distance Map

		CImg<int> *map = new CImg<int>(maskCopy.width(),maskCopy.height(),1,1,(int)0);

		for (unsigned int i = 0; i < internal.size();i++)
		{
			unsigned int x = internal[i].x;
			unsigned int y = internal[i].y;

			int dist = static_cast<unsigned int>(*internalDistances[i]) * (-1);

			(*map)(x,y,0,0) = dist;
		}

		for (unsigned int i = 0; i < external.size();i++)
		{
			unsigned int x = external[i].x;
			unsigned int y = external[i].y;

			int dist = *externalDistances[i];

			(*map)(x,y,0,0) = dist;
		}

		return map;
	}
	template<typename T>
	cv::Mat_<double> lime::Algorithm<T>::findDistances(SearchTree& st, const cv::Mat_<double>& queryPoints )
	{
		// what we need for K nearest points search ( here K=1 )
		cv::Mat_<int> nearestIndices( queryPoints.rows, 1 ); // 1D matrix to store the indices of the K-nearest points
		cv::Mat_<double> nearestDistances( queryPoints.rows, 1 ); // 1D matrix to store the distances to the K-nearest points

		// run the search
		st.knnSearch(queryPoints,nearestIndices,nearestDistances,1,cvflann::SearchParams(128));

		return nearestDistances;

	}

}