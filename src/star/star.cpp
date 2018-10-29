#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>

//Common
#include "debug.h"
#include "global.h"

#include "tools.h"
#include "skopy.h"







struct skopypark2
{
	uint32_t ntoy;
	uint32_t ntree;
	struct v2f64 * toys;
	struct v2f64 * trees;
	uint32_t * track;
	struct v2f64 * treed;
	double * windings;
	double * windings0;
};


void skopypark2_init (struct skopypark2 * park)
{
	park->ntree = 20;
	park->ntoy = 20;
	park->toys = (struct v2f64 *) malloc (sizeof (struct v2f64) * park->ntoy);
	park->trees = (struct v2f64 *) malloc (sizeof (struct v2f64) * park->ntree);
	park->treed = (struct v2f64 *) malloc (sizeof (struct v2f64) * park->ntree);
	park->track = (uint32_t *) malloc (sizeof (uint32_t) * park->ntoy);
	park->windings = (double *) malloc (sizeof (double) * park->ntoy);
	park->windings0 = (double *) malloc (sizeof (double) * park->ntoy);
	vu32_set1 (park->ntoy, park->track, UINT32_MAX);
	vf64_set1 (park->ntoy, park->windings, 0.0);
	vf64_set1 (park->ntoy, park->windings0, 0.0);
	vf64_set1 (park->ntoy * 2, (double *)park->treed, 0.0);
	
	
	struct v2f64 p1 [1] = {1000, 1000};
	srand (7);
	v2f64_random (park->ntoy, park->toys);
	v2f64_random (park->ntree, park->trees);
	v2f64_mul1 (park->ntoy, park->toys, park->toys, p1);
	v2f64_mul1 (park->ntree, park->trees, park->trees, p1);
	park->toys [0].x = 500;
	park->toys [0].y = 500;
}


void entagle 
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
	//Some elements of this can build up 
	//if (o) spins around some (p) coordinates
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
		w [i] += a * (180 / M_PI);
		u [i].x = op [0];
		u [i].y = op [1];
	}
}


void entagle0 
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



int main (int argc, char** argv)
{
	typedef long unsigned lu;
	setbuf (stdout, NULL);
	setbuf (stderr, NULL);
	
	struct skopypark2 park;
	skopypark2_init (&park);

	
	cv::Mat image (1920, 1080, CV_8UC3);
	image.setTo (cv::Scalar (255, 255, 255));
	draw_v2f64 (image, park.ntoy, park.toys, cv::Scalar (200, 40, 255));
	draw_v2f64 (image, park.ntree, park.trees, cv::Scalar (20, 200, 20));
	cv::imshow ("test", image);
	
	uint32_t idog = 0;
	entagle0 (park.toys + idog, park.ntree, park.trees, park.treed);
	image.setTo (cv::Scalar (255, 255, 255));
	draw_v2f64 (image, park.ntoy, park.toys, cv::Scalar (200, 40, 255));
	draw_v2f64 (image, park.ntree, park.trees, cv::Scalar (20, 200, 20));
	draw_v2f64_track (image, park.ntoy, park.toys, park.track, cv::Scalar (200, 40, 255));
	draw_v2f64_star (image, park.toys + idog, park.ntree, park.trees, park.windings, cv::Scalar (210, 210, 210));
	cv::imshow ("test", image);
			
	uint32_t flags = 0;
	while (1)
	{
		get_flags (&flags);
		//printf ("%i\n", flags);
		if (flags & QUIT) {break;}
		
		if (flags & NEXTACTION) 
		{
			shortest (park.ntoy, park.toys, park.track, idog);
			TRACE_F ("%u", idog);
			if (park.track [idog] != UINT32_MAX)
			{
				idog = park.track [idog];
			}
			entagle (park.toys [idog].v, park.ntree, park.trees, park.treed, park.windings);
		}
		
		
		if (flags & NEXTACTION)
		{
			image.setTo (cv::Scalar (255, 255, 255));
			draw_v2f64 (image, park.ntoy, park.toys, cv::Scalar (200, 40, 255));
			draw_v2f64 (image, park.ntree, park.trees, cv::Scalar (20, 200, 20));
			draw_v2f64_track (image, park.ntoy, park.toys, park.track, cv::Scalar (200, 40, 255));
			draw_v2f64_star (image, park.toys + idog, park.ntree, park.trees, park.windings, cv::Scalar (210, 210, 210));
			cv::imshow ("test", image);
		}
		
	}
	return 0;
}

