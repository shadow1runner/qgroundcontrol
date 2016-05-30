#ifndef COLLISIONAVOIDANCE_COLLISIONAVOIDANCESETTINGSCONTROLLER_H
#define COLLISIONAVOIDANCE_COLLISIONAVOIDANCESETTINGSCONTROLLER_H

#include <QObject>
#include <QDir>

#include "CollisionAvoidanceSettings.h"

class CollisionAvoidanceSettingsController : public QObject
{
    Q_OBJECT

public:
    CollisionAvoidanceSettingsController(QObject* parent = NULL)
        : QObject(parent)
        , _settings(CollisionAvoidanceSettings::getInstance())
    {
    }

    ~CollisionAvoidanceSettingsController() {
       _settings.storeSettings();
    }

   Q_PROPERTY(bool useRecordedVideoInsteadOfDevice  READ getUseRecordedVideoInsteadOfDevice WRITE setUseRecordedVideoInsteadOfDevice NOTIFY useRecordedVideoInsteadOfDeviceChanged);
   Q_PROPERTY(QString fileName                      READ getFileName WRITE setFileName NOTIFY fileNameChanged);
   Q_PROPERTY(int device                            READ getDevice WRITE setDevice NOTIFY deviceChanged);
   Q_PROPERTY(int rawFrameRotation                  READ getRawFrameRotation WRITE setRawFrameRotation NOTIFY rawFrameRotationChanged);

   Q_PROPERTY(int subsampleAmount                   READ getSubsampleAmount WRITE setSubsampleAmount NOTIFY subsampleAmountChanged);
   Q_PROPERTY(int particles                         READ getParticles WRITE setParticles NOTIFY particlesChanged);
   Q_PROPERTY(int windowSize                        READ getWindowSize WRITE setWindowSize NOTIFY windowSizeChanged);
   Q_PROPERTY(bool inlierProportionThresholdEnabled READ getInlierProportionThresholdEnabled WRITE setInlierProportionThresholdEnabled NOTIFY inlierProportionThresholdEnabledChanged);
   Q_PROPERTY(double inlierProportionThreshold      READ getInlierProportionThreshold WRITE setInlierProportionThreshold NOTIFY inlierProportionThresholdChanged);
   Q_PROPERTY(int divergencePatchSize               READ getDivergencePatchSize WRITE setDivergencePatchSize NOTIFY divergencePatchSizeChanged);
   Q_PROPERTY(double divergenceThreshold            READ getDivergenceThreshold WRITE setDivergenceThreshold NOTIFY divergenceThresholdChanged);
   Q_PROPERTY(double avgDivergenceThreshold         READ getAvgDivergenceThreshold WRITE setAvgDivergenceThreshold NOTIFY avgDivergenceThresholdChanged);
   Q_PROPERTY(int divergenceHistoryBufferSize       READ getDivergenceHistoryBufferSize WRITE setDivergenceHistoryBufferSize NOTIFY divergenceHistoryBufferSizeChanged);
   Q_PROPERTY(bool undistortFrames                  READ getUndistortFrames WRITE setUndistortFrames NOTIFY undistortFramesChanged);
   Q_PROPERTY(QString ocamModelPath                 READ getOcamModelPath WRITE setOcamModelPath NOTIFY ocamModelPathChanged);

   Q_PROPERTY(bool displayBadFramesInUi                   READ getDisplayBadFramesInUi WRITE setDisplayBadFramesInUi NOTIFY displayBadFramesInUiChanged);
   Q_PROPERTY(double opticalFlowVectorVisualizationFactor READ getOpticalFlowVectorVisualizationFactor WRITE setOpticalFlowVectorVisualizationFactor NOTIFY opticalFlowVectorVisualizationFactorChanged);

