#ifndef PARAM_HANDLER_H
#define PARAM_HANDLER_H

#include <param.h>

using namespace std;

class ParamHandler
{
public:
  ParamHandler();
  int  getFastLevel();
  int  getFastState();
  int  getFastSuppression();
  void init(char *app_name);

private:
  static param_stat handleParam1(const char *name, const char *value);
  static param_stat handleParam2(const char *name, const char *value);
  static param_stat handleParam3(const char *name, const char *value);

  static int   param_fast;
  static int   param_fast_level;
  static int   param_fast_suppression;
  static pid_t app_pid;
};

#endif // PARAM_HANDLER_H
