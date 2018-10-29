#pragma once
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>

//Common
#include "global.h"
#include "debug.h"
#include "v.h"

#include "skopy.h"


#define SPACEBAR 32
#define ESCAPE 27
#define QUIT 1U << 1
#define PAUSE 1U << 2
#define NEXTACTION 1U << 3



void draw_v2f64 
(
	cv::Mat & image, 
	uint32_t n, 
	struct v2f64 p [], 
	cv::Scalar color
)
{
	char buf [100];
	for (uint32_t i = 0; i < n; ++ i)
	{
		cv::Point a (p [i].x, p [i].y);
		snprintf (buf, 100, "%lu", (long unsigned)i);
		cv::circle (image, a, 2, color, -1);
		cv::putText (image, buf, a, CV_FONT_HERSHEY_SIMPLEX, 1, color);
	}
}


void draw_v2f64_track
(
	cv::Mat & image, 
	uint32_t n, 
	struct v2f64 p [], 
	uint32_t t [], 
	cv::Scalar color
)
{
	for (uint32_t i = 0; i < n; ++ i)
	{
		if (t [i] == UINT32_MAX) {continue;}
		struct v2f64 * a = p + i;
		struct v2f64 * b = p + t [i];
		cv::Point cva = cv::Point (a->x, a->y);
		cv::Point cvb = cv::Point (b->x, b->y);
		cv::line (image, cva, cvb, color);
	}
}


void draw_v2f64_star
(
	cv::Mat & image, 
	struct v2f64 o [1],
	uint32_t n, 
	struct v2f64 p [],
	double w [],
	cv::Scalar color
)
{
	
	for (uint32_t i = 0; i < n; ++ i)
	{
		cv::Point cva = cv::Point (o->x, o->y);
		cv::Point cvb = cv::Point (p [i].x, p [i].y);
		cv::line (image, cva, cvb, color);
		
		char buf [100];
		//double d [2];
		//vf64_sub (2, d, o->v, p [i].v);
		//snprintf (buf, 100, "%lf", atan2 (d [1], d [0]) * (180/CV_PI));
		snprintf (buf, 100, "%i", (int)w [i]);
		cv::putText (image, buf, cvb + cv::Point (50, 0), CV_FONT_HERSHEY_SIMPLEX, 1, color);
	}
}


void get_flags (uint32_t * flags)
{
	(*flags) &= ~NEXTACTION;
	int c = cv::waitKey (3);
	if (c == 'q' || c == 'Q' || (c & 255) == ESCAPE) 
	{
		(*flags) |= QUIT;
	}
	if (c == SPACEBAR) 
	{
		(*flags) |= NEXTACTION;
	}
}
