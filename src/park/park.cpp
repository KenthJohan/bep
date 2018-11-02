#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>


#include "debug.h"
#include "tools.h"
#include "global.h"







int main (int argc, char** argv)
{
	setbuf (stdout, NULL);
	setbuf (stderr, NULL);
	//ASSERT_F (argc == 2, "Need exactly one argument that is the filename of the input");
	
	typedef long unsigned lu;
	setbuf (stdout, NULL);
	setbuf (stderr, NULL);
	
	struct drawdata draw;
	struct skopypark park;
	skopypark_from_filename (&park, "data/barktree.04.in");
	drawdata_init (&draw, &park);
	
	
	double transform [4] = {0,0,0,0};
	
	cv::Mat image (1000, 1000, CV_8UC3);
	image.setTo (cv::Scalar (255, 255, 255));
	cv::imshow ("test", image);
			
	uint32_t flags = 0;
	while (1)
	{
		get_flags (&flags);
		if (flags & ACTION_QUIT) {break;}
		if (flags & ACTION_PAUSE) {continue;}
		
		if (flags & ACTION_NEXTTOY)
		{
			if (park.itree == UINT32_MAX)
			{
				entangle (park.ntoy, park.toy, park.ntree, park.tree, park.idog, park.toy + park.idog, &park.itree);
			}
			else
			{
				entangle (park.ntoy, park.toy, park.ntree, park.tree, park.idog, park.tree + park.itree, &park.itree);
				if (park.itree == UINT32_MAX)
				{
					park.idog ++;
				}
			}
			
			//entangle (park.ntoy, park.toy, park.ntree, park.tree, park.idog, park.tree + park.itree, &park.itree);
			//park.idog ++;
		}
		
		if (flags & ACTION_NEXTTREE)
		{
			if (park.itree != UINT32_MAX)
			{
				
			}
		}
		
		
		uint32_t u = 
		ACTION_NEXTTOY | 
		ACTION_NEXTTREE | 
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
			drawdata_update (&draw, &park, transform);
		}

	}
	return 0;
}

