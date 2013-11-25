#pragma once

#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>

#include "IBGS.h"
#include "QDir"

class WeightedMovingMeanBGS : public IBGS
{
private:
  bool firstTime;
  cv::Mat img_input_prev_1;
  cv::Mat img_input_prev_2;
  bool enableWeight;
  bool enableThreshold;
  int threshold;
  bool showOutput;
  bool showBackground;

public:
  WeightedMovingMeanBGS();
  ~WeightedMovingMeanBGS();

  void process(const cv::Mat &img_input, cv::Mat &img_output);
  inline void setThreshold(int t){threshold = t;}
  inline int getThreshold(){return threshold;}
private:
  void saveConfig();
  void loadConfig();
};

