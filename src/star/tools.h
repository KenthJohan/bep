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


#define ACTION_QUIT 1U << 1
#define ACTION_PAUSE 1U << 2
#define ACTION_NEXTTOY 1U << 4
#define ACTION_SIMPLIFY0 1U << 5
#define ACTION_SIMPLIFY1 1U << 6
#define ACTION_RESET 1U << 7

#define ACTION_MOVE_LEFT 1U << 8
#define ACTION_MOVE_RIGHT 1U << 9
#define ACTION_MOVE_DOWN 1U << 10
#define ACTION_MOVE_UP 1U << 11
#define ACTION_MOVE_ZOOMIN 1U << 12
#define ACTION_MOVE_ZOOMOUT 1U << 13



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
		
		//If entangled
		if (abs (w [i]) > 180.0)
		{
			cv::circle (image, cvb, 10, cv::Scalar (0,200,0), 3);
		}
		
		char buf [100];
		//double d [2];
		//vf64_sub (2, d, o->v, p [i].v);
		//snprintf (buf, 100, "%lf", atan2 (d [1], d [0]) * (180/CV_PI));
		snprintf (buf, 100, "%i", (int)w [i]);
		cv::putText (image, buf, cvb + cv::Point (50, 0), CV_FONT_HERSHEY_SIMPLEX, 0.7, cv::Scalar (200,200,0));
	}
}


void get_flags (uint32_t * flags)
{
	(*flags) &= ~ACTION_NEXTTOY;
	(*flags) &= ~ACTION_SIMPLIFY0;
	(*flags) &= ~ACTION_SIMPLIFY1;
	(*flags) &= ~ACTION_MOVE_UP;
	(*flags) &= ~ACTION_MOVE_DOWN;
	(*flags) &= ~ACTION_MOVE_LEFT;
	(*flags) &= ~ACTION_MOVE_RIGHT;
	(*flags) &= ~ACTION_MOVE_ZOOMIN;
	(*flags) &= ~ACTION_MOVE_ZOOMOUT;
	int c = cv::waitKey (3);
	if (c == 'q' || c == 'Q' || (c & 255) == ESCAPE) 
	{
		(*flags) |= ACTION_QUIT;
	}
	if (c == SPACEBAR) 
	{
		(*flags) ^= ACTION_PAUSE;
	}
	if (c == '0') 
	{
		(*flags) |= ACTION_RESET;
	}
	if (c == '1') 
	{
		(*flags) |= ACTION_NEXTTOY;
	}
	if (c == '2') 
	{
		(*flags) |= ACTION_SIMPLIFY0;
	}
	if (c == '3') 
	{
		(*flags) |= ACTION_SIMPLIFY1;
	}
	
	if (c == 'w') 
	{
		(*flags) |= ACTION_MOVE_UP;
	}
	if (c == 's') 
	{
		(*flags) |= ACTION_MOVE_DOWN;
	}
	if (c == 'a') 
	{
		(*flags) |= ACTION_MOVE_LEFT;
	}
	if (c == 'd') 
	{
		(*flags) |= ACTION_MOVE_RIGHT;
	}
	if (c == '+') 
	{
		(*flags) |= ACTION_MOVE_ZOOMIN;
	}
	if (c == '-') 
	{
		(*flags) |= ACTION_MOVE_ZOOMOUT;
	}
}




void move 
(
	uint32_t n, 
	struct v2f64 des [],
	struct v2f64 src [],
	double t [4]
)
{
	for (uint32_t i = 0; i < n; ++ i)
	{
		des [i].x = (t [0] + src [i].x) * exp (t [2]);
		des [i].y = (t [1] + src [i].y) * exp (t [3]);
	}
}
