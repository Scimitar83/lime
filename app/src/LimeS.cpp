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

#include <lime/util.hpp>

#include "ui_LimeS.h"
#include <LimeS.hpp>

LimeS::LimeS(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::LimeS)
{
    ui->setupUi(this);

    connect( ui->load_image, SIGNAL(clicked(bool)), this, SLOT(on_loadImage(void)) );
}


LimeS::~LimeS()
{
    delete ui;
}


void LimeS::on_loadImage()
{
    try
    {
        QString imagePath = QFileDialog::getOpenFileName(this, "Load Image", ".", "Images (*.bmp *.png *.xpm *.jpg *.tif *.tiff)");

        if( imagePath.size() > 0 )
        {
            // load image
            cimg_library::CImg<uint8_t> image;
            image.load( imagePath.toStdString().c_str() );

            // convert image to Qt
            QImage imageQt( image.width(), image.height(), QImage::Format_RGB888 );
            for( int y=0; y<image.height(); y++ )
            {
                for( int x=0; x<image.width(); x++ )
                {
                    QColor col( image(x,y,0,0),
                                image(x,y,0,1),
                                image(x,y,0,2) );
                    imageQt.setPixel( x, y, col.rgb() );
                }
            }

            // set the images
            ui->view->setAxisBackground(QPixmap::fromImage(imageQt), true, Qt::IgnoreAspectRatio );
            ui->view->xAxis->setRange(0, imageQt.width() );
            ui->view->yAxis->setRange(0, imageQt.height() );
            ui->view->replot();
        }

    }
    catch( std::exception &e )
    {
        ui->statusBar->showMessage( QString( e.what() ), 5000 );
        std::cerr << e.what() << std::endl;
        QMessageBox::critical(this, "Error", QString( e.what() ) );
    }
}
