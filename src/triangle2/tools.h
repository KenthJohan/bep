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





void draw_points (cv::Mat & image, uint32_t n, struct v2f64 p [], cv::Scalar c)
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
	return cv::Scalar (0, 0, 0);
}

/*
void show (char const * name, cv::Mat const & image)
{
	cv::Mat dst;
	//cv::resize (image, dst, cv::Size (1920, 1080));
	cv::imshow (name, dst);
}
*/
/*
void translate (cv::Point &a)
{
	a.x += 200;
	a.y += 100;
	a.x *= 4;
	a.y *= 4;
}
*/

void draw_points_index 
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
		cv::circle (image, a, 2, class_color (c [i]), -1);
		snprintf (buf, 100, "%lu", (long unsigned)i);
		cv::putText (image, buf, a, CV_FONT_HERSHEY_SIMPLEX, 1, class_color (c [i]));
	}
}


void draw_tracks 
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
		cv::line (image, cva, cvb, c);
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






