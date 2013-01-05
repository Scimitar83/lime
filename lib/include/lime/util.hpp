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
/// @file util.hpp
/// @author Alexandru Duliu, Alexander Schoch
/// @version 0.3.0
/// @date Oct 29, 2012 - First creation
/// @brief Collection of utility functions
/// @details This file contains a collection of small utility functions to simplify development of the lime library.
/// @package lime
///

#include <cmath>
#include <memory>
#include <vector>
#include <queue>
#include <stdint.h>
#include <exception>
#include <Eigen/Core>
#include <Eigen/Geometry>

#define cimg_display 0
#include <CImg.h>

namespace lime
{

	///
	/// @struct BinarySeed
	/// @brief This struct provides 2D coordinates of a seed point in cartesianspace together with a label if the seed is skin (label == true) or non-skin (label == false)
	///
	struct BinarySeed
	{
	public:

		BinarySeed(unsigned	int _x, unsigned int _y, bool _label):x(_x),y(_y),label(_label){}

		unsigned int x;
		unsigned int y;
		bool label;
	};

	///
	/// @struct Point2D
	/// @brief This struct provides 2D coordinates in Cartesian space.
	///
	struct Point2D
	{
	public:

		Point2D():x(0),y(0){}
		Point2D(unsigned int _x, unsigned int _y):x(_x),y(_y){}

		unsigned int x;
		unsigned int y;
	};

///
/// @brief This function can be used to load an image from a file into a CImg object using the filepath
/// @param filename The path of the image that should be loaded
/// @param image The CImg object that should store the new image data
/// @tparam T The data format of each image channel (e.g. char or double)
///
template <typename T>
inline void loadImage( const std::string& filename, cimg_library::CImg<T>& image )
{
    image.load( filename.c_str() );
}


///
/// @brief Changes the data representation of a CImg object from binary data (true,false) to the RGB range (0-255)
/// @param img The input image in binary format
/// @return The resulting image object in RGB format (3 channels)
///
inline cimg_library::CImg<int>* changeBinaryMaskToRGBImage(cimg_library::CImg<bool> &img){

	int _width = img.width();
	int _height = img.height();

	cimg_library::CImg<int>* resImg = new cimg_library::CImg<int>(_width,_height,1,3,0);

	for (int y = 0; y < _height; y++)
	{
		for (int x = 0; x < _width; x++)
		{
			if (img(x,y,0,0))
			{
				(*resImg)(x,y,0,0)=255;
				(*resImg)(x,y,0,1)=255;
				(*resImg)(x,y,0,2)=255;
			}
		}
	}

	return resImg;
}

///
/// @brief This function can be used to visualize skin and non-skin pixels by colouring the corresponding pixels in a rgb image (skin = red, non-skin = green)
/// @rgb A pointer to the rgb image
/// @skin A pointer to the vector with the skin pixels
/// @nonSkin A pointer to the vector with the non-skin pixels
///
inline void addSeedsToRGBImage(cimg_library::CImg<int> *rgb, std::vector<BinarySeed> *skin, std::vector<BinarySeed> *nonSkin)
{
	for (unsigned int i = 0; i < skin->size(); i++)
	{
		BinarySeed seed = skin->at(i);
		unsigned int x = seed.x;
		unsigned int y = seed.y;

		(*rgb)(x,y,0,0) = 255;
		(*rgb)(x,y,0,1) = 0;
		(*rgb)(x,y,0,2) = 0;
	}

	for (unsigned int i = 0; i < nonSkin->size(); i++)
	{
		BinarySeed seed = nonSkin->at(i);
		unsigned int x = seed.x;
		unsigned int y = seed.y;

		(*rgb)(x,y,0,0) = 0;
		(*rgb)(x,y,0,1) = 255;
		(*rgb)(x,y,0,2) = 0;
	}
}

inline cimg_library::CImg<unsigned char>* distanceMapToGreyscale(cimg_library::CImg<int> *map)
{
	cimg_library::CImg<unsigned char> *res = new cimg_library::CImg<unsigned char>(map->width(),map->height(),1,1, (unsigned char)255);

	int maximum = map->max();
	int minimum = map->min();

	cimg_forXY(*map,x,y){

		int val = (*map)(x,y,0,0);

		if ( val >= 0)
		{
			(*res)(x,y,0,0) = (val *127/maximum + 128); 
		}
		else
		{
			(*res)(x,y,0,0) = 128 - (val *128/minimum + 128); 
		}
	}

	return res;
}

inline cimg_library::CImg<unsigned char>* distanceMapToRGB(cimg_library::CImg<int> *map)
{
	cimg_library::CImg<unsigned char> *res = new cimg_library::CImg<unsigned char>(map->width(),map->height(),1,3);

	int maximum = std::log((double)std::abs(map->max()))/std::log(2.0);
	int minimum = std::log((double)std::abs(map->min()))/std::log(2.0);

	cimg_forXY(*map,x,y){

		int val = (*map)(x,y,0,0);

		if (val == 0)
		{
			(*res)(x,y,0,0) = 0;
			(*res)(x,y,0,1) = 0;
			(*res)(x,y,0,2) = 0;
			continue;
		}

		if ( val < 0)
		{
			val = std::log((double)std::abs(val))/std::log(2.0);

			(*res)(x,y,0,0) = 255 - ((val * 255)/minimum);
			(*res)(x,y,0,1) = 255;
			(*res)(x,y,0,2) = 255 - ((val * 255)/minimum);
		}
		else
		{
			val = std::log((double)std::abs(val))/std::log(2.0);

			(*res)(x,y,0,0) = 255;
			(*res)(x,y,0,1) = 255 - ((val * 255)/maximum);
			(*res)(x,y,0,2) = 255 - ((val * 255)/maximum);
		}
	}

	return res;
}

///
/// @brief Fuses two CImg objects together: A binary mask and a RGB image (3 channels)
/// @param img The RGB image that will afterwards have an additional channel (alpha channel)
/// @param mask The binary mask that should be added as alpha channel
/// @warning The dimensions of the mask and image have to match exactly. Otherwise an Exception is thrown.
/// @tparam T The desired data format of the input/output CImg (e.g. char or double)
///
template <typename T>
inline void fuseBinaryMaskWithRGBImage(cimg_library::CImg<T> *img, cimg_library::CImg<bool> *mask){

	int imgWidth = img->width();
	int imgHeight = img->height();

	if (imgWidth != mask->width() || imgHeight != mask->height())
	{
		throw std::exception("Dimensions of mask and image don't match!");
	}

	// Copy the image data over into a temporary container
	CImg<T> tempImg(*img);

	// Changes the image in-place to an image with 4 channels
	img->assign(imgWidth,imgHeight,1,4);

	for (int y = 0; y < imgHeight; y++)
	{
		for (int x = 0; x < imgWidth; x++)
		{
			(*img)(x,y,0,0) = tempImg(x,y,0,0);
			(*img)(x,y,0,1) = tempImg(x,y,0,1);
			(*img)(x,y,0,2) = tempImg(x,y,0,2);

			if ((*mask)(x,y,0,0))
			{
				(*img)(x,y,0,3) = 255;
			} 
			else
			{
				(*img)(x,y,0,3) = 0;
			}
		}
	}
}

} // end namespace lime


