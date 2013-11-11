#ifndef BGSUBTRACTORPLUGIN_H
#define BGSUBTRACTORPLUGIN_H

#include "bgsubtractorplugin_global.h"
#include "noobapluginapi.h"

#include <QObject>

#include "package_bgs/StaticFrameDifferenceBGS.h"

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
//    void onIntParamChanged(const QString& varName, int val);
//    void onDoubleParamChanged(const QString& varName, double val);
//    void onStringParamChanged(const QString& varName, const QString& val);
//    void onMultiValParamChanged(const QString& varName, const QString& val);

    void inputData(const PluginPassData& data);

private:
    StaticFrameDifferenceBGS bgs;
    cv::Mat img_mask;

};

#endif // BGSUBTRACTORPLUGIN_H
