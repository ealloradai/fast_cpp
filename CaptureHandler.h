#ifndef CAPTURE_HANDLER_H
#define CAPTURE_HANDLER_H

#include <capture.h>
#include "Fast.h"
#include "Sobel.h"
#include "ThreadHandler.h"
#include "ParamHandler.h"

using namespace std;

class CaptureHandler
{
public:
  CaptureHandler(Fast & f, Sobel &s);
  void  handle(int exit_signal, Params params);
  void open(int res_id);
  void close();
  static char* get_strfast();
  static char* get_strsobel();
  ~CaptureHandler();

private:
  int parse_window(const char *window, int *x1, int *x2, int *y1, int *y2);

  char* m_fast_param;
  char* m_sobel_param;
  
  Fast fast;
  Sobel sobel;
  ThreadHandler *threadHandler;
  media_stream *stream;
  static char* str_corners;
  static char* str_sobel;
};

#endif // CAPTURE_HANDLER_H
