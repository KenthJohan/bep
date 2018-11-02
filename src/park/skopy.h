#pragma once

#include <math.h>
#include "global.h"
#include "debug.h"
#include "v.h"




struct skopypark
{
	uint32_t ntoy;
	uint32_t ntree;
	uint32_t ntrack;
	struct v2f64 * tree;
	struct v2f64 * toy;
	uint32_t * track;
	
	uint32_t idog;
	uint32_t itree;
};


void skopypark_from_file 
(
	struct skopypark * park,
	FILE * f
)
{
	typedef long unsigned lu;
	fscanf (f, "%lu%lu", (lu*)&park->ntoy, (lu*)&park->ntree);
	park->ntoy = park->ntoy + 1;
	park->ntrack = park->ntoy * park->ntoy;
	TRACE_F ("ALLOCATING %lu toys", (lu)park->ntoy);
	TRACE_F ("ALLOCATING %lu trees", (lu)park->ntree);
	TRACE_F ("ALLOCATING %lu track", (lu)park->ntrack);
	park->toy = (struct v2f64 *) malloc (sizeof (struct v2f64) * park->ntree);
	park->tree = (struct v2f64 *) malloc (sizeof (struct v2f64) * park->ntoy);
	park->track = (uint32_t *) malloc (sizeof (uint32_t) * park->ntrack);
	park->idog = 0;
	park->itree = UINT32_MAX;
	park->toy [park->idog].x = 0;
	park->toy [park->idog].y = 0;
	for (uint32_t i = 1; i < park->ntoy; ++ i)
	{
		int r = fscanf (f, "%lf%lf", &park->toy [i].x, &park->toy [i].y);
		ASSERT (r != EOF);
	}
	for (uint32_t i = 0; i < park->ntree; ++ i)
	{
		int r = fscanf (f, "%lf%lf", &park->tree [i].x, &park->tree [i].y);
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

















void entangle 
(
	uint32_t ntoy,
	struct v2f64 toy [],
	uint32_t ntree,
	struct v2f64 tree [],
	uint32_t idog,
	struct v2f64 * a,
	uint32_t * itree
)
{
	typedef long unsigned lu;
	//struct v2f64 * a = toy + idog;
	struct v2f64 * b = toy + idog + 1;
	struct v2f64 * c = toy + idog + 2;
	double lmin = DBL_MAX;
	uint32_t imin = UINT32_MAX;
	for (uint32_t i = 0; i < ntree; ++ i)
	{
		int r = v2f64_in_triangle (a, b, c, tree + i);
		if (r)
		{
			TRACE_F ("TREE %lu in the way", (lu)i);
			double l = v2f64_point_line_distance (tree [i].v, a->v, b->v);
			if (l < lmin)
			{
				lmin = l;
				imin = i;
			}
		}
	}
	
	TRACE_F ("TREEMIN %lu", (lu)imin);
	(*itree) = imin;
	
}






