#include <stdio.h>
#include <stdint.h>
#include "debug.h"
#include "global.h"
#include "skopy.h"


double test (char const * filename)
{
	typedef long unsigned lu;
	setbuf (stdout, NULL);
	setbuf (stderr, NULL);
	
	struct skopypark park;
	skopypark_from_filename (&park, filename);
	
	uint32_t idog;
	
	idog = 0;
	while (1)
	{
		shortest (park.n, park.p, park.t, park.c, idog);
		if (park.t [idog] == UINT32_MAX) {break;}
		idog = park.t [idog];
	}
	
	idog = 0;
	while (1)
	{
		int r = simplify_trivial (park.n, park.p, park.t, park.c, &idog);
		//printf ("idog %lu\n", (lu)idog);
		if (r) {break;}
	}
	
	idog = 0;
	while (1)
	{
		int r = simplify1 (park.n, park.p, park.t, park.c, &idog);
		//printf ("idog %lu\n", (lu)idog);
		if (r) {break;}
	}
	
	//skopypark_print (&park);
	
	double l = length1 (park.n, park.p, park.t);
	
	free (park.p);
	free (park.t);
	free (park.c);
	return l;
}


double readans (char const * filename)
{
	FILE * f = fopen (filename, "r");
	ASSERT (f != NULL);
	double l = 0;
	fscanf (f, "%lf", &l);
	fclose (f);
	return l;
}


int main (int argc, char** argv)
{
	for (uint32_t i = 0; i < 100; ++ i)
	{
		char buf [100];
		snprintf (buf, 100, "data/barktree.%02u.in", (unsigned) i);
		//TRACE_F ("%s", buf);
		double l1 = test (buf);
		snprintf (buf, 100, "data/barktree.%02u.ans", (unsigned) i);
		double l2 = readans (buf);
		printf ("%20s %20.10lf %20.10lf\n", buf, l1, l2);
	}
	

	return 0;
}


