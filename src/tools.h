#pragma once
#include <opencv2/bgsegm.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/features2d.hpp>
#include <opencv2/core/utility.hpp>
#include <opencv2/xfeatures2d.hpp>
#include "debug.h"


#define SPACEBAR 32
#define ESCAPE 27
#define QUIT 1U << 1
#define PAUSE 1U << 2

#define FILTER1 1U << 4
#define FILTER2 1U << 5
#define FILTER3 1U << 6
#define FILTER4 1U << 7

#define TOY 0
#define TREE 1
#define DOG 2

struct v2f64
{
	union
	{
		double v [2];
		struct
		{
			double x;
			double y;
		};
	};
};





double vf64_dot (uint32_t n, double a [], double b [])
{
	double psum = 0;
	for (uint32_t i = 0; i < n; ++ i)
	{
		psum += a [i] * b [i];
	}
	return psum;
}


void vf64_sub (uint32_t n, double r [], double a [], double b [])
{
	for (uint32_t i = 0; i < n; ++ i)
	{
		r [i] = a [i] - b [i];
	}
}


void vu32_set1 (uint32_t n, uint32_t r [], uint32_t a)
{
	for (uint32_t i = 0; i < n; ++ i)
	{
		r [i] = a;
	}
}


void v2f64_mul1 (uint32_t n, struct v2f64 r [], struct v2f64 a [], struct v2f64 * b)
{
	for (uint32_t i = 0; i < n; ++ i)
	{
		r [i].x = a [i].x * b [0].x;
		r [i].y = a [i].y * b [0].y;
	}
}


void v2f64_random (uint32_t n, struct v2f64 p [])
{
	for (uint32_t i = 0; i < n; ++ i)
	{
		p [i].x = (double)rand () / (double)RAND_MAX;
		p [i].y = (double)rand () / (double)RAND_MAX;
	}
}


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
		return cv::Scalar (100, 255, 100);
		case DOG:
		return cv::Scalar (0, 100, 150);
	};
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
		cv::circle (image, a, 5, class_color (c [i]), -1);
		snprintf (buf, 100, "%lu", (long unsigned)i);
		cv::putText (image, buf, a, CV_FONT_HERSHEY_SIMPLEX, 2, class_color (c [i]));
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
		cv::line (image, cv::Point (a->x, a->y), cv::Point (b->x, b->y), c);
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


int point_inside_triangle 
(
	struct v2f64 * p0,
	struct v2f64 * p1,
	struct v2f64 * p2,
	struct v2f64 * p
)
{
	double A = 1.0/2.0 * (-p1->y * p2->x + p0->y * (-p1->x + p2->x) + p0->x * (p1->y - p2->y) + p1->x * p2->y);
	double sign = A < 0.0 ? -1.0 : 1.0;
	double s = (p0->y * p2->x - p0->x * p2->y + (p2->y - p0->y) * p->x + (p0->x - p2->x) * p->y) * sign;
	double t = (p0->x * p1->y - p0->y * p1->x + (p0->y - p1->y) * p->x + (p1->x - p0->x) * p->y) * sign;
	return (s > 0.0) && (t > 0.0) && (s + t) < (2.0 * A * sign);
}


