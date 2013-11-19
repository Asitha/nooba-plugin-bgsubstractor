#pragma once

#include <opencv/cv.h>

class IBGS
{
public:
  virtual void process(const cv::Mat &img_input, cv::Mat &img_output) = 0;
  virtual ~IBGS(){}
    virtual void setThreshold(int t);
    virtual int getThreshold();
private:
  virtual void saveConfig() = 0;
  virtual void loadConfig() = 0;
};
