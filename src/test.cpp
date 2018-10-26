#include <opencv2/bgsegm.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/features2d.hpp>
#include <opencv2/core/utility.hpp>
#include <opencv2/xfeatures2d.hpp>


#include "debug.h"
#include "tools.h"
#include "global.h"

#define POINT_COUNT 20


void pr (uint32_t n, uint32_t x [])
{
	for (uint32_t i = 0; i < 10; ++ i) 
	{
		if (x [i] == UINT32_MAX) {printf ("%3s", "-");}
		else {printf ("%3lu", (long unsigned) x [i]);}
	};
	printf ("\n");
}

void shortest 
(
	uint32_t n, 
	
	//Pos
	struct v2f64 p [], 
	
	//Track
	uint32_t t [],
	
	//Class
	uint32_t c [], 
	
	//Start point index
	uint32_t o
)
{
	printf ("FIND NEXT FROM %lu\n", o);
	double lmin = DBL_MAX;
	uint32_t imin = UINT32_MAX;
	double d [2];
	for (uint32_t i = 0; i < n; ++ i)
	{
		if (c [i] != TOY) {continue;}
		if (o == i) {continue;}
		if (t [i] != UINT32_MAX) {continue;}
		double * a = p [o].v;
		double * b = p [i].v;
		vf64_sub (2, d, a, b);
		double l = vf64_dot (2, d, d);
		printf ("%10lu %4lu %4lu %7.3f\n", t [i], o, i, l);
		if (l < lmin)
		{
			lmin = l;
			imin = i;
		}
	}
	printf ("SET %lu %lu\n", (long unsigned) o, (long unsigned) imin);
	if (imin != UINT32_MAX)
	{
		t [o] = imin;
	}
	
	//printf ("\n\n");
}

/*
void test2 
(
	uint32_t n, 
	uint32_t t [], 
	struct v2f64 p [], 
	
	uint32_t no, 
	struct v2f64 o [],
	uint32_t iob
)
{
	
}

*/



void test0 
(
	uint32_t n, 
	struct v2f64 p [], 
	uint32_t t [], 
	uint32_t c [],
	uint32_t * o
)
{
	uint32_t ia = (*o);
	if (ia == UINT32_MAX) {printf ("ia == UINT32_MAX\n"); return;}
	uint32_t ib = t [ia];
	if (ib == UINT32_MAX) {printf ("ib == UINT32_MAX\n"); return;}
	uint32_t ic = t [ib];
	if (ic == UINT32_MAX) {printf ("ic == UINT32_MAX\n"); return;}
	

	struct v2f64 * pa = p + ia;
	struct v2f64 * pb = p + ib;
	struct v2f64 * pc = p + ic;
	
	printf ("(%lu %lu %lu)\n", ia, ib, ic);
	
	int r = 0;
	uint32_t i;
	for (i = 0; i < n; ++ i)
	{
		if ((*o) == i) {continue;}
		switch (c [i])
		{
			case TREE:
			r = point_inside_triangle (pa, pb, pc, p + i);
			break;
		}
		if (r)
		{
			(*o) = ib;
			break;
		}
	}
	if (!r)
	{
		t [ia] = ic;
		t [ib] = ib;
	}
}



void test1 
(
	uint32_t n, 
	struct v2f64 p [], 
	uint32_t t [], 
	uint32_t c [],
	uint32_t * o
)
{
	uint32_t ia = (*o);
	if (ia == UINT32_MAX) {printf ("ia == UINT32_MAX\n"); return;}
	uint32_t ib = t [ia];
	if (ib == UINT32_MAX) {printf ("ib == UINT32_MAX\n"); return;}
	uint32_t ic = t [ib];
	if (ic == UINT32_MAX) {printf ("ic == UINT32_MAX\n"); return;}
	

	struct v2f64 * pa = p + ia;
	struct v2f64 * pb = p + ib;
	struct v2f64 * pc = p + ic;
	
	printf ("(%lu %lu %lu)\n", ia, ib, ic);
	
	int r = 0;
	uint32_t i;
	for (i = 0; i < n; ++ i)
	{
		if ((*o) == i) {continue;}
		switch (c [i])
		{
			case TREE:
			r = point_inside_triangle (pa, pb, pc, p + i);
			//printf ("r %i\n", r);
			if (r) 
			{
				printf ("Found tree (%lu) in the way\n", i);
				t [(*o)] = i;
				t [i] = ib;
				(*o) = i;
			}
			break;
		}
		if (r) {break;}
	}
}



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



int main (int argc, char** argv)
{
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
	cv::imshow ("test", image);
	
	render (image, POINT_COUNT, points, track, class1);
	
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
			
			render (image, POINT_COUNT, points, track, class1);
		}
		
		if (flags & FILTER2)
		{
			test0 (POINT_COUNT, points, track, class1, &idog2);
			render (image, POINT_COUNT, points, track, class1);
		}
		
		if (flags & FILTER3)
		{
			test1 (POINT_COUNT, points, track, class1, &idog3);
			render (image, POINT_COUNT, points, track, class1);
		}
		
		/*
		if (flags & NEXTACTION)
		{
			shortest (10, ptoy, idog, taken);
			if (taken [idog] != UINT32_MAX)
			{
				idog = taken [idog];
			}
			pr (10, taken);
			printf ("dog %lu\n", (long unsigned) idog);
			//track [it] = idog;
			//it ++;
			
			paint_points_index (image, POINT_COUNT, ptoy, cv::Scalar (0, 50, 150));
			paint_points_index (image, POINT_COUNT, ptree, cv::Scalar (0, 200, 0));
			paint_points (image, 1, ptoy + idog, cv::Scalar (255, 55, 55));
			paint_tracks (image, POINT_COUNT, ptoy, taken, cv::Scalar (200, 200, 200));
			cv::imshow ("test", image);
		}
		if (flags & NEXTACTION2)
		{
			test (POINT_COUNT, taken, ptoy, POINT_COUNT, ptree);
			image.setTo (cv::Scalar (255,255,255));
			paint_points_index (image, POINT_COUNT, ptoy, cv::Scalar (0, 50, 150));
			paint_points_index (image, POINT_COUNT, ptree, cv::Scalar (0, 200, 0));
			paint_points (image, 1, ptoy + idog, cv::Scalar (255, 55, 55));
			paint_tracks (image, POINT_COUNT, ptoy, taken, cv::Scalar (200, 200, 200));
			cv::imshow ("test", image);
		}
		*/
	}
	return 0;
}