   Q_PROPERTY(bool clearOldFramesEnabled            READ getClearOldFramesEnabled WRITE setClearOldFramesEnabled NOTIFY clearOldFramesEnabledChanged);
   Q_PROPERTY(bool writeBadFrames                   READ getWriteBadFrames WRITE setWriteBadFrames NOTIFY writeBadFramesChanged);
   Q_PROPERTY(bool writeGoodFrames                  READ getWriteGoodFrames WRITE setWriteGoodFrames NOTIFY writeGoodFramesChanged);
   Q_PROPERTY(bool writeHistogramFrames             READ getWriteHistogramFrames WRITE setWriteHistogramFrames NOTIFY writeHistogramFramesChanged);
   Q_PROPERTY(bool writeOpticalFlowFrames           READ getWriteOpticalFlowFrames WRITE setWriteOpticalFlowFrames NOTIFY writeOpticalFlowFramesChanged);
   Q_PROPERTY(bool writeRawFrames                   READ getWriteRawFrames WRITE setWriteRawFrames NOTIFY writeRawFramesChanged);
   Q_PROPERTY(bool writeToOutputEnabled             READ getWriteToOutputEnabled WRITE setWriteToOutputEnabled NOTIFY writeToOutputEnabledChanged);
   Q_PROPERTY(bool writeUiFrames                    READ getWriteUiFrames WRITE setWriteUiFrames NOTIFY writeUiFramesChanged);
   Q_PROPERTY(QString badFramesPath                 READ getBadFramesPath WRITE setBadFramesPath NOTIFY badFramesPathChanged);
   Q_PROPERTY(QString goodFramesPath                READ getGoodFramesPath WRITE setGoodFramesPath NOTIFY goodFramesPathChanged);
   Q_PROPERTY(QString histogramFramesPath           READ getHistogramFramesPath WRITE setHistogramFramesPath NOTIFY histogramFramesPathChanged);
   Q_PROPERTY(QString opticalFlowFramesPath         READ getOpticalFlowFramesPath WRITE setOpticalFlowFramesPath NOTIFY opticalFlowFramesPathChanged);
   Q_PROPERTY(QString rawFramesPath                 READ getRawFramesPath WRITE setRawFramesPath NOTIFY rawFramesPathChanged);
   Q_PROPERTY(QString uiFramesPath                  READ getUiFramesPath WRITE setUiFramesPath NOTIFY uiFramesPathChanged);

   Q_PROPERTY(QString csvFilePath                  READ getCsvFilePath WRITE setCsvFilePath NOTIFY csvFilePathChanged);

    bool              getUseRecordedVideoInsteadOfDevice() const  { return _settings.getUseRecordedVideoInsteadOfDevice(); }
    const QString     getFileName() const                         { return _settings.getFileName();                  }
    int               getDevice() const                           { return _settings.getDevice();                    }
    int               getRawFrameRotation() const                 { return _settings.RawFrameRotation;           }
    int               getSubsampleAmount() const                  { return _settings.SubsampleAmount;           }
    int               getParticles() const                        { return _settings.Particles;                 }
    int               getWindowSize() const                       { return _settings.WindowSize;                }
    bool              getInlierProportionThresholdEnabled() const { return _settings.InlierProportionThresholdEnabled; }
    double            getInlierProportionThreshold() const        { return _settings.InlierProportionThreshold; }
    int               getDivergencePatchSize() const              { return _settings.DivergencePatchSize;       }
    double            getDivergenceThreshold() const              { return _settings.DivergenceThreshold;       }
    double            getAvgDivergenceThreshold() const           { return _settings.AvgDivergenceThreshold;       }
    int               getDivergenceHistoryBufferSize() const      { return _settings.DivergenceHistoryBufferSize;       }

    bool              getUndistortFrames() const { return _settings.UndistortFrames; }
    const QString     getOcamModelPath() const   { return _settings.OcamModelPath; }

    bool              getDisplayBadFramesInUi() const { return _settings.DisplayBadFramesInUi; }
    double            getOpticalFlowVectorVisualizationFactor() const { return _settings.OpticalFlowVectorVisualizationFactor; }

