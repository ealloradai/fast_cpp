#include <stdlib.h>
#include "Fast.h"
#include "syslog.h"
#include "CaptureHandler.h"

Fast::Fast()
{
    m_kmeans = CaptureHandler::get_kmeans();
    m_time_corner = 0;
    m_time_supp = 0;
    m_time_kmeans = 0;
    m_filter_s = 500;
}
void Fast::fast_detect_nonmax(const uint8_t* im, const int& xsize, const int& ysize, const int &stride, const int &b, int& ret_num_corners, int suppression, int kmeans, int kmeans_k)
{
    int num_corners;
	
    if(suppression)
    {
	Pixel* corners;
	Pixel* nonmax;
	int* scores;
	
	corners = fast_detect(im, xsize, ysize, stride, b, ret_num_corners, kmeans, kmeans_k);
	
// 	m_kmeans.kmeans_init(xsize, ysize);
// 	m_kmeans.kmeans_execute(corners, ret_num_corners);
	
	num_corners = ret_num_corners;
	time_t startsup = time(NULL);
	scores = fast_score(im, stride, corners, num_corners, b);
	nonmax = nonmax_suppression(corners, scores, num_corners, ret_num_corners);
// 	m_time_supp += ((time(NULL) - startsup)*1000000 - m_time_supp) / m_filter_s;
	m_time_supp = ((time(NULL) - startsup)*1000000);
	free(scores);
	free(nonmax);
	
	free(corners);
    }
    else
	fast_detect_nosup(im, xsize, ysize, stride, b, ret_num_corners, kmeans, kmeans_k);
 	

// 	ret_num_corners =  num_corners;
}

bool Fast::full_seg_test_bright(const uint8_t* p, const int pixel[])
{
    int pix_count = 1;
    int ptr = 1;
    if(p[pixel[0]] > m_higher_t)
    {
	while(p[pixel[ptr]] > m_higher_t)
	{	
	    if(++pix_count >= 9)
	      return true;
	
	    ptr ++;
	}
    
	ptr = 15;
	//CounterClockWise
	while(p[pixel[ptr]] > m_higher_t)
	{
	    if(++pix_count >= 9)
	      return true;
	
	    ptr--;
	}
    }
    if(p[pixel[8]] > m_higher_t)
    {
	pix_count = 1;
	ptr = 9;
	//ClockWise
	while(p[pixel[ptr]] > m_higher_t)
	{	
	    if(++pix_count >= 9)
	      return true;
	    
	    ptr = (ptr + 1) % 16;
	}
	
	ptr = 7;
	//CounterClockWise
	while(p[pixel[ptr]] > m_higher_t)
	{
	    if(++pix_count >= 9)
	      return true;
	    
	    ptr > 0 ? ptr -- : ptr = 15;
	}
    }  
    return false;
}
  
bool Fast::full_seg_test_dark(const uint8_t* p, const int pixel[])
{
    int pix_count = 1;
    int ptr = 1;
    if(p[pixel[0]] < m_lower_t)
    {
	while(p[pixel[ptr]] < m_lower_t)
	{	
	    if(++pix_count >= 9)
	      return true;
	
	    ptr ++;
	}
    
	ptr = 15;
	//CounterClockWise
	while(p[pixel[ptr]] < m_lower_t)
	{
	    if(++pix_count >= 9)
	      return true;
	
	    ptr--;
	}
    }
    if(p[pixel[8]] < m_lower_t)
    {
	pix_count = 1;
	ptr = 9;
	//ClockWise
	while(p[pixel[ptr]] < m_lower_t)
	{	
	    if(++pix_count >= 9)
	      return true;
	    
	    ptr = (ptr + 1) % 16;
	}
	
	ptr = 7;
	//CounterClockWise
	while(p[pixel[ptr]] < m_lower_t)
	{
	    if(++pix_count >= 9)
	      return true;
	    
	    ptr > 0 ? ptr -- : ptr = 15;
	}
    }  
    return false;
}

// Pixel* Fast::fast10_detect_nonmax(const uint8_t* im, int xsize, int ysize, int stride, int b, int* ret_num_corners)
// {
// 	Pixel* corners;
// 	int num_corners;
// 	int* scores;
// 	Pixel* nonmax;
// 
// 	corners = fast10_detect(im, xsize, ysize, stride, b, &num_corners);
// 	scores = fast10_score(im, stride, corners, num_corners, b);
// 	nonmax = nonmax_suppression(corners, scores, num_corners, ret_num_corners);
// 
// 	free(corners);
// 	free(scores);
// 
// 	return nonmax;
// }
// 
// Pixel* Fast::fast11_detect_nonmax(const uint8_t* im, int xsize, int ysize, int stride, int b, int* ret_num_corners)
// {
// 	Pixel* corners;
// 	int num_corners;
// 	int* scores;
// 	Pixel* nonmax;
// 
// 	corners = fast11_detect(im, xsize, ysize, stride, b, &num_corners);
// 	scores = fast11_score(im, stride, corners, num_corners, b);
// 	nonmax = nonmax_suppression(corners, scores, num_corners, ret_num_corners);
// 
// 	free(corners);
// 	free(scores);
// 
// 	return nonmax;
// }
// 
// Pixel* Fast::fast12_detect_nonmax(const uint8_t* im, int xsize, int ysize, int stride, int b, int* ret_num_corners)
// {
// 	Pixel* corners;
// 	int num_corners;
// 	int* scores;
// 	Pixel* nonmax;
// 
// 	corners = fast12_detect(im, xsize, ysize, stride, b, &num_corners);
// 	scores = fast12_score(im, stride, corners, num_corners, b);
// 	nonmax = nonmax_suppression(corners, scores, num_corners, ret_num_corners);
// 
// 	free(corners);
// 	free(scores);
// 
// 	return nonmax;
// }