#include "bgsubtractorplugin.h"
#include <QtCore>
#include <opencv2/core/core.hpp>
#include <QtGui/QImage>

BgsubtractorPlugin::BgsubtractorPlugin()
{

}

BgsubtractorPlugin::~BgsubtractorPlugin()
{

}

bool BgsubtractorPlugin::procFrame( const cv::Mat &in, cv::Mat &out, ProcParams &params )
{


    return true;
}

bool BgsubtractorPlugin::init()
{
    return true;
}

bool BgsubtractorPlugin::release()
{
    return true;
}

PluginInfo BgsubtractorPlugin::getPluginInfo() const
{
    PluginInfo pluginInfo(
        "Background Subtractor Plugin",
        0,
        1,
        "This provides an interface for BGS library PluginPassData will contain an image",
        "Chathuranga Hettiarachchi");
    return pluginInfo;
}

void BgsubtractorPlugin::inputData(const PluginPassData& data){

    PluginPassData eventData;
    QImage temp = data.getImage();

    cv::Mat in(temp.height(),temp.width(),CV_8UC3,(uchar*)temp.bits(),temp.bytesPerLine());
    cv::cvtColor(in, in,CV_BGR2RGB); // make convert colort to BGR !

    img_mask = in.clone();
    // bgs->process(...) method internally shows the foreground mask image
    bgs.process(in, img_mask);
    cv::Mat element = cv::getStructuringElement(cv::MORPH_RECT,cv::Size(2,2));
    cv::dilate(img_mask,img_mask, element,cv::Point(-1,-1),1);
    cv::erode(img_mask,img_mask, element,cv::Point(-1,-1),2);

    if (img_mask.channels()== 1){
        QImage img((uchar*)img_mask.data, img_mask.cols, img_mask.rows, img_mask.step1(), QImage::Format_RGB32);
        eventData.setImage(img);
        outputData(eventData);
    }
    else{
        QImage img((uchar*)img_mask.data, img_mask.cols, img_mask.rows, img_mask.step1(), QImage::Format_Indexed8);
        eventData.setImage(img);
        outputData(eventData);
    }
}


// see qt4 documentation for details on the macro (Qt Assistant app)
// Mandatory  macro for plugins in qt4. Made obsolete in qt5
#if QT_VERSION < 0x050000
    Q_EXPORT_PLUGIN2(BgsubtractorPlugin, BgsubtractorPlugin);
#endif
