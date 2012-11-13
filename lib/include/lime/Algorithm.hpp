#pragma once

#ifndef _USE_MATH_DEFINES
#define _USE_MATH_DEFINES
#endif

#include <lime/util.hpp>
#include <CImg.h>
#include <cmath>

using namespace cimg_library;



namespace lime{

	// Forward-declaration
	template<typename U> class Segmentation;

	template<typename T = int> class Algorithm{

		friend class Segmentation<T>;

	public:
		Algorithm(float _driftValue = -1.0, bool _driftBool = false):driftValue(_driftValue),driftBool(_driftBool){}
		virtual ~Algorithm(){}

	protected:
		
		// Virtual functions
		
		virtual CImg<T>* processImage(const CImg<T> &img) = 0;
		virtual void processImage(CImg<T> *img) = 0;
		
		//Inline functions
		virtual inline const float getDrift() const{return driftValue;}
		virtual inline void changeDrift(const float drift){driftValue = drift;}
		virtual inline bool hasDrift(){return driftBool;}

		// Members
		float driftValue;
		bool driftBool;

	};

}