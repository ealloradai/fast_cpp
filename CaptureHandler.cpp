#include <stdio.h>
#include <stdlib.h>
#include <syslog.h>
#include <capture.h>
#include <string>
#include <param.h>

#include "CaptureHandler.h"
#include "Fast.h"
#include "Sobel.h"
#include "Shared.h"

#define CAPTURE_PROPERTIES    "sdk_format=Y800&resolution=160x120&fps=30"
#define SIZEOF_PROPERTIES     1024

char* CaptureHandler::str_corners;
char* CaptureHandler::str_sobel;
char* CaptureHandler::get_strfast()
{
    return str_corners; 
}
char* CaptureHandler::get_strsobel()
{
    return str_sobel;
}
CaptureHandler::CaptureHandler(Fast &f, Sobel &s)
{
    fast = f;
    sobel = s;
    stream   = NULL;
    str_corners = (char*)malloc(SIZEOF_APP_CORNER_COORD);
    str_sobel = (char*)malloc(SIZEOF_APP_SOBEL);
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
CaptureHandler::handle(int exit_signal, int fast_level, int suppression, int fast_state, int sobel_state)
{
  media_frame *frame = NULL;
  uint8_t *    data  = NULL;
  int          width;
  int          height;
  int          stride;
  int          result = 0;
  int	       num_corner;
  
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
  data   = (uint8_t*)capture_frame_data(frame);
  width  = capture_frame_width(frame);
  height = capture_frame_height(frame);
  stride = capture_frame_stride(frame);
  
//   if(threadHandler->is_running())
//     pthread_mutex_lock(threadHandler->get_mutex_a());
  if(sobel_state)
      sobel.sobel(data, width, height, stride);

  if(fast_state)
      fast.fast_detect_nonmax(data, width, height, stride, fast_level, num_corner, suppression);
  
  if(threadHandler->is_running())
    pthread_mutex_unlock(threadHandler->get_mutex_b());
    
  capture_frame_free(frame);
}

/* Capture */

void CaptureHandler::open(int res_id)
{
    char     properties[SIZEOF_PROPERTIES];
    const char*   RES[3] = {"160x120", "240x180", "320x240"};								
		        
//     string capture_properies = "sdk_format=Y800&resolution"+RES+"&fps=30";
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
            "&fps=20",
            sizeof(properties) - 1);
    
//     snprintf(properties, SIZEOF_PROPERTIES, CAPTURE_PROPERTIES);

    stream = capture_open_stream(IMAGE_UNCOMPRESSED, properties);
    if (!stream)
    {
	syslog(LOG_CRIT, "Failed to open capture stream!");
	exit(EXIT_FAILURE);
    }
    syslog(LOG_INFO, "Stream Opened!");
}
