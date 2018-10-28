#pragma once

#include <math.h>
#include "global.h"
#include "debug.h"
#include "v.h"


#define FILTER1 1U << 4
#define FILTER2 1U << 5
#define FILTER3 1U << 6
#define FILTER4 1U << 7
#define FILTER0 1U << 8

#define TOY 0
#define TREE 1
#define DOG 2



void shortest 
(
	uint32_t n, 
	
	//Pos
	struct v2f64 p [], 
	
	//Track
	uint32_t t [],
	
	//Start point index
	uint32_t o
)
{
	typedef long unsigned lu;
	//TRACE_F ("FIND NEXT FROM %lu\n", (lu)o);
	double lmin = DBL_MAX;
	uint32_t imin = UINT32_MAX;
	double d [2];
	for (uint32_t i = 0; i < n; ++ i)
	{
		if (o == i) {continue;}
		if (t [i] != UINT32_MAX) {continue;}
		double * a = p [o].v;
		double * b = p [i].v;
		vf64_sub (2, d, a, b);
		double l = vf64_dot (2, d, d);
		//TRACE_F ("%10lu %4lu %4lu %7.3f\n", t [i], o, i, l);
		if (l < lmin)
		{
			lmin = l;
			imin = i;
		}
	}
	//printf ("SET %lu %lu\n", (long unsigned) o, (long unsigned) imin);
	if (imin != UINT32_MAX)
	{
		t [o] = imin;
	}
	
	//printf ("\n\n");
}