    bool              getClearOldFramesEnabled() const            { return _settings.ClearOldFramesEnabled; }
    bool              getWriteBadFrames() const                   { return _settings.WriteBadFrames; }
    bool              getWriteGoodFrames() const                  { return _settings.WriteGoodFrames; }
    bool              getWriteHistogramFrames() const             { return _settings.WriteHistogramFrames; }
    bool              getWriteOpticalFlowFrames() const           { return _settings.WriteOpticalFlowFrames; }
    bool              getWriteRawFrames() const                   { return _settings.WriteRawFrames; }
    bool              getWriteToOutputEnabled() const             { return _settings.WriteToOutputEnabled; }
    bool              getWriteUiFrames() const                    { return _settings.WriteUiFrames; }
    const QString     getBadFramesPath() const                    { return _settings.BadFramesPath; }
    const QString     getGoodFramesPath() const                   { return _settings.GoodFramesPath; }
    const QString     getHistogramFramesPath() const              { return _settings.HistogramFramesPath; }
    const QString     getOpticalFlowFramesPath() const            { return _settings.OpticalFlowFramesPath; }
    const QString     getRawFramesPath() const                    { return _settings.RawFramesPath; }
    const QString     getUiFramesPath() const                     { return _settings.UiFramesPath; }

    const QString     getCsvFilePath() const                     { return _settings.CsvFilePath; }

signals:
  void useRecordedVideoInsteadOfDeviceChanged(bool value);
  void fileNameChanged(QString value);
  void deviceChanged(int value);
  void rawFrameRotationChanged(int value);
  void subsampleAmountChanged(int value);
  void particlesChanged(int value);
  void windowSizeChanged(int value);
  void inlierProportionThresholdEnabledChanged(bool value);
  void inlierProportionThresholdChanged(double value);
  void divergencePatchSizeChanged(int value);
  void divergenceThresholdChanged(double value);
  void avgDivergenceThresholdChanged(double value);
  void divergenceHistoryBufferSizeChanged(int value);

  void undistortFramesChanged(bool value);
  void ocamModelPathChanged(QString value);

  void displayBadFramesInUiChanged(bool value);
  void opticalFlowVectorVisualizationFactorChanged(double value);

  void clearOldFramesEnabledChanged(bool value);
  void writeBadFramesChanged(bool value);
  void writeGoodFramesChanged(bool value);
  void writeHistogramFramesChanged(bool value);
  void writeOpticalFlowFramesChanged(bool value);
  void writeRawFramesChanged(bool value);
  void writeToOutputEnabledChanged(bool value);
  void writeUiFramesChanged(bool value);
  void badFramesPathChanged(QString value);
  void goodFramesPathChanged(QString value);
  void histogramFramesPathChanged(QString value);
  void opticalFlowFramesPathChanged(QString value);
  void rawFramesPathChanged(QString value);
  void uiFramesPathChanged(QString value);

  void csvFilePathChanged(QString value);

public slots:
    void setUseRecordedVideoInsteadOfDevice(bool value)     { _settings.setUseRecordedVideoInsteadOfDevice(value); _settings.storeSettings(); emit useRecordedVideoInsteadOfDeviceChanged(value); }
    void setFileName(QString value)                         { _settings.setFileName(value); _settings.storeSettings(); emit fileNameChanged(value); }
    void setDevice(int value)                               { _settings.setDevice(value); _settings.storeSettings(); emit deviceChanged(value); }
    void setRawFrameRotation(int value)                     { _settings.RawFrameRotation = value; _settings.storeSettings(); emit rawFrameRotationChanged(value); }
    void setSubsampleAmount(int value)                      { _settings.SubsampleAmount = value; _settings.storeSettings(); emit subsampleAmountChanged(value); }
    void setParticles(int value)                            { _settings.Particles = value; _settings.storeSettings(); emit particlesChanged(value); }
    void setWindowSize(int value)                           { _settings.WindowSize = value; _settings.storeSettings(); emit windowSizeChanged(value); }
    void setInlierProportionThresholdEnabled(bool value)    { _settings.InlierProportionThresholdEnabled = value; _settings.storeSettings(); emit inlierProportionThresholdEnabledChanged(value); }
    void setInlierProportionThreshold(double value)         { _settings.InlierProportionThreshold = value; _settings.storeSettings(); emit inlierProportionThresholdChanged(value); }
    void setDivergencePatchSize(int value)                  { _settings.DivergencePatchSize = value; _settings.storeSettings(); emit divergencePatchSizeChanged(value); }
    void setDivergenceThreshold(double value)               { _settings.DivergenceThreshold = value; _settings.storeSettings();emit divergenceThresholdChanged(value); }
    void setAvgDivergenceThreshold(double value)            { _settings.AvgDivergenceThreshold = value; _settings.storeSettings();emit avgDivergenceThresholdChanged(value); }
    void setDivergenceHistoryBufferSize(int value)          { _settings.DivergenceHistoryBufferSize = value; _settings.storeSettings(); emit divergenceHistoryBufferSizeChanged(value); }
    void setUndistortFrames(bool value)  { _settings.UndistortFrames = value; _settings.storeSettings(); emit undistortFramesChanged(value); }
    void setOcamModelPath(QString value) { _settings.OcamModelPath = value; _settings.storeSettings(); emit ocamModelPathChanged(value); }

