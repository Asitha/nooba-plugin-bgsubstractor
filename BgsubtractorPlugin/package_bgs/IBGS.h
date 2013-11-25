#pragma once

#include <opencv2/opencv.hpp>

class IBGS
{
public:
  virtual void process(const cv::Mat &img_input, cv::Mat &img_output) = 0;
  virtual ~IBGS(){}
    virtual void setThreshold(int t) = 0;
    virtual int getThreshold() = 0;
private:
  virtual void saveConfig() = 0;
  virtual void loadConfig() = 0;
};
