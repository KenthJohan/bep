#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include "debug.h"
#include "tools.h"
#include "global.h"
#include "skopy.h"

#define POINT_COUNT 20


void show 
(
	cv::Mat & image,
	uint32_t n, 
	struct v2f64 p [], 
	uint32_t t [], 
	uint32_t c []
)
{
	char buf [100];
	snprintf (buf, 100, "L=%lf", length1 (POINT_COUNT, p, t));
	image.setTo (cv::Scalar (255,255,255));
	cv::putText (image, buf, cv::Point (100, 100), CV_FONT_HERSHEY_SIMPLEX, 1, cv::Scalar (0, 0, 0));
	paint_points_index (image, n, p, c);
	paint_tracks (image, n, p, t, cv::Scalar (200, 200, 200));
	cv::imshow ("test", image);
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
	double transform [4] = {0,0,0,0};
	
	vu32_set1 (POINT_COUNT, track, UINT32_MAX);

	srand (7);
	v2f64_random (POINT_COUNT, points);
	v2f64_mul1 (POINT_COUNT, points, points, p1);
	points [idog].x = 500;
	points [idog].y = 500;
	
	cv::Mat image (1000, 1000, CV_8UC3);
	image.setTo (cv::Scalar (255,255,255));
	show (image, POINT_COUNT, points, track, class1);
			
	uint32_t flags = 0;
	while (1)
	{
		get_flags (&flags);
		if (flags & ACTION_QUIT) {break;}
		if (flags & ACTION_PAUSE) {continue;}
		
		if (flags & ACTION_NEXTTOY)
		{
			shortest (POINT_COUNT, points, track, class1, idog);
			if (track [idog] != UINT32_MAX)
			{
				idog = track [idog];
			}
		}
		
		if (flags & ACTION_SIMPLIFY0)
		{
			simplify_trivial (POINT_COUNT, points, track, class1, &idog2);
		}
		
		if (flags & ACTION_SIMPLIFY1)
		{
			simplify1 (POINT_COUNT, points, track, class1, &idog3);
			TRACE_F ("idog3 %lu %lu", idog3, track [idog3]);
		}
		
		if (flags & ACTION_RESET)
		{
			idog = 0;
			idog2 = 0;
			idog3 = 0;
		}
		
		
		transform [0] += (!!(flags & ACTION_MOVE_RIGHT) - !!(flags & ACTION_MOVE_LEFT)) * 10.0;
		transform [1] += (!!(flags & ACTION_MOVE_DOWN) - !!(flags & ACTION_MOVE_UP)) * 10.0;
		transform [2] += (!!(flags & ACTION_MOVE_ZOOMIN) - !!(flags & ACTION_MOVE_ZOOMOUT)) * 0.2;
		transform [3] += (!!(flags & ACTION_MOVE_ZOOMIN) - !!(flags & ACTION_MOVE_ZOOMOUT)) * 0.2;
		
		
		uint32_t u = 
		ACTION_NEXTTOY | 
		ACTION_SIMPLIFY0 | 
		ACTION_SIMPLIFY1 | 
		ACTION_MOVE_LEFT |
		ACTION_MOVE_RIGHT |
		ACTION_MOVE_DOWN |
		ACTION_MOVE_UP |
		ACTION_MOVE_ZOOMIN |
		ACTION_MOVE_ZOOMOUT;
		if (flags & u)
		{
			TRACE ("");
			struct v2f64 tpoints [POINT_COUNT];
			move (POINT_COUNT, tpoints, points, transform);
			show (image, POINT_COUNT, tpoints, track, class1);
		}
		
		
		
		
	}
	return 0;
}

