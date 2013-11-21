#pragma once

#include <iostream>
#include <opencv2/opencv.hpp>


#include "IBGS.h"
#include "QDir"
class GMG : public IBGS
{
private:

  bool firstTime;
  cv::Ptr<cv::BackgroundSubtractor> fgbg;
  int initializationFrames;
  double decisionThreshold;
  cv::Mat img_foreground;
  cv::Mat img_segmentation;
  bool showOutput;

public:
  GMG();
  ~GMG();

  void process(const cv::Mat &img_input, cv::Mat &img_output);
  inline void setThreshold(int t){decisionThreshold = t;}
  inline int getThreshold(){return decisionThreshold;}
private:
  void saveConfig();
  void loadConfig();
};

