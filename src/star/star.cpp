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
	skopypark_init (&park);
	
	//Dog position in the track.
	uint32_t idog = 0;
	
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
		//printf ("%i\n", flags);
		if (flags & QUIT) {break;}
		
		if (flags & NEXTACTION) 
		{
			//Assuming every toy is 100% shiny at beginning.
			//Assuming Skopy goes to a 100% shiny nearest toy.
			//Assuming Skopy shews the next toy to 0% shiny.
			skopy_goto_newtoy (park.ntoy, park.toys, park.track, idog);
			//TRACE_F ("%u", idog);
			if (park.track [idog] != UINT32_MAX)
			{
				idog = park.track [idog];
			}
			//Every tree is keeping track of how many times skopy has circled around.
			skopy_entagle (park.toys [idog].v, park.ntree, park.trees, park.treed, park.windings);
		}
		
		
		if (flags & NEXTACTION)
		{
			image.setTo (cv::Scalar (255, 255, 255));
			draw_v2f64_star (image, park.toys + idog, park.ntree, park.trees, park.windings, cv::Scalar (230, 230, 230));
			draw_v2f64 (image, park.ntoy, park.toys, cv::Scalar (200, 40, 255));
			draw_v2f64 (image, park.ntree, park.trees, cv::Scalar (20, 200, 20));
			draw_v2f64_track (image, park.ntoy, park.toys, park.track, cv::Scalar (200, 40, 255));
			cv::imshow ("test1", image);
		}
		
	}
	return 0;
}

