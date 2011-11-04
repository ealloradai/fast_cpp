#ifndef PARAM_HANDLER_H
#define PARAM_HANDLER_H

#include <param.h>
#include "Shared.h"
using namespace std;

typedef struct
{
    int    param_app;
    int    param_fast_level;
    int    param_fast_suppression;
    int	   param_res_id;
    int    param_fast;
    int    param_sobel;
    int    param_sobel_operation;
    int    param_threshold_level;
    int	   param_kmeans;
    int	   param_kmeans_partitions;
    int	   param_capture_fps;
    
    void init()
    {
	  param_app = DISABLED;
	  param_fast_level = 40;
	  param_fast_suppression = DISABLED;
	  param_res_id = 0;
	  param_fast  = DISABLED;
	  param_sobel = DISABLED;
	  param_sobel_operation = 7;
	  param_threshold_level = 128;
	  param_kmeans = DISABLED;
	  param_kmeans_partitions = 3;
	  param_capture_fps = 30;
    }
}Params;

class ParamHandler
{
public:
  ParamHandler();
  Params getParams();
  int  getAppState();
  int  getFastLevel();
  int  getFastSuppression();
  int  getFastResId();
  int  getFastState();
  int  getSobelState();
  int  getSobelOperation();
  int  getThresholdLevel();
  
  void init(char *app_name);

private:
  static param_stat handleParam1(const char *name, const char *value);
  static param_stat handleParam2(const char *name, const char *value);
  static param_stat handleParam3(const char *name, const char *value);
  static param_stat handleParam4(const char *name, const char *value);
  static param_stat handleParam5(const char *name, const char *value);
  static param_stat handleParam6(const char *name, const char *value);
  static param_stat handleParam7(const char *name, const char *value);
  static param_stat handleParam8(const char *name, const char *value);
  static param_stat handleParam9(const char *name, const char *value);
  static param_stat handleParam10(const char *name, const char *value);
  static param_stat handleParam11(const char *name, const char *value);
  
  static Params m_params;

  
  static pid_t  app_pid;
};

#endif // PARAM_HANDLER_H
