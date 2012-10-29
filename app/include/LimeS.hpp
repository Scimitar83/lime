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

#pragma once

#include <cstdint>
#include <memory>


#include <QDialog>
#include <QMainWindow>



namespace Ui {
    class LimeS;
}


class LimeS : public QMainWindow
{
    Q_OBJECT

public:
    explicit LimeS(QWidget *parent = 0);
    ~LimeS();

protected:
    // ui
    Ui::LimeS *ui;
};

