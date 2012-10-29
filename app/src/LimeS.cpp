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


#include <iostream>
#include <stdexcept>
#include <cmath>

#include <QFileDialog>
#include <QFileInfo>
#include <QMessageBox>
#include <QPainter>
#include <QPixmap>
#include <QProgressDialog>
#include <QProgressBar>
#include <QStringList>
#include <QGraphicsPixmapItem>

#include "ui_LimeS.h"
#include <LimeS.hpp>

LimeS::LimeS(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::LimeS)
{
    ui->setupUi(this);
}


LimeS::~LimeS()
{
    delete ui;
}
