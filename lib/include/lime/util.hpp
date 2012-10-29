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
/// \file    util.hpp
///
/// \package lime
/// \version 0.1.0
///
/// \brief   collection of utility functions
///
/// \details this file contains a collection of small utility functions,
///          to simplify development of the lime library
///
/// \author  Alexandru Duliu
/// \date    Oct 29, 2012
///

#include <cmath>
#include <memory>
#include <vector>
#include <stdint.h>

#include <Eigen/Core>
#include <Eigen/Geometry>

#define cimg_display 0
#include <CImg.h>

namespace lime
{

template <typename T>
inline void loadImage( const std::string& filename, cimg_library::CImg<T>& image )
{
    image.load( filename.c_str() );
}

} // end namespace lime


