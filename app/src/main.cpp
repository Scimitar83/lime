////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of LimeS, a C++ UI image segmentation                    //
//                                                                            //
// Copyright (C) 2012 Alexandru Duliu                                         //
//                                                                            //
// LimeS is free software; you can redistribute it and/or                     //
// modify it under the terms of the GNU  General Public License               //
// as published by the Free Software Foundation; either version 3             //
// of the License, or (at your option) any later version.                     //
//                                                                            //
// LimeS is distributed in the hope that it will be useful,                   //
// but WITHOUT ANY WARRANTY; without even the implied warranty of             //
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the              //
// GNU General Public License for more details.                               //
//                                                                            //
// You should have received a copy of the GNU General Public License          //
// along with LimeS. If not, see <http://www.gnu.org/licenses/>.              //
//                                                                            //
///////////////////////////////////////////////////////////////////////////////

#include <QtGui/QApplication>
#include <LimeS.hpp>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    LimeS w;
    w.show();

    return a.exec();
}
