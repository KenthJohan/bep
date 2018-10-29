#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>


#include "debug.h"
#include "tools.h"
#include "global.h"


void draw_length
(
	cv::Mat & image,
	uint32_t n, 
	struct v2f64 p [], 
	uint32_t t []
)
{
	char buf [100];
	snprintf (buf, 100, "L=%lf", length1 (n, p, t));
	cv::putText (image, buf, cv::Point (100, 100), CV_FONT_HERSHEY_SIMPLEX, 1, cv::Scalar (0, 0, 0));
}


void draw_help (cv::Mat & image)
{
	char buf [100];
	snprintf (buf, 100, "1: next toy");
	cv::putText (image, buf, cv::Point (100, 100), CV_FONT_HERSHEY_SIMPLEX, 2, cv::Scalar (0,0,0));
	snprintf (buf, 100, "2: simplify0");
	cv::putText (image, buf, cv::Point (100, 200), CV_FONT_HERSHEY_SIMPLEX, 2, cv::Scalar (0,0,0));
	snprintf (buf, 100, "3: simplify1");
	cv::putText (image, buf, cv::Point (100, 300), CV_FONT_HERSHEY_SIMPLEX, 2, cv::Scalar (0,0,0));
	snprintf (buf, 100, "a: Translate left");
	cv::putText (image, buf, cv::Point (100, 400), CV_FONT_HERSHEY_SIMPLEX, 2, cv::Scalar (0,0,0));
	snprintf (buf, 100, "d: Translate right");
	cv::putText (image, buf, cv::Point (100, 500), CV_FONT_HERSHEY_SIMPLEX, 2, cv::Scalar (0,0,0));
	snprintf (buf, 100, "w: Translate up");
	cv::putText (image, buf, cv::Point (100, 600), CV_FONT_HERSHEY_SIMPLEX, 2, cv::Scalar (0,0,0));
	snprintf (buf, 100, "s: Translate down");
	cv::putText (image, buf, cv::Point (100, 700), CV_FONT_HERSHEY_SIMPLEX, 2, cv::Scalar (0,0,0));
	snprintf (buf, 100, "+: Zoom in");
	cv::putText (image, buf, cv::Point (100, 800), CV_FONT_HERSHEY_SIMPLEX, 2, cv::Scalar (0,0,0));
	snprintf (buf, 100, "-: Zoom out");
	cv::putText (image, buf, cv::Point (100, 900), CV_FONT_HERSHEY_SIMPLEX, 2, cv::Scalar (0,0,0));
}


int main (int argc, char** argv)
{
	setbuf (stdout, NULL);
	setbuf (stderr, NULL);
	ASSERT_F (argc == 2, "Need exactly one argument that is the filename of the input");
	
	typedef long unsigned lu;
	setbuf (stdout, NULL);
	setbuf (stderr, NULL);
	
	struct skopypark park;
	//skopypark_from_filename (&park, "data/barktree.06.in");
	skopypark_from_filename (&park, argv [1]);
	skopypark_print (&park);
	struct v2f64 * tpoints = (struct v2f64 *)malloc (park.n * sizeof (struct v2f64));

	uint32_t idog = 0;
	uint32_t idog2 = 0;
	uint32_t idog3 = 0;
	
	cv::Mat image (1000, 1000, CV_8UC3);
	image.setTo (cv::Scalar (255,255,255));
	draw_help (image);
	draw_points_index (image, park.n, park.p, park.c);
	draw_tracks (image, park.n, park.p, park.t, cv::Scalar (200, 200, 200));
	cv::imshow ("test", image);
	
	
	double transform [4] = {0,0,0,0};
	
	uint32_t flags = 0;
	while (1)
	{
		get_flags (&flags);
		if (flags & ACTION_QUIT) {break;}
		if (flags & ACTION_PAUSE) {continue;}
		
		if (flags & ACTION_NEXTTOY)
		{
			skopy_goto_newtoy (park.n, park.p, park.t, park.c, idog);
			if (park.t [idog] != UINT32_MAX)
			{
				idog = park.t [idog];
			}
		}
		
		if (flags & ACTION_SIMPLIFY0)
		{
			skopy_triangle_simplify0 (park.n, park.p, park.t, park.c, &idog2);
		}
		
		if (flags & ACTION_SIMPLIFY1)
		{
			skopy_triangle_simplify1 (park.n, park.p, park.t, park.c, &idog3);
			//TRACE_F ("idog3 %lu %lu", idog3, park.t [idog3]);
		}
		
		if (flags & ACTION_RESET)
		{
			idog = 0;
			idog2 = 0;
			idog3 = 0;
		}
		
		
		transform [0] += (!!(flags & ACTION_MOVE_RIGHT) - !!(flags & ACTION_MOVE_LEFT)) * -50.0;
		transform [1] += (!!(flags & ACTION_MOVE_DOWN) - !!(flags & ACTION_MOVE_UP)) * -50.0;
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
			move (park.n, tpoints, park.p, transform);
			cv::Mat image (1000, 1000, CV_8UC3);
			image.setTo (cv::Scalar (255,255,255));
			draw_points_index (image, park.n, tpoints, park.c);
			draw_tracks (image, park.n, tpoints, park.t, cv::Scalar (200, 200, 200));
			draw_length (image, park.n, park.p, park.t);
			cv::imshow ("test", image);
		}

	}
	return 0;
}