    void setDisplayBadFramesInUi(bool value)   { _settings.DisplayBadFramesInUi = value; _settings.storeSettings(); emit displayBadFramesInUiChanged(value); }
    void setOpticalFlowVectorVisualizationFactor(double value) { _settings.OpticalFlowVectorVisualizationFactor = value; _settings.storeSettings(); emit opticalFlowVectorVisualizationFactorChanged(value); }
    
    void setClearOldFramesEnabled(bool value)    { _settings.ClearOldFramesEnabled = value; _settings.storeSettings(); emit clearOldFramesEnabledChanged(value); }
    void setWriteBadFrames(bool value)           { _settings.WriteBadFrames = value; _settings.storeSettings(); emit writeBadFramesChanged(value); }
    void setWriteGoodFrames(bool value)          { _settings.WriteGoodFrames = value; _settings.storeSettings(); emit writeGoodFramesChanged(value); }
    void setWriteHistogramFrames(bool value)     { _settings.WriteHistogramFrames = value; _settings.storeSettings(); emit writeHistogramFramesChanged(value); }
    void setWriteOpticalFlowFrames(bool value)   { _settings.WriteOpticalFlowFrames = value; _settings.storeSettings(); emit writeOpticalFlowFramesChanged(value); }
    void setWriteRawFrames(bool value)           { _settings.WriteRawFrames = value; _settings.storeSettings(); emit writeRawFramesChanged(value); }
    void setWriteToOutputEnabled(bool value)     { _settings.WriteToOutputEnabled = value; _settings.storeSettings(); emit writeToOutputEnabledChanged(value); }
    void setWriteUiFrames(bool value)            { _settings.WriteUiFrames = value; _settings.storeSettings(); emit writeUiFramesChanged(value); }
    void setBadFramesPath(QString value)         { _settings.BadFramesPath = value; _settings.storeSettings(); emit badFramesPathChanged(value);  }
    void setGoodFramesPath(QString value)        { _settings.GoodFramesPath = value; _settings.storeSettings(); emit goodFramesPathChanged(value);  }
    void setHistogramFramesPath(QString value)   { _settings.HistogramFramesPath = value; _settings.storeSettings(); emit histogramFramesPathChanged(value);  }
    void setOpticalFlowFramesPath(QString value) { _settings.OpticalFlowFramesPath = value; _settings.storeSettings(); emit opticalFlowFramesPathChanged(value);  }
    void setRawFramesPath(QString value)         { _settings.RawFramesPath = value; _settings.storeSettings(); emit rawFramesPathChanged(value);  }
    void setUiFramesPath(QString value)          { _settings.UiFramesPath = value; _settings.storeSettings(); emit uiFramesPathChanged(value);  }

    void setCsvFilePath(QString value)           { _settings.CsvFilePath = value; _settings.storeSettings(); emit csvFilePathChanged(value);  }

private:
    CollisionAvoidanceSettings& _settings;
};

#endif // COLLISIONAVOIDANCE_COLLISIONAVOIDANCES fETTINGSCONTROLLER_H
