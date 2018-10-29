#pragma once

#include <math.h>
#include "global.h"
#include "debug.h"
#include "v.h"


void skopy_goto_newtoy 
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
	//TRACE_F ("FIND NEXT FROM %lu", (lu)o);
	double lmin = DBL_MAX;
	uint32_t imin = UINT32_MAX;
	double d [2];
	for (uint32_t i = 0; i < n; ++ i)
	{
		//TRACE_F ("FIND NEXT FROM %lu", t [i]);
		if (o == i) {continue;}
		if (t [i] != UINT32_MAX) {continue;}
		double * a = p [o].v;
		double * b = p [i].v;
		vf64_sub (2, d, a, b);
		double l = vf64_dot (2, d, d);
		//TRACE_F ("%10lu %4lu %4lu %7.3f", t [i], o, i, l);
		if (l < lmin)
		{
			lmin = l;
			imin = i;
		}
	}
	//TRACE_F ("SET %lu %lu", (long unsigned) o, (long unsigned) imin);
	if (imin != UINT32_MAX)
	{
		t [o] = imin;
	}
}


void skopy_entagle 
(
	//Origin.
	double o [2],
	
	//Number of windings and coordinates.
	uint32_t n, 
	
	//Coordinates.
	struct v2f64 p [],
	
	//Last vectors between (o) and (p).
	struct v2f64 u [],
	
	//Windings.
	//Element (i) of this can build up if (o) spins around (p [i]).
	double w []
)
{
	for (uint32_t i = 0; i < n; ++ i)
	{
		double op [2];
		vf64_sub (2, op, o, p [i].v);
		//Does not output negative angle between two vectors.
		double a = vf64_angle (2, u [i].v, op);
		//https://stackoverflow.com/questions/2150050/finding-signed-angle-between-vectors
		//perp dot product???
		if ((u [i].x*op [1] - u [i].y*op [0]) < 0) {a = -a;}
		//Accumulate the angle.
		w [i] += a * (180 / M_PI);
		//Store last vector.
		u [i].x = op [0];
		u [i].y = op [1];
	}
}


void skopy_entagle0 
(
	//Origin
	struct v2f64 o [1],
	
	//Number of windings and coordinates
	uint32_t n, 
	
	//Coordinates
	struct v2f64 p [],
	struct v2f64 u []
)
{
	for (uint32_t i = 0; i < n; ++ i)
	{
		vf64_sub (2, (double *) (u + i), (double *) o, (double *) (p + i));
	}
}


struct skopypark
{
	uint32_t ntoy;
	uint32_t ntree;
	
	//Toys coordinates
	struct v2f64 * toys;
	
	//Tree coordinates
	struct v2f64 * trees;
	
	//Track coordinates indices
	uint32_t * track;
	
	//Last tree to dog vector
	struct v2f64 * treed;
	
	//Leash windings around the tree.
	double * windings;
};


void skopypark_init (struct skopypark * park)
{
	park->ntree = 20;
	park->ntoy = 40;
	park->toys = (struct v2f64 *) malloc (sizeof (struct v2f64) * park->ntoy);
	park->trees = (struct v2f64 *) malloc (sizeof (struct v2f64) * park->ntree);
	park->treed = (struct v2f64 *) malloc (sizeof (struct v2f64) * park->ntree);
	park->track = (uint32_t *) malloc (sizeof (uint32_t) * park->ntoy);
	park->windings = (double *) malloc (sizeof (double) * park->ntoy);
	vu32_set1 (park->ntoy, park->track, UINT32_MAX);
	vf64_set1 (park->ntoy, park->windings, 0.0);
	vf64_set1 (park->ntree * 2, (double *)park->treed, 0.0);
	
	//Create random scenario
	struct v2f64 p1 [1] = {1920, 1080};
	srand (12);
	v2f64_random (park->ntoy, park->toys);
	v2f64_random (park->ntree, park->trees);
	v2f64_mul1 (park->ntoy, park->toys, park->toys, p1);
	v2f64_mul1 (park->ntree, park->trees, park->trees, p1);
	park->toys [0].x = 500;
	park->toys [0].y = 500;
}
