#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>

//Common
#include "debug.h"
#include "global.h"

#include "tools.h"
#include "skopy.h"






int main (int argc, char** argv)
{
	typedef long unsigned lu;
	setbuf (stdout, NULL);
	setbuf (stderr, NULL);
	
	struct skopypark park;
	skopypark_from_filename (&park, "data/barktree.02.in");
	
	//Dog position in the track.
	uint32_t idog = 0;
	double transform [4] = {0,0,0,0};
	
	//Initialize tree leash entanglement vectors.
	//At start skopy is entagled 0 amount around every tree.
	skopy_entagle0 (park.toys + idog, park.ntree, park.trees, park.treed);
	
	//Initial drawings
	cv::Mat image (1080, 1920, CV_8UC3);
	image.setTo (cv::Scalar (255, 255, 255));
	draw_v2f64_star (image, park.toys + idog, park.ntree, park.trees, park.windings, cv::Scalar (230, 230, 230));
	draw_v2f64 (image, park.ntoy, park.toys, cv::Scalar (200, 40, 255));
	draw_v2f64 (image, park.ntree, park.trees, cv::Scalar (20, 200, 20));
	draw_v2f64_track (image, park.ntoy, park.toys, park.track, cv::Scalar (200, 40, 255));
	{
		char buf [100];
		snprintf (buf, 100, "spacebar: next step");
		cv::putText (image, buf, cv::Point (100, 100), CV_FONT_HERSHEY_SIMPLEX, 2, cv::Scalar (0,0,0));
	}
	cv::imshow ("test1", image);



	uint32_t flags = 0;
	while (1)
	{
		get_flags (&flags);
		if (flags & ACTION_QUIT) {break;}
		if (flags & ACTION_PAUSE) {continue;}
		
		if (flags & ACTION_NEXTTOY) 
		{
			if (idog < park.ntoy-1)
			{
				idog = idog + 1;
			}
			//Every tree is keeping track of how many times skopy has circled around.
			skopy_entagle (park.toys [idog].v, park.ntree, park.trees, park.treed, park.windings);
		}
		
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
			transform [0] += (!!(flags & ACTION_MOVE_RIGHT) - !!(flags & ACTION_MOVE_LEFT)) * -50.0;
			transform [1] += (!!(flags & ACTION_MOVE_DOWN) - !!(flags & ACTION_MOVE_UP)) * -50.0;
			transform [2] += (!!(flags & ACTION_MOVE_ZOOMIN) - !!(flags & ACTION_MOVE_ZOOMOUT)) * 0.2;
			transform [3] += (!!(flags & ACTION_MOVE_ZOOMIN) - !!(flags & ACTION_MOVE_ZOOMOUT)) * 0.2;
			
			move (park.ntoy, park.toyt, park.toys, transform);
			move (park.ntree, park.treet, park.trees, transform);
			
			image.setTo (cv::Scalar (255, 255, 255));
			draw_v2f64_star (image, park.toyt + idog, park.ntree, park.treet, park.windings, cv::Scalar (230, 230, 230));
			draw_v2f64 (image, park.ntoy, park.toyt, cv::Scalar (200, 40, 255));
			draw_v2f64 (image, park.ntree, park.treet, cv::Scalar (20, 200, 20));
			draw_v2f64_track (image, park.ntoy, park.toyt, park.track, cv::Scalar (200, 40, 255));
			cv::imshow ("test1", image);
		}
		
	}
	return 0;
}



