#ifndef BGSUBTRACTORPLUGIN_H
#define BGSUBTRACTORPLUGIN_H

#include "bgsubtractorplugin_global.h"
#include "noobapluginapi.h"
#include <opencv/cv.h>
#include <QObject>

class IBGS;

class BGSUBTRACTORPLUGIN_EXPORT BgsubtractorPlugin: public NoobaPluginAPI
{
    Q_OBJECT
    Q_INTERFACES(NoobaPluginAPI)
#if QT_VERSION >= 0x050000
    Q_PLUGIN_METADATA(IID "nooba.plugins.qt5.bgsubtractor-plugin" FILE "bgsubtractorPlugin.json")
#endif

public:
    BgsubtractorPlugin();
    ~BgsubtractorPlugin();

    bool procFrame(const cv::Mat &in, cv::Mat &out, ProcParams &params);
    bool init();
    bool release();
    PluginInfo getPluginInfo() const;

public slots:
    //void onMultiValParamChanged(const QString &varName, const QString &val);

    /**
     * These functions will be called when the parameters are changed by the
     * user.
     */
    void onIntParamChanged(const QString& varName, int val);
//    void onDoubleParamChanged(const QString& varName, double val);
//    void onStringParamChanged(const QString& varName, const QString& val);
//    void onMultiValParamChanged(const QString& varName, const QString& val);

    void inputData(const PluginPassData& data);
    void onMultiValParamChanged(const QString &varName, const QString &val);
private:

    void setActiveBGS(const QString &bgsName);
    /*
     \brief get a color QImage from cv::Mat
     \return QImage with three color channels
     */
    inline QImage convertToQImage(const cv::Mat &cvImg);

    inline void process(const cv::Mat &in, cv::Mat& out);
    IBGS                *bgs;
    const QString       BGSMethod_Param;
    const QString       StaticFrameDiff_BGS;
    const QString       MixtureOfGaussianV1_BGS;
    const QString       MixtureOfGaussianV2_BGS;
    const QString       WeightedMovingMean_BGS;
    const QString       AdaptiveBackgroundLearning_BGS;
    const QString       WeightedMovingVariance_BGS;
    const QString       FrameDifference_BGS;
    const QString       GMG_BGS;


    cv::Mat             img_mask;
    QString             activeBGSName;
    int dilation_rounds;
    int erosion_rounds;
    int threshold;

};

#endif // BGSUBTRACTORPLUGIN_H
