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
#define ACTION_RENDER 1U << 3
#define ACTION_NEXTTOY 1U << 4
#define ACTION_NEXTTREE 1U << 5
#define ACTION_SIMPLIFY1 1U << 6
#define ACTION_RESET 1U << 7

#define ACTION_MOVE_LEFT 1U << 8
#define ACTION_MOVE_RIGHT 1U << 9
#define ACTION_MOVE_DOWN 1U << 10
#define ACTION_MOVE_UP 1U << 11
#define ACTION_MOVE_ZOOMIN 1U << 12
#define ACTION_MOVE_ZOOMOUT 1U << 13





void draw_points (cv::Mat & image, uint32_t n, struct v2f64 p [], int radius, cv::Scalar c)
{
	for (uint32_t i = 0; i < n; ++ i)
	{
		cv::Point a (p [i].x, p [i].y);
		cv::circle (image, a, radius, c, -1);
	}
}


void draw_points_index 
(
	cv::Mat & image, 
	uint32_t n, 
	struct v2f64 p [], 
	int radius, 
	cv::Scalar c
)
{
	char buf [100];
	for (uint32_t i = 0; i < n; ++ i)
	{
		cv::Point a (p [i].x, p [i].y);
		cv::circle (image, a, radius, c, -1);
		snprintf (buf, 100, "%lu", (long unsigned)i);
		cv::putText (image, buf, a, CV_FONT_HERSHEY_SIMPLEX, 1, c);
	}
}


void draw_tracks 
(
	cv::Mat & image, 
	uint32_t np, 
	struct v2f64 p [], 
	uint32_t nt, 
	uint32_t t [], 
	cv::Scalar c
)
{
	for (uint32_t i = 0; i < nt-1; ++ i)
	{
		uint32_t ia = t [i];
		uint32_t ib = t [i + 1];
		if (ia >= np) {break;}
		if (ib >= np) {break;}
		struct v2f64 * a = p + ia;
		struct v2f64 * b = p + ib;
		cv::Point cva = cv::Point (a->x, a->y);
		cv::Point cvb = cv::Point (b->x, b->y);
		cv::line (image, cva, cvb, c);
	}
}


void get_flags (uint32_t * flags)
{
	(*flags) &= ~ACTION_NEXTTOY;
	(*flags) &= ~ACTION_NEXTTREE;
	(*flags) &= ~ACTION_SIMPLIFY1;
	(*flags) &= ~ACTION_MOVE_UP;
	(*flags) &= ~ACTION_MOVE_DOWN;
	(*flags) &= ~ACTION_MOVE_LEFT;
	(*flags) &= ~ACTION_MOVE_RIGHT;
	(*flags) &= ~ACTION_MOVE_ZOOMIN;
	(*flags) &= ~ACTION_MOVE_ZOOMOUT;
	int c = cv::waitKey (3);
	//TRACE_F ("%i", c);
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
		(*flags) |= ACTION_NEXTTREE;
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
		des [i].x = t [0] + (src [i].x) * exp (t [2]);
		des [i].y = t [1] + (src [i].y) * exp (t [3]);
	}
}








struct drawdata
{
	struct v2f64 * tree;
	struct v2f64 * toy;
};


void drawdata_init (struct drawdata * r, struct skopypark * park)
{
	r->toy = (struct v2f64 *) malloc (sizeof (struct v2f64) * park->ntoy);
	r->tree = (struct v2f64 *) malloc (sizeof (struct v2f64) * park->ntree);
}


void drawdata_update (struct drawdata * r, struct skopypark * park, double transform [4])
{
	move (park->ntoy, r->toy, park->toy, transform);
	move (park->ntree, r->tree, park->tree, transform);
	
	cv::Mat image (1000, 1000, CV_8UC3);
	image.setTo (cv::Scalar (255,255,255));
	struct v2f64 o [2] = {0.0, 0.0};
	struct v2f64 dog [2] = {0.0, 0.0};
	move (1, o, o, transform);
	move (1, dog, park->toy + park->idog, transform);
	if (park->itree < park->ntree)
	{
		struct v2f64 tree [2] = {0.0, 0.0};
		move (1, tree, park->tree + park->itree, transform);
		draw_points (image, 1, tree, 10, cv::Scalar (180, 120, 255));
	}
	draw_points (image, 1, o, 2, cv::Scalar (0, 0, 255));
	draw_points (image, 1, dog, 10, cv::Scalar (180, 120, 155));
	draw_points_index (image, park->ntoy, r->toy, 2, cv::Scalar (0, 0, 255));
	draw_points_index (image, park->ntree, r->tree, 2, cv::Scalar (0, 0, 255));
	//draw_tracks (image, park->ntoy,);
	cv::imshow ("test", image);
}




