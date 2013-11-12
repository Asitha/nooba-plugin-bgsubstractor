#include "bgsubtractorplugin.h"
#include <QtCore>
#include <QtGui/QImage>

#include <opencv2/core/core.hpp>

#include "package_bgs/StaticFrameDifferenceBGS.h"
#include "package_bgs/MixtureOfGaussianV2BGS.h"
#include "package_bgs/WeightedMovingMeanBGS.h"

BgsubtractorPlugin::BgsubtractorPlugin():
    BGSMethod_Param(tr("Method Used")),
    StaticFrameDiff_BGS("sfd"),
    MixtureOfGaussian_BGS("mog"),
    WeightedMovingMean_BGS("wmm"),
    activeBGSName(StaticFrameDiff_BGS)
{
    bgs = new StaticFrameDifferenceBGS();
}

BgsubtractorPlugin::~BgsubtractorPlugin()
{
    release();
}

bool BgsubtractorPlugin::procFrame( const cv::Mat &in, cv::Mat &out, ProcParams &params )
{
    Q_UNUSED(params)

    PluginPassData eventData;
    cv::cvtColor(in, in, CV_RGB2BGR);
    process(in, img_mask);
    out = img_mask;
    cv::cvtColor(in, in, CV_BGR2RGB);

    eventData.setImage(convertToQImage(out));
    emit outputData(eventData);

    return true;
}

bool BgsubtractorPlugin::init()
{
    setActiveBGS(activeBGSName);
    createMultiValParam(BGSMethod_Param, QStringList() << StaticFrameDiff_BGS
                        << WeightedMovingMean_BGS << MixtureOfGaussian_BGS);
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
    cv::cvtColor(in, in,CV_RGB2BGR); // make convert colort to BGR !

    process(in, img_mask);

    if (img_mask.channels()== 1){
        QImage img((uchar*)img_mask.data, img_mask.cols, img_mask.rows, img_mask.step1(), QImage::Format_Indexed8);
        eventData.setImage(img);
    }
    else{
        QImage img((uchar*)img_mask.data, img_mask.cols, img_mask.rows, img_mask.step1(), QImage::Format_RGB888);
        eventData.setImage(img);
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
        debugMsg(tr("%1 Not implemented").arg(bgsName));
        //bgs = new MixtureOfGaussianV2BGS();
    }
    else if(bgsName.compare(WeightedMovingMean_BGS) == 0)
    {
        debugMsg(tr("%1 Not implemented").arg(bgsName));
        //bgs = new WeightedMovingMeanBGS();
    }
    //activeBGSName = bgsName;
}

QImage BgsubtractorPlugin::convertToQImage(cv::Mat &cvImg)
{
    return QImage((const unsigned char*)(cvImg.data),
                cvImg.cols,cvImg.rows,cvImg.step,  QImage::Format_RGB888);
}

void BgsubtractorPlugin::process(const cv::Mat &in, cv::Mat& out)
{
    if(!bgs)
        return;

    bgs->process(in, out);
    cv::Mat element = cv::getStructuringElement(cv::MORPH_RECT,cv::Size(2,2));
    cv::dilate(out,out, element,cv::Point(-1,-1),1);
    cv::erode(out,out, element,cv::Point(-1,-1),2);
}


// see qt4 documentation for details on the macro (Qt Assistant app)
// Mandatory  macro for plugins in qt4. Made obsolete in qt5
#if QT_VERSION < 0x050000
    Q_EXPORT_PLUGIN2(BgsubtractorPlugin, BgsubtractorPlugin);
#endif
