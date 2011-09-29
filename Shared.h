#ifndef SHARED_H
#define SHARED_H

#include <time.h>
#define RSIZE 512

#define DISABLED           0
#define ENABLED            1

#define SIZEOF_APP_CORNER_COORD  131072
#define SIZEOF_APP_SOBEL  	 1048576
#define SIZEOF_PKG		 2048

#define PARAM_ENABLED      "Enabled"
#define PARAM_LEVEL        "Level"
#define PARAM_SUPPRESSION  "Suppression"
#define PARAM_RES	   "Resolution"
#define PARAM_FAST  	   "Fast"
#define PARAM_SOBEL	   "Sobel"


enum FAST_STATUS {
  FAST_IDLE,
  FAST_START,
  FAST_RUNNING,
  FAST_STOP,
  FAST_RECONF
};
#endif //SHARED_H
