#pragma once

#include <lime/Algorithm.hpp>

#ifndef M_PI
#define M_PI 3.14159265358979323846f
#endif

static const double threshold_H_lower1 = 1.0;
static const double threshold_H_higher1 = 28.0;
static const double threshold_H_lower2 = 332.0;
static const double threshold_H_higher2 = 360.0;
static const double threshold_H_lower3 = 309.0;
static const double threshold_H_higher3 = 331.0;
static const double threshold_I = 0.4;
static const double threshold_S_lower = 13.0/255.0;
static const double threshold_S_higher1 = 110.0/255.0;
static const double threshold_S_higher2 = 75.0/255.0;

namespace lime{

	template<typename T> class ColorimetricHSIAlgorithm1: public Algorithm<T>{

	public:

		ColorimetricHSIAlgorithm1(float _driftValue = -1.0):Algorithm<T>(_driftValue,true){}
		virtual ~ColorimetricHSIAlgorithm1(){}

	protected:

		// virtual functions
		virtual CImg<T>* processImage( const CImg<T> &img );
		virtual void processImage( CImg<T> *img );
		
		virtual CImg<double>* transformRGBToHSI(const CImg<T> &img);
		virtual bool skinThresholds(double h, double s, double i);

		// Thresholds
		
	};

}


template<typename T>
CImg<T>* lime::ColorimetricHSIAlgorithm1<T>::processImage( const CImg<T> &img )
{
	CImg<T> tmpImg(img);

	this->processImage(&tmpImg);

	return new CImg<T>(tmpImg);
}

template<typename T>
void lime::ColorimetricHSIAlgorithm1<T>::processImage( CImg<T> *img )
{

	int _width = img->width();
	int _height = img->height();

	// Applying a median filter
	//const CImg<T> medianImg = img->get_blur_median(5);
	const CImg<T> medianImg(*img);

	// Changing img to accept the new data with only 1 (binary) channel, in-place
	img->assign(_width,_height,1,1);

	CImg<double> *hsiImg = transformRGBToHSI(medianImg);

	hsiImg->save("test2.bmp");

	*hsiImg = medianImg.get_RGBtoHSI();

	double h,s,i;

	for (int y = 0; y < _height; y++)
	{
		for (int x = 0; x < _width; x++)
		{
			h = (*hsiImg)(x,y,0,0);
			s = (*hsiImg)(x,y,0,1);
			i = (*hsiImg)(x,y,0,2);


			if (skinThresholds(h,s,i))
			{
				(*img)(x,y,0,0) = 1;
			}
			else
			{
				(*img)(x,y,0,0) = 0;
			}
		}
	}

	// some cleanup
	delete hsiImg;
}

template<typename T>
CImg<double>* lime::ColorimetricHSIAlgorithm1<T>::transformRGBToHSI( const CImg<T> &img )
{
	int _width = img.width();
	int _height = img.height();

	double r,g,b;

	double i1,i2,i3,h,s,i;

	double pi = M_PI;

	CImg<double> *resImg = new CImg<double>(_width,_height,1,3);

	for (int y = 0; y < _height;y++)
	{
		for (int x = 0; x < _width; x++)
		{
			r = img(x,y,0,0);
			g = img(x,y,0,1);
			b = img(x,y,0,2);

			i = i1 = (r+g+b)/3.0;
			i2 = (r-b)/2.0;
			i3 = (2.0*g - r -b)/4.0;

			s = std::sqrt(i2*i2 + i3*i3);

			h = std::atan(i3/i2) * 360/pi + 180; // transformation from radians to degrees in  [0,360]

			(*resImg)(x,y,0,0) = h;
			(*resImg)(x,y,0,1) = s;
			(*resImg)(x,y,0,2) = i;
		}
	}

	return resImg;
}

template<typename T>
bool lime::ColorimetricHSIAlgorithm1<T>::skinThresholds( double h, double s, double i )
{
	if (i < threshold_I)
	{
		return false;
	}

	if (threshold_S_lower && s < threshold_S_higher2)
	{
		return (h > threshold_H_lower3 && h < threshold_H_higher3);
	}

	if (s > threshold_S_lower && s < threshold_S_higher1)
	{
		return ((threshold_H_lower1 < h && h < threshold_H_higher1) || (threshold_H_lower2 < h && h < threshold_H_higher2));
	}

	return false;
}