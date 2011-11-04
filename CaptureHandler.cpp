#include <stdio.h>
#include <stdlib.h>
#include <syslog.h>
#include <capture.h>
#include <string>
#include <param.h>
#include <time.h>

#include "CaptureHandler.h"
#include "Fast.h"
#include "Sobel.h"
#include "Shared.h"

#define CAPTURE_PROPERTIES    "sdk_format=Y800&resolution=160x120&fps=30"
#define SIZEOF_PROPERTIES     1024

char* CaptureHandler::str_corners;
char* CaptureHandler::str_sobel;
char* CaptureHandler::str_kmeans;
Profiler CaptureHandler::profiler;
Kmeans CaptureHandler::m_kmeans;

Kmeans* CaptureHandler::get_kmeans()
{
    Kmeans* ptr_kmeans = &m_kmeans;
    return ptr_kmeans;
}
char* CaptureHandler::get_strfast()
{
    return str_corners; 
}
char* CaptureHandler::get_strsobel()
{
    return str_sobel;
}
char* CaptureHandler::get_strkmeans()
{
    return str_kmeans;
}
Profiler CaptureHandler::get_profiler()
{
    return profiler;
}

CaptureHandler::CaptureHandler(Fast &f, Sobel &s)
{
    fast = f;
    sobel = s;
    stream   = NULL;
    str_corners = (char*)malloc(SIZEOF_APP_CORNER_COORD);
    str_sobel = (char*)malloc(SIZEOF_APP_SOBEL);
    str_kmeans = (char*)malloc(SIZEOF_APP_KMEANS);
    profiler.reset();
}

CaptureHandler::~CaptureHandler()
{
    close();  
    free(str_corners);
    free(str_sobel);
    param_free(m_fast_param);
    param_free(m_sobel_param);
}

void
CaptureHandler::close()
{
    if (stream != NULL) 
    {
	capture_close_stream(stream);
	stream = NULL;
    }
}

void
CaptureHandler::handle(int exit_signal, Params params)
{
  media_frame *frame = NULL;
  uint8_t *    data  = NULL;
  int          width;
  int          height;
  int          stride;
  int          result = 0;
  int	       num_corner;
  
  time_t start = time(NULL);
  frame = capture_get_frame(stream);
  if (!frame) 
  {
      if (exit_signal) 
      {
	  return;
      }
      else 
      {
	  syslog(LOG_CRIT, "Failed to capture frame!");
	  close();
	  exit(EXIT_FAILURE);
      }
  }

  strcpy(str_corners, "");
  strcpy(str_sobel, "");
  strcpy(str_kmeans, "");
  data   = (uint8_t*)capture_frame_data(frame);
  width  = capture_frame_width(frame);
  height = capture_frame_height(frame);
  stride = capture_frame_stride(frame);
  profiler.m_time_grab += ((time(NULL) - start)*1000000 - profiler.m_time_grab) / 500;
//   if(threadHandler->is_running())
//     pthread_mutex_lock(threadHandler->get_mutex_a());
  time_t mid = time(NULL);
  if(params.param_sobel)
  {
      sobel.sobel(data, width, height, stride, params.param_sobel_operation, params.param_threshold_level);
//       profiler.m_time_edge += ((time(NULL) - mid)*1000000 - profiler.m_time_edge) / 500;
      profiler.m_time_edge = (time(NULL) - mid)*1000000;
  }

  if(params.param_fast)
  {
      fast.fast_detect_nonmax(data, width, height, stride, params.param_fast_level, num_corner, params.param_fast_suppression, params.param_kmeans, params.param_kmeans_partitions);
      fast.get_time_corner(profiler.m_time_corner);
      fast.get_time_supp(profiler.m_time_supp);
      fast.get_time_kmeans(profiler.m_time_kmeans);
      profiler.num_corner = num_corner;
  }
  
//   profiler.m_time_proc += ((time(NULL) - start)*1000000 - profiler.m_time_proc) / 500;
    profiler.m_time_proc = (time(NULL) - start)*1000000;
  if(threadHandler->is_running())
    pthread_mutex_unlock(threadHandler->get_mutex_b());
    
  capture_frame_free(frame);
}

/* Capture */

void CaptureHandler::open(int res_id, int fps)
{
    char     properties[SIZEOF_PROPERTIES];
    const char*   RES[5] = {"160x120", "240x180", "320x240", "480x360", "640x480"};	
    const int 	  WIDTH[5] = {160, 240, 320, 480, 640};
    const int 	  HEIGHT[5] = {120, 180, 240, 360, 480};
    char     FPS[20];		        
//     string capture_properies = "sdk_format=Y800&resolution"+RES+"&fps=30";
    snprintf(FPS, sizeof(FPS), "%d", fps);
    snprintf(properties,
           sizeof(properties),
           "sdk_format=Y800");
 
    strncat(properties,
            "&resolution=",
            sizeof(properties) - 1);
    strncat(properties,
            RES[res_id],
            sizeof(properties) - 1);
    strncat(properties,
            "&fps=",
            sizeof(properties) - 1);
        strncat(properties,
            FPS,
            sizeof(properties) - 1);
    
    
//     snprintf(properties, SIZEOF_PROPERTIES, CAPTURE_PROPERTIES);

    stream = capture_open_stream(IMAGE_UNCOMPRESSED, properties);
    if (!stream)
    {
	syslog(LOG_CRIT, "Failed to open capture stream!");
	exit(EXIT_FAILURE);
    }
    syslog(LOG_INFO, "Stream Opened!");
    m_kmeans.kmeans_init(WIDTH[res_id], HEIGHT[res_id]);
}
