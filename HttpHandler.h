#ifndef HTTP_HANDLER_H
#define HTTP_HANDLER_H

#include <net_http.h>

using namespace std;

class HttpHandler
{
public:
  HttpHandler();
  int  init(char *app_name);
  void closeHttp();
  int  waitForFastStart(int param_fast);
  int  checkForFastStopOrReconf(int param_fast);

  ~HttpHandler();
    void get_strfast();

private:
  static void handle_main(const char *method, const char *path, const http_options *options, int fd);
  static void handle_streaming_jpg(const char *method, const char *path, const http_options *options, int fd);
  static void handle_corners(const char *method, const char *path, const http_options *options, int fd);
};

#endif // HTTP_HANDLER_H
