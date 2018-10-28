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
};


void skopypark2_init (struct skopypark2 * park)
{
	park->ntree = 20;
	park->ntoy = 20;
	park->toys = (struct v2f64 *) malloc (sizeof (struct v2f64) * park->ntoy);
	park->trees = (struct v2f64 *) malloc (sizeof (struct v2f64) * park->ntree);
	park->track = (uint32_t *) malloc (sizeof (uint32_t) * park->ntoy);
	vu32_set1 (park->ntoy, park->track, UINT32_MAX);
	
	struct v2f64 p1 [1] = {1000, 1000};
	srand (7);
	v2f64_random (park->ntoy, park->toys);
	v2f64_random (park->ntree, park->trees);
	v2f64_mul1 (park->ntoy, park->toys, park->toys, p1);
	v2f64_mul1 (park->ntree, park->trees, park->trees, p1);
	park->toys [0].x = 500;
	park->toys [0].y = 500;
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
		}
		
		
		if (flags & NEXTACTION)
		{
			image.setTo (cv::Scalar (255, 255, 255));
			draw_v2f64 (image, park.ntoy, park.toys, cv::Scalar (200, 40, 255));
			draw_v2f64 (image, park.ntree, park.trees, cv::Scalar (20, 200, 20));
			draw_v2f64_track (image, park.ntoy, park.toys, park.track, cv::Scalar (200, 40, 255));
			draw_v2f64_star (image, park.toys + idog, park.ntree, park.trees, cv::Scalar (210, 210, 210));
			cv::imshow ("test", image);
		}
		
	}
	return 0;
}

