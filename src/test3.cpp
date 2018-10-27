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
	cv::imshow ("test", image);
}


struct skopypark2
{
	uint32_t n;
	struct v2f64 * p;
	uint32_t * c;
	uint32_t * t;
};

void skopypark2_init (struct skopypark2 * park)
{
	park->n = 20;
	park->p = (struct v2f64 *) malloc (sizeof (struct v2f64) * park->n);
	park->c = (uint32_t *) malloc (sizeof (uint32_t) * park->n);
	park->t = (uint32_t *) malloc (sizeof (uint32_t) * park->n);
	vu32_set1 (park->n, park->t, UINT32_MAX);
	uint32_t c [] = {DOG,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1};
	memcpy (park->c, c, sizeof (uint32_t) * park->n);
	struct v2f64 p1 [1] = {1000, 1000};
	srand (7);
	v2f64_random (park->n, park->p);
	v2f64_mul1 (park->n, park->p, park->p, p1);
	park->p [0].x = 500;
	park->p [0].y = 500;
}


void skopypark2_print (struct skopypark2 * park)
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




int main (int argc, char** argv)
{
	typedef long unsigned lu;
	setbuf (stdout, NULL);
	setbuf (stderr, NULL);
	
	struct skopypark2 park;
	skopypark2_init (&park);
	skopypark2_print (&park);
	
	
	cv::Mat image (1920, 1080, CV_8UC3);
	render (image, park.n, park.p, park.t, park.c);
	
	uint32_t idog = 0;
	uint32_t flags = 0;
	while (1)
	{
		get_flags (&flags);
		//printf ("%i\n", flags);
		if (flags & QUIT) {break;}
		if (flags & PAUSE) {continue;}
		
		
		if (flags & FILTER1)
		{
			shortest (park.n, park.p, park.t, park.t, idog);
			TRACE_F ("%u", idog);
			TRACE_F ("%u", park.t [idog]);
			if (park.t [idog] != UINT32_MAX)
			{
				idog = park.t [idog];
			}
			render (image, park.n, park.p, park.t, park.c);
		}
		

		
	}
	return 0;
}

