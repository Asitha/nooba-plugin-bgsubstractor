#include "bgsubtractorplugin.h"
#include <QtCore>
#include <QtGui/QImage>

#include <opencv2/core/core.hpp>

#include "package_bgs/StaticFrameDifferenceBGS.h"
#include "package_bgs/MixtureOfGaussianV2BGS.h"
#include "package_bgs/WeightedMovingMeanBGS.h"
#include "package_bgs/AdaptiveBackgroundLearning.h"


BgsubtractorPlugin::BgsubtractorPlugin():
    BGSMethod_Param(tr("Background Subtractor")),
    StaticFrameDiff_BGS("Static Frame Difference"),
    MixtureOfGaussian_BGS("Mixture of GaussianV2"),
    WeightedMovingMean_BGS("WeightedMovingMean"),
    AdaptiveBackgroundLearning_BGS("AdaptiveBackgroundLearning"),
    activeBGSName(StaticFrameDiff_BGS),
    dilation_rounds(1),
    erosion_rounds(2),
    threshold(15)
{
    bgs = new StaticFrameDifferenceBGS();
    threshold = bgs->getThreshold();
}

BgsubtractorPlugin::~BgsubtractorPlugin()
{
    release();
}

bool BgsubtractorPlugin::procFrame( const cv::Mat &in, cv::Mat &out, ProcParams &params )
{
    Q_UNUSED(params)
    cv::Mat input;
    cv::Mat output;
    in.copyTo(input);

    //cv::cvtColor(in, in, CV_RGB2BGR);
    process(in, img_mask);
    out = img_mask;
    img_mask.copyTo(output);
    //cv::cvtColor(in, in, CV_BGR2RGB);
    QList<QImage> images;
    QStringList strings;

    images.append(convertToQImage(input));
    images.append(convertToQImage(output));
    emit outputData(strings,images);
    return true;
}

bool BgsubtractorPlugin::init()
{
    setActiveBGS(activeBGSName);
    createMultiValParam(BGSMethod_Param, QStringList()
                        << StaticFrameDiff_BGS
                        << WeightedMovingMean_BGS
                        << MixtureOfGaussian_BGS
                        << AdaptiveBackgroundLearning_BGS
                        );

    createIntParam("dilation_rounds",1,10,0);
    createIntParam("erosion_rounds",1,10,0);
    createIntParam("threshold",bgs->getThreshold(),255,0);

    createFrameViewer("BG Mask");
    return true;
}

bool BgsubtractorPlugin::release()
{
    img_mask.release();
    delete bgs;
    bgs = NULL;
    return true;
}

PluginInfo BgsubtractorPlugin::getPluginInfo() const
{
    PluginInfo pluginInfo(
        "Background Subtractor Plugin",
        0,
        2,
        "This provides an interface for BGS library PluginPassData will contain an image",
        "Chathuranga Hettiarachchi, Asitha Nanayakkara");
    return pluginInfo;
}

void BgsubtractorPlugin::inputData(const PluginPassData& data){

    PluginPassData eventData;
    QImage temp = data.getImage();

    cv::Mat in(temp.height(),temp.width(),CV_8UC3,(uchar*)temp.bits(),temp.bytesPerLine());
    //cv::cvtColor(in, in,CV_RGB2BGR); // make convert colort to BGR !

    process(in, img_mask);
    //cv::cvtColor(in, img_mask,CV_BGR2GRAY);
    //in.copyTo(img_mask);
    if (img_mask.channels()== 1){
        QImage img((uchar*)img_mask.data, img_mask.cols, img_mask.rows, img_mask.step1(), QImage::Format_Indexed8);
        eventData.setImage(img);
        updateFrameViewer("BG Mask",img);
    }
    else{
        QImage img((uchar*)img_mask.data, img_mask.cols, img_mask.rows, img_mask.step1(), QImage::Format_RGB888);
        eventData.setImage(img);
        updateFrameViewer("BG Mask",img);
    }

    emit outputData(eventData);
}

void BgsubtractorPlugin::onMultiValParamChanged(const QString &varName, const QString &val)
{
    if(varName.compare(BGSMethod_Param) == 0)
    {
        setActiveBGS(val);
    }
}

void BgsubtractorPlugin::setActiveBGS(const QString& bgsName)
{
//    if(bgsName.compare(activeBGSName) == 0)
//        return;

    delete bgs;
    bgs = NULL;
    img_mask = cv::Mat();

    if(bgsName.compare(StaticFrameDiff_BGS) == 0)
    {
        bgs = new StaticFrameDifferenceBGS();
    }
    else if(bgsName.compare(MixtureOfGaussian_BGS) == 0)
    {
        bgs = new MixtureOfGaussianV2BGS();
    }
    else if(bgsName.compare(WeightedMovingMean_BGS) == 0)
    {
        bgs = new WeightedMovingMeanBGS();
    }
    else if(bgsName.compare(AdaptiveBackgroundLearning_BGS) == 0)
    {
        bgs = new AdaptiveBackgroundLearning();
    }
    bgs->setThreshold(threshold);
    //activeBGSName = bgsName;
}

QImage BgsubtractorPlugin::convertToQImage(const cv::Mat &cvImg)
{
    if (cvImg.channels()== 1){
        QImage img((uchar*)cvImg.data, cvImg.cols, cvImg.rows, cvImg.step1(), QImage::Format_Indexed8);
        return img;
    }
    else{
        QImage img((uchar*)cvImg.data, cvImg.cols, cvImg.rows, cvImg.step1(), QImage::Format_RGB888);
        return img;
    }
}

void BgsubtractorPlugin::process(const cv::Mat &in, cv::Mat& out)
{
    if(!bgs)
        return;

    bgs->process(in, out);
    cv::Mat element = cv::getStructuringElement(cv::MORPH_RECT,cv::Size(2,2));
    cv::dilate(out,out, element,cv::Point(-1,-1),dilation_rounds);
    cv::erode(out,out, element,cv::Point(-1,-1),erosion_rounds);
}

void BgsubtractorPlugin::onIntParamChanged(const QString& varName, int val)
{
    if(varName == "dilation_rounds"){

        dilation_rounds = val;

        debugMsg("dilation_rounds set to "  + QString("%1").arg(val));
    }
    else if(varName == "erosion_rounds"){

        erosion_rounds = val;
        debugMsg("erosion_rounds set to "  + QString("%1").arg(val));
    }
    else if(varName == "threshold"){

        threshold = val;
        bgs->setThreshold(val);
        debugMsg("threshold set to "  + QString("%1").arg(val));
    }
}


// see qt4 documentation for details on the macro (Qt Assistant app)
// Mandatory  macro for plugins in qt4. Made obsolete in qt5
#if QT_VERSION < 0x050000
    Q_EXPORT_PLUGIN2(BgsubtractorPlugin, BgsubtractorPlugin);
#endif
