#pragma once

#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/video/background_segm.hpp>

#include "IBGS.h"
#include "QDir"
class MixtureOfGaussianV1BGS : public IBGS
{
private:
  bool firstTime;
  cv::BackgroundSubtractorMOG mog;
  cv::Mat img_foreground;
  double alpha;
  bool enableThreshold;
  int threshold;
  bool showOutput;

public:
  MixtureOfGaussianV1BGS();
  ~MixtureOfGaussianV1BGS();

  void process(const cv::Mat &img_input, cv::Mat &img_output);
  inline void setThreshold(int t){threshold = t;}
  inline int getThreshold(){return threshold;}
private:
  void saveConfig();
  void loadConfig();
};

