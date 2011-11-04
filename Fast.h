#ifndef FAST_H
#define FAST_H
#include "Shared.h"
#include "Pixel.h"
#include "Kmeans.h"

#include <string.h>
#include <time.h>

using namespace std;

typedef unsigned char uint8_t;

class Fast
{
protected:
  //int m_pix_count;
  int m_lower_t;
  int m_higher_t;
  int m_n;
  int frame_count;
  
  Kmeans* m_kmeans;
  
  int m_time_corner;
  int m_time_supp;
  int m_time_kmeans;
  int m_filter_s;
  
  void make_offsets(int pixel[], int row_stride);

  bool zero_dark(const uint8_t* p, const int pixel[]);
  bool zero_bright(const uint8_t* p, const int pixel[]);
  bool eight_dark(const uint8_t* p, const int pixel[]);
  bool eight_bright(const uint8_t* p, const int pixel[]);
  
public:
    Fast();
    ~Fast(){}

     int fast_corner_score(const uint8_t* p, const int pixel[], int t_start, bool bright);
//     int fast9_corner_score(const uint8_t* p, const int pixel[], int bstart, bool bright);
//     int fast10_corner_score(const uint8_t* p, const int pixel[], int bstart);
//     int fast11_corner_score(const uint8_t* p, const int pixel[], int bstart);
//     int fast12_corner_score(const uint8_t* p, const int pixel[], int bstart);

     Pixel* fast_detect(const uint8_t* im, int xsize, int ysize, int stride, int b, int &ret_num_corners, int kmeans, int kmeans_k);
//     Pixel* fast9_detect(const uint8_t* im, int xsize, int ysize, int stride, int b, int* ret_num_corners);
//     Pixel* fast10_detect(const uint8_t* im, int xsize, int ysize, int stride, int b, int* ret_num_corners);
//     Pixel* fast11_detect(const uint8_t* im, int xsize, int ysize, int stride, int b, int* ret_num_corners);
//     Pixel* fast12_detect(const uint8_t* im, int xsize, int ysize, int stride, int b, int* ret_num_corners);

     void fast_detect_nosup(const uint8_t* im, int xsize, int ysize, int stride, int t, int &ret_num_corners,int kmeans, int kmeans_k);
     
     int *fast_score(const uint8_t* i, int stride, Pixel* corners, int num_corners, int b);
//     int* fast9_score(const uint8_t* i, int stride, Pixel* corners, int num_corners, int b);
//     int* fast10_score(const uint8_t* i, int stride, Pixel* corners, int num_corners, int b);
//     int* fast11_score(const uint8_t* i, int stride, Pixel* corners, int num_corners, int b);
//     int* fast12_score(const uint8_t* i, int stride, Pixel* corners, int num_corners, int b);

     void fast_detect_nonmax(const uint8_t* im, const int &xsize, const int &ysize, const int &stride, const int &b, int& ret_num_corners, int suppression, int kmeans, int kmeans_k);
//     Pixel* fast9_detect_nonmax(const uint8_t* im, int xsize, int ysize, int stride, int b, int& ret_num_corners);
//     Pixel* fast10_detect_nonmax(const uint8_t* im, int xsize, int ysize, int stride, int b, int* ret_num_corners);
//     Pixel* fast11_detect_nonmax(const uint8_t* im, int xsize, int ysize, int stride, int b, int* ret_num_corners);
//     Pixel* fast12_detect_nonmax(const uint8_t* im, int xsize, int ysize, int stride, int b, int* ret_num_corners);

     Pixel* nonmax_suppression(const Pixel* corners, const int* scores, int num_corners, int &ret_num_nonmax);
    
     bool full_seg_test_bright(const uint8_t* p, const int pixel[]);
     bool full_seg_test_dark(const uint8_t* p, const int pixel[]);
     
     inline void get_time_corner(int& time_corner)
     {
	time_corner = m_time_corner;
     }
     inline void get_time_supp(int& time_supp)
     {
	time_supp = m_time_supp;
     }
     inline void get_time_kmeans(int& time_kmeans)
     {
	time_kmeans = m_time_kmeans;
     }
    };

#endif // FAST_H
