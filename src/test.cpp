#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include "debug.h"
#include "tools.h"
#include "global.h"
#include "skopy.h"

#define POINT_COUNT 20


void render 
(
	cv::Mat & image,
	uint32_t n, 
	struct v2f64 p [], 
	uint32_t t [], 
	uint32_t c []
)
{
	paint_points_index (image, n, p, c);
	paint_tracks (image, n, p, t, cv::Scalar (200, 200, 200));
}



int main (int argc, char** argv)
{
	setbuf (stdout, NULL);
	setbuf (stderr, NULL);
	
	struct v2f64 points [POINT_COUNT];
	uint32_t class1 [POINT_COUNT] = {DOG,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1};
	struct v2f64 p1 [1] = {1000, 1000};
	uint32_t track [POINT_COUNT];
	uint32_t idog = 0;
	uint32_t idog2 = 0;
	uint32_t idog3 = 0;
	
	vu32_set1 (POINT_COUNT, track, UINT32_MAX);

	srand (7);
	v2f64_random (POINT_COUNT, points);
	v2f64_mul1 (POINT_COUNT, points, points, p1);
	points [idog].x = 500;
	points [idog].y = 500;
	
	cv::Mat image (1000, 1000, CV_8UC3);
	image.setTo (cv::Scalar (255,255,255));
	render (image, POINT_COUNT, points, track, class1);
	show ("test", image);
			
	uint32_t flags = 0;
	while (1)
	{
		get_flags (&flags);
		//printf ("%i\n", flags);
		if (flags & QUIT) {break;}
		if (flags & PAUSE) {continue;}
		
		if (flags & FILTER1)
		{
			shortest (POINT_COUNT, points, track, class1, idog);
			if (track [idog] != UINT32_MAX)
			{
				idog = track [idog];
			}
		}
		
		if (flags & FILTER2)
		{
			simplify_trivial (POINT_COUNT, points, track, class1, &idog2);
		}
		
		if (flags & FILTER3)
		{
			simplify1 (POINT_COUNT, points, track, class1, &idog3);
			printf ("idog3 %lu\n", idog3);
			printf ("idog3 %lu\n", track [idog3]);
		}
		
		if (flags & FILTER0)
		{
			idog = 0;
			idog2 = 0;
			idog3 = 0;
		}
		
		if (flags & (FILTER1 | FILTER2 | FILTER3))
		{
			TRACE ("");
			char buf [100];
			snprintf (buf, 100, "L=%lf", length1 (POINT_COUNT, points, track));
			image.setTo (cv::Scalar (255,255,255));
			cv::putText (image, buf, cv::Point (100, 100), CV_FONT_HERSHEY_SIMPLEX, 1, cv::Scalar (0, 0, 0));
			render (image, POINT_COUNT, points, track, class1);
			show ("test", image);
		}
		
	}
	return 0;
}

