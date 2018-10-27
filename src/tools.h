#pragma once
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>

#include "global.h"
#include "debug.h"
#include "skopy.h"
#include "v.h"


#define SPACEBAR 32
#define ESCAPE 27
#define QUIT 1U << 1
#define PAUSE 1U << 2


void paint_points (cv::Mat & image, uint32_t n, struct v2f64 p [], cv::Scalar c)
{
	for (uint32_t i = 0; i < n; ++ i)
	{
		cv::Point a (p [i].x, p [i].y);
		cv::circle (image, a, 5, c, -1);
	}
}


cv::Scalar class_color (uint32_t c)
{
	switch (c)
	{
		case TOY:
		return cv::Scalar (255, 100, 200);
		case TREE:
		return cv::Scalar (50, 200, 50);
		case DOG:
		return cv::Scalar (0, 100, 150);
	};
}


void show (char const * name, cv::Mat const & image)
{
	cv::Mat dst;
	//cv::resize (image, dst, cv::Size (1920, 1080));
	cv::imshow (name, dst);
}


void translate (cv::Point &a)
{
	//a.x += 250;
	//a.y += 250;
}


void paint_points_index 
(
	cv::Mat & image, 
	uint32_t n, 
	struct v2f64 p [], 
	uint32_t c []
)
{
	char buf [100];
	for (uint32_t i = 0; i < n; ++ i)
	{
		cv::Point a (p [i].x, p [i].y);
		translate (a);
		cv::circle (image, a, 2, class_color (c [i]), -1);
		snprintf (buf, 100, "%lu", (long unsigned)i);
		cv::putText (image, buf, a, CV_FONT_HERSHEY_SIMPLEX, 1, class_color (c [i]));
	}
}


void paint_tracks 
(
	cv::Mat & image, 
	uint32_t n, 
	struct v2f64 p [], 
	uint32_t t [], 
	cv::Scalar c
)
{
	for (uint32_t i = 0; i < n; ++ i)
	{
		if (t [i] == UINT32_MAX) {continue;}
		struct v2f64 * a = p + i;
		struct v2f64 * b = p + t [i];
		cv::Point cva = cv::Point (a->x, a->y);
		cv::Point cvb = cv::Point (b->x, b->y);
		translate (cva);
		translate (cvb);
		cv::line (image, cva, cvb, c);
	}
}


void get_flags (uint32_t * flags)
{
	(*flags) &= ~FILTER1;
	(*flags) &= ~FILTER2;
	(*flags) &= ~FILTER3;
	(*flags) &= ~FILTER4;
	int c = cv::waitKey (3);
	if (c == 'q' || c == 'Q' || (c & 255) == ESCAPE) 
	{
		(*flags) |= QUIT;
	}
	if (c == SPACEBAR) 
	{
		//(*flags) ^= PAUSE;
		//(*flags) |= NEXTACTION;
	}
	if (c == '0') 
	{
		(*flags) |= FILTER0;
	}
	if (c == '1') 
	{
		(*flags) |= FILTER1;
	}
	if (c == '2') 
	{
		(*flags) |= FILTER2;
	}
	if (c == '3') 
	{
		(*flags) |= FILTER3;
	}
	if (c == '4') 
	{
		(*flags) |= FILTER4;
	}
}
