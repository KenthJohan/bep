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


void track_print (uint32_t n, uint32_t x [])
{
	for (uint32_t i = 0; i < n; ++ i) 
	{
		if (x [i] == UINT32_MAX) {printf ("%3s", "-");}
		else {printf ("%3lu", (long unsigned) x [i]);}
	};
	printf ("\n");
}


void skopy_goto_newtoy 
(
	uint32_t n, 
	
	//Pos
	struct v2f64 p [], 
	
	//Track
	uint32_t t [],
	
	//Class
	uint32_t c [], 
	
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
		if (c [i] != TOY) {continue;}
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


//Simplifes the track and avoids trees
int skopy_triangle_simplify0 
(
	uint32_t n, 
	
	//Input
	//Point data
	struct v2f64 p [], 
	
	//Input Output
	//Track data
	uint32_t t [], 
	
	//Input
	//Class data
	uint32_t c [],
	
	//Input Output
	uint32_t * o
)
{
	typedef long unsigned lu;
	//Get 3 consecutive points from the track
	uint32_t ia = (*o);
	if (ia == UINT32_MAX) 
	{
		//printf ("ia == UINT32_MAX\n"); 
		return 1;
	}
	uint32_t ib = t [ia];
	if (ib == UINT32_MAX) 
	{
		//printf ("ib == UINT32_MAX\n");
		return 2;
	}
	uint32_t ic = t [ib];
	if (ic == UINT32_MAX) 
	{
		//printf ("ic == UINT32_MAX\n");
		return 3;
	}
	struct v2f64 * pa = p + ia;
	struct v2f64 * pb = p + ib;
	struct v2f64 * pc = p + ic;
	//printf ("(%lu %lu %lu)\n", (lu)ia, (lu)ib, (lu)ic);
	
	int r = 0;
	uint32_t i;
	for (i = 0; i < n; ++ i)
	{
		if ((*o) == i) {continue;}
		switch (c [i])
		{
			case TREE:
			r = v2f64_in_triangle (pa, pb, pc, p + i);
			break;
		}
		if (r)
		{
			(*o) = ib;
			break;
		}
	}
	if (!r)
	{
		t [ia] = ic;
		t [ib] = ib;
	}
	return 0;
}


int skopy_triangle_simplify1 
(
	uint32_t n, 
	
	//Input
	//Point data
	struct v2f64 p [], 
	
	//Input Output
	//Track data
	uint32_t t [], 
	
	//Input
	//Class data
	uint32_t c [],
	
	//Input Output
	uint32_t * o
)
{
	typedef long unsigned lu;
	//Get 3 consecutive points from the track
	uint32_t ia = (*o);
	if (ia == UINT32_MAX) 
	{
		//printf ("ia == UINT32_MAX\n"); 
		return 1;
	}
	uint32_t ib = t [ia];
	if (ib == UINT32_MAX) 
	{
		//printf ("ib == UINT32_MAX\n");
		return 2;
	}
	uint32_t ic = t [ib];
	if (ic == UINT32_MAX) 
	{
		//printf ("ic == UINT32_MAX\n");
		return 3;
	}
	struct v2f64 * pa = p + ia;
	struct v2f64 * pb = p + ib;
	struct v2f64 * pc = p + ic;
	//printf ("(%lu %lu %lu)\n", (lu)ia, (lu)ib, (lu)ic);
	
	
	int r = 0;
	uint32_t i;
	for (i = 0; i < n; ++ i)
	{
		//Ignore points that are the origin.
		if ((*o) == i) {continue;}
		
		switch (c [i])
		{
			case TREE:
			r = v2f64_in_triangle (pa, pb, pc, p + i);
			//printf ("r %i\n", r);
			if (r) 
			{
				//printf ("Found tree (%lu) in the way\n", (lu)i);
				t [(*o)] = i;
				t [i] = ib;
				(*o) = i;
			}
			break;
		}
		if (r) {break;}
	}
	if (!r)
	{
		t [ia] = ic;
		t [ib] = ib;
	}
	return 0;
}



double length1 
(
	uint32_t n, 
	
	//Input
	//Point data
	struct v2f64 p [], 
	
	//Input Output
	//Track data
	uint32_t t []
)
{
	double sum = 0;
	for (uint32_t i = 0; i < n; ++ i)
	{
		if (t [i] == UINT32_MAX) {continue;}
		if (t [i] == i) {continue;}
		struct v2f64 * a = p + i;
		struct v2f64 * b = p + t [i];
		double d [2];
		vf64_sub (2, d, (double*)a, (double*)b);
		sum += sqrt (vf64_dot (2, d, d));
	}
	return sum;
}



struct skopypark
{
	uint32_t n;
	struct v2f64 * p;
	uint32_t * c;
	uint32_t * t;
	double l;
};


void skopypark_from_file 
(
	struct skopypark * park,
	FILE * f
)
{
	typedef long unsigned lu;
	uint32_t ntree;
	uint32_t ntoy;
	fscanf (f, "%lu%lu", (lu*)&ntoy, (lu*)&ntree);
	//TRACE_F ("%lu %lu", (lu)ntree, (lu)ntoy);
	park->n = ntree + ntoy + 1; //Plus one skopy.
	park->p = (struct v2f64 *) malloc (sizeof (struct v2f64) * park->n);
	park->c = (uint32_t *) malloc (sizeof (struct v2f64) * park->n);
	park->t = (uint32_t *) malloc (sizeof (struct v2f64) * park->n);
	vu32_set1 (park->n, park->t, UINT32_MAX);
	uint32_t j = 1;
	park->c [0] = DOG;
	park->p [0].x = 0;
	park->p [0].y = 0;
	for (uint32_t i = 0; i < ntoy; ++ i)
	{
		park->c [j] = TOY;
		j++;
	}
	for (uint32_t i = 0; i < ntree; ++ i)
	{
		park->c [j] = TREE;
		j++;
	}
	for (uint32_t i = 1; i < park->n; ++ i)
	{
		int r = fscanf (f, "%lf%lf", &park->p [i].x, &park->p [i].y);
		ASSERT (r != EOF);
	}
}


void skopypark_from_filename 
(
	struct skopypark * park,
	char const * filename
)
{
	FILE * f = fopen (filename, "r");
	ASSERT (f != NULL);
	skopypark_from_file (park, f);
	fclose (f);
}


char const * skopypark_classstr (uint32_t c)
{
	switch (c)
	{
		case TREE: return "TREE";
		case TOY: return "TOY";
		case DOG: return "DOG";
	};
	return "";
}


void skopypark_print (struct skopypark * park)
{
	typedef long unsigned lu;
	for (uint32_t i = 0; i < park->n; ++ i)
	{
		char const * c = skopypark_classstr (park->c [i]);
		printf ("%5lu "   , (lu)i);
		printf ("%5s "    , c);
		printf ("%10.2lf ", park->p [i].x);
		printf ("%10.2lf ", park->p [i].y);
		printf ("%10lu "  , (lu)park->t [i]);
		printf ("\n");
	}
}
