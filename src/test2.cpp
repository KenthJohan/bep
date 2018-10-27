#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>


#include "debug.h"
#include "tools.h"
#include "global.h"





void render 
(
	cv::Mat & image,
	uint32_t n, 
	struct v2f64 p [], 
	uint32_t t [], 
	uint32_t c []
)
{
	image.setTo (cv::Scalar (255,255,255));
	paint_points_index (image, n, p, c);
	paint_tracks (image, n, p, t, cv::Scalar (200, 200, 200));
	show ("test", image);
}


void gen_class (uint32_t n, uint32_t c [], uint32_t ntoy, uint32_t ntree)
{
	uint32_t i = 0;
	c [i] = DOG;
	i ++;
	for (uint32_t j = 0; j < ntree; ++ j)
	{
		c [i] = TREE;
		i ++;
	}
	for (uint32_t j = 0; j < ntoy; ++ j)
	{
		c [i] = TOY;
		i ++;
	}
}


void read_count (uint32_t * ntoy, uint32_t * ntree, FILE * f)
{
	int r = fscanf (f, "%lu%lu", ntoy, ntree);
	TRACE_F ("ntoy, ntree = %lu %lu", *ntoy, *ntree);
	//if (r == EOF) {break;}
}


void read_points (uint32_t n, struct v2f64 p [], FILE * f)
{
	for (uint32_t i = 0; i < n; ++ i)
	{
		int r = fscanf (f, "%lf%lf", &p [i].x, &p [i].y);
		if (r == EOF) {break;}
	}
}



int main (int argc, char** argv)
{
	setbuf (stdout, NULL);
	setbuf (stderr, NULL);
	
	typedef long unsigned lu;
	setbuf (stdout, NULL);
	setbuf (stderr, NULL);
	
	struct skopypark park;
	skopypark_from_filename (&park, "data/barktree.06.in");
	skopypark_print (&park);

	uint32_t idog = 0;
	uint32_t idog2 = 0;
	uint32_t idog3 = 0;
	
	cv::Mat image (1920, 1080, CV_8UC3);
	render (image, park.n, park.p, park.t, park.c);
	
	uint32_t flags = 0;
	while (1)
	{
		get_flags (&flags);
		//printf ("%i\n", flags);
		if (flags & QUIT) {break;}
		if (flags & PAUSE) {continue;}
		
		if (flags & FILTER1)
		{
			shortest (park.n, park.p, park.t, park.c, idog);
			if (park.t [idog] != UINT32_MAX)
			{
				idog = park.t [idog];
			}
			render (image, park.n, park.p, park.t, park.c);
		}
		
		if (flags & FILTER2)
		{
			simplify_trivial (park.n, park.p, park.t, park.c, &idog2);
			render (image, park.n, park.p, park.t, park.c);
		}
		
		if (flags & FILTER3)
		{
			simplify1 (park.n, park.p, park.t, park.c, &idog3);
			render (image, park.n, park.p, park.t, park.c);
			printf ("idog3 %lu\n", idog3);
			printf ("idog3 %lu\n", park.t [idog3]);
			printf ("Length: %lf\n", length1 (park.n, park.p, park.t));
		}
		
		if (flags & FILTER0)
		{
			idog = 0;
			idog2 = 0;
			idog3 = 0;
		}
		

	}
	return 0;
}

