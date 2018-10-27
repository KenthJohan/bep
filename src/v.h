#pragma once

#include <stdio.h>
#include <stdint.h>
#include <float.h>
#include "debug.h"
#include "global.h"

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


void vu32_print (uint32_t n, uint32_t x [])
{
	for (uint32_t i = 0; i < n; ++ i) 
	{
		printf ("%10lu\n", (long unsigned) x [i]);
	};
}


void vv2f64_print (uint32_t n, struct v2f64 x [])
{
	for (uint32_t i = 0; i < n; ++ i) 
	{
		printf ("%lf %lf\n", x [i].x, x [i].y);
	};
}


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


int v2f64_in_triangle 
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
