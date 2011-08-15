/*This is mechanically generated code*/
#include <stdlib.h>
#include "Fast.h"
#include "CaptureHandler.h"
#include <stdio.h>
#include <syslog.h>
int Fast::fast_corner_score(const uint8_t* p, const int pixel[], int t_start, bool bright)
{    
    int t_max = 255;
    int t_min = t_start;
    int t; 
    if(bright)
	while(t_min != (t_max - 1) && t_min != t_max)
	{	
	    t = (t_max + t_min) / 2;
	    m_higher_t = *p + t;
	    
	    if(!full_seg_test_bright(p, pixel))
		t_max = t;
	    else 
		t_min = t;
	}
    else
    {
	while(t_min != (t_max - 1) && t_min != t_max)
	{	
	    t = (t_max + t_min) / 2;
	    m_lower_t = *p - t;
	    
	    if(!full_seg_test_dark(p, pixel))
		t_max = t;
	    else 
		t_min = t;
	}
    }
    return t_min;
}

void Fast::make_offsets(int pixel[], int row_stride)
{
        pixel[0] = 0 + row_stride * 3;
        pixel[1] = 1 + row_stride * 3;
        pixel[2] = 2 + row_stride * 2;
        pixel[3] = 3 + row_stride * 1;
        pixel[4] = 3 + row_stride * 0;
        pixel[5] = 3 + row_stride * -1;
        pixel[6] = 2 + row_stride * -2;
        pixel[7] = 1 + row_stride * -3;
        pixel[8] = 0 + row_stride * -3;
        pixel[9] = -1 + row_stride * -3;
        pixel[10] = -2 + row_stride * -2;
        pixel[11] = -3 + row_stride * -1;
        pixel[12] = -3 + row_stride * 0;
        pixel[13] = -3 + row_stride * 1;
        pixel[14] = -2 + row_stride * 2;
        pixel[15] = -1 + row_stride * 3;
}



int* Fast::fast_score(const uint8_t* i, int stride, Pixel* corners, int num_corners, int t)
{	
	int* scores = (int*)malloc(sizeof(int)* num_corners);
	int n;

	int pixel[16];
	make_offsets(pixel, stride);

    for(n=0; n < num_corners; n++)
        scores[n] = fast_corner_score(i + corners[n].y * stride + corners[n].x, pixel, t, corners[n].bright);

	return scores;
}


Pixel* Fast::fast_detect(const uint8_t* im, int xsize, int ysize, int stride, int t, int &ret_num_corners)
{
	int num_corners = 0;
	int n_pos = 0;	
	int rsize=512;
	int pixel[16];
	int x, y;
	int pos_tr, neg_tr;
	int b_count, d_count;

	const uint8_t* p;
	
	Pixel* ret_corners;
	Pixel px;

	ret_corners = (Pixel*)malloc(sizeof(Pixel)*rsize);
	make_offsets(pixel, stride);

	for(y=3; y < ysize - 3; y++)
	    for(x=3; x < xsize - 3; x++)
	    {
		p = im + y * stride + x;
		b_count = 0;
		d_count = 0;
		pos_tr = *p + t;
		neg_tr = *p - t;

		if(p[pixel[0]] > pos_tr)
		    b_count++;
		if(p[pixel[8]] > pos_tr)
		    b_count++;
		if(b_count >= 1)
		{
		    
		    if(p[pixel[12]] > pos_tr)
			b_count++;
		    if(b_count == 1)
		    {			    
			if(p[pixel[4]] > pos_tr)
			    b_count ++;
		    }
		    if(b_count >=2)
		    {
			m_higher_t = pos_tr;
			if(full_seg_test_bright(p, pixel))
			{
			    if(num_corners == rsize)
			    {
				rsize*=2;
				ret_corners = (Pixel*)realloc(ret_corners, sizeof(Pixel)*rsize);
			    }
			    ret_corners[num_corners].x = x;
			    ret_corners[num_corners].y = y;
			    ret_corners[num_corners].bright = true;
  			    n_pos += sprintf(CaptureHandler::get_strfast() + n_pos, "%d,%d;", x, y);

 			    num_corners++;
			    continue;
			}
		    }
		}
		if(p[pixel[0]] < neg_tr)
		    d_count++;
		if(p[pixel[8]] < neg_tr)
		    d_count++;
		if(d_count >= 1)
		{
		    if(p[pixel[12]] < neg_tr)
			d_count++;
		    if(d_count == 1)
		    {			    
			if(p[pixel[4]] < neg_tr)
			    d_count ++;
		    }
		    if(d_count >= 2)
		    {
			m_lower_t = neg_tr;
			if(full_seg_test_dark(p, pixel))
			{
			    if(num_corners == rsize)
			    {
				rsize*=2;
				ret_corners = (Pixel*)realloc(ret_corners, sizeof(Pixel)*rsize);
			    }
			    ret_corners[num_corners].x = x;
			    ret_corners[num_corners].y = y;
			    ret_corners[num_corners].bright = false;
  			    n_pos += sprintf(CaptureHandler::get_strfast() + n_pos, "%d,%d;", x, y);

			    num_corners++;
			    continue;
			}
		    }
		}
	    }
// 	syslog(LOG_INFO, "Corner %d", num_corners );
	ret_num_corners = num_corners;
	return ret_corners;

}

