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
		skopy_goto_newtoy (park.n, park.p, park.t, park.c, idog);
		if (park.t [idog] == UINT32_MAX) {break;}
		idog = park.t [idog];
	}
	
	idog = 0;
	while (1)
	{
		int r = skopy_triangle_simplify0 (park.n, park.p, park.t, park.c, &idog);
		//printf ("idog %lu\n", (lu)idog);
		if (r) {break;}
	}
	
	idog = 0;
	while (1)
	{
		int r = skopy_triangle_simplify1 (park.n, park.p, park.t, park.c, &idog);
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




int main (int argc, char** argv)
{
	setbuf (stdout, NULL);
	setbuf (stderr, NULL);
	ASSERT_F (argc == 2, "Need exactly one argument that is the filename of the input");
	
	double l = test (argv [1]);
	printf ("%lf\n", l);
	

	return 0;
}


