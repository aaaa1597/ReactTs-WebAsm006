#include <opencv2/opencv.hpp>
#include "../src/MainProcess.h"

int main(int argc, char *argv[]) {
  CreateAruCoMarker();
  DetectAruCoMarker();
  return 0;
}