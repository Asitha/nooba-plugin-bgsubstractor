#pragma once

#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>

#include "IBGS.h"
#include "QDir"
class FrameDifferenceBGS : public IBGS
{
private:
  bool firstTime;
  cv::Mat img_input_prev;
  cv::Mat img_foreground;
  bool enableThreshold;
  int threshold;
  bool showOutput;

public:
  FrameDifferenceBGS();
  ~FrameDifferenceBGS();

  void process(const cv::Mat &img_input, cv::Mat &img_output);
  inline void setThreshold(int t){threshold = t;}
  inline int getThreshold(){return threshold;}
private:
  void saveConfig();
  void loadConfig();
};
