#ifndef COLLISIONAVOIDANCE_COLLISIONAVOIDANCESETTINGSCONTROLLER_H
#define COLLISIONAVOIDANCE_COLLISIONAVOIDANCESETTINGSCONTROLLER_H

#include <QObject>

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

   Q_PROPERTY(bool useRecordedVideoInsteadOfDevice READ getUseRecordedVideoInsteadOfDevice WRITE setUseRecordedVideoInsteadOfDevice NOTIFY useRecordedVideoInsteadOfDeviceChanged);
   Q_PROPERTY(QString fileName                     READ getFileName WRITE setFileName NOTIFY fileNameChanged);
   Q_PROPERTY(int device                           READ getDevice WRITE setDevice NOTIFY deviceChanged);
   Q_PROPERTY(bool writeToOutputDirEnabled         READ getWriteToOutputEnabled WRITE setWriteToOutputEnabled NOTIFY writeToOutputEnabledChanged);
   Q_PROPERTY(QString outputDir                    READ getOutputDir WRITE setOutputDir NOTIFY outputDirChanged);
   Q_PROPERTY(bool writeRawFrames                  READ getWriteRawFrames WRITE setWriteRawFrames NOTIFY writeRawFramesChanged);
   Q_PROPERTY(QString rawFramesDir                 READ getRawFramesDir WRITE setRawFramesDir NOTIFY rawFramesDirChanged);
   Q_PROPERTY(int subsampleAmount                  READ getSubsampleAmount WRITE setSubsampleAmount NOTIFY subsampleAmountChanged);
   Q_PROPERTY(int particles                        READ getParticles WRITE setParticles NOTIFY particlesChanged);
   Q_PROPERTY(int windowSize                       READ getWindowSize WRITE setWindowSize NOTIFY windowSizeChanged);
   Q_PROPERTY(bool inlierProportionThresholdEnabled     READ getInlierProportionThresholdEnabled WRITE setInlierProportionThresholdEnabled NOTIFY inlierProportionThresholdEnabledChanged);
   Q_PROPERTY(double inlierProportionThreshold     READ getInlierProportionThreshold WRITE setInlierProportionThreshold NOTIFY inlierProportionThresholdChanged);
   Q_PROPERTY(int divergencePatchSize              READ getDivergencePatchSize WRITE setDivergencePatchSize NOTIFY divergencePatchSizeChanged);
   Q_PROPERTY(double divergenceThreshold           READ getDivergenceThreshold WRITE setDivergenceThreshold NOTIFY divergenceThresholdChanged);

    bool              getUseRecordedVideoInsteadOfDevice() const  { return _settings.getUseRecordedVideoInsteadOfDevice(); }
    const QString     getFileName() const                         { return _settings.getFileName();                  }
    int               getDevice() const                           { return _settings.getDevice();                    }
    bool              getWriteToOutputEnabled() const             { return _settings.getWriteToOutputEnabled();      }
    const QString     getOutputDir() const                        { return _settings.getOutputDir();                 }
    bool              getWriteRawFrames() const                   { return _settings.getWriteRawFrames();            }
    const QString     getRawFramesDir() const                     { return _settings.getRawFramesDir();              }
    int               getSubsampleAmount() const                  { return _settings.getSubsampleAmount();           }
    int               getParticles() const                        { return _settings.getParticles();                 }
    int               getWindowSize() const                       { return _settings.getWindowSize();                }
    bool              getInlierProportionThresholdEnabled() const { return _settings.getInlierProportionThresholdEnabled(); }
    double            getInlierProportionThreshold() const        { return _settings.getInlierProportionThreshold(); }
    int               getDivergencePatchSize() const              { return _settings.getDivergencePatchSize();       }
    double            getDivergenceThreshold() const              { return _settings.getDivergenceThreshold();       }

signals:
    void useRecordedVideoInsteadOfDeviceChanged(bool value);
    void fileNameChanged(const QString value);
    void deviceChanged(int value);
    void writeToOutputEnabledChanged(bool value);
    void outputDirChanged(const QString value);
    void writeRawFramesChanged(bool value);
    void rawFramesDirChanged(const QString value);
    void subsampleAmountChanged(int value);
    void particlesChanged(int value);
    void windowSizeChanged(int value);
    void inlierProportionThresholdEnabledChanged(bool value);
    void inlierProportionThresholdChanged(double value);
    void divergencePatchSizeChanged(int value);
    void divergenceThresholdChanged(double value);

public slots:
    void setUseRecordedVideoInsteadOfDevice(bool useRecordedVideoInsteadOfDevice) { _settings.setUseRecordedVideoInsteadOfDevice(useRecordedVideoInsteadOfDevice); emit useRecordedVideoInsteadOfDeviceChanged(useRecordedVideoInsteadOfDevice); _settings.storeSettings();                  }
    void setFileName(QString fileName)                                            { _settings.setFileName(fileName); emit fileNameChanged(fileName); _settings.storeSettings();                                   }
    void setWriteToOutputEnabled(bool writeToOutputEnabled)                       { _settings.setWriteToOutputEnabled(writeToOutputEnabled); emit writeToOutputEnabledChanged(writeToOutputEnabled); _settings.storeSettings();     }
    void setOutputDir(QString  outputDir)                                         { _settings.setOutputDir(outputDir); emit outputDirChanged(outputDir); _settings.storeSettings();                                }
    void setWriteRawFrames(bool writeRawFrames)                                   { _settings.setWriteRawFrames(writeRawFrames); emit writeRawFramesChanged(writeRawFrames); _settings.storeSettings();     }
    void setRawFramesDir(QString rawFramesDir)                                    { _settings.setRawFramesDir(rawFramesDir); emit rawFramesDirChanged(rawFramesDir); _settings.storeSettings();     }
    void setDevice(int device)                                                    { _settings.setDevice(device); emit deviceChanged(device); _settings.storeSettings();                                       }
    void setSubsampleAmount(int subsampleAmount)                                  { _settings.setSubsampleAmount(subsampleAmount); emit subsampleAmountChanged(subsampleAmount); _settings.storeSettings();                     }
    void setParticles(int particles)                                              { _settings.setParticles(particles); emit particlesChanged(particles); _settings.storeSettings();                                  }
    void setWindowSize(int windowSize)                                            { _settings.setWindowSize(windowSize); emit windowSizeChanged(windowSize); _settings.storeSettings();                               }
    void setInlierProportionThresholdEnabled(bool value)                          { _settings.setInlierProportionThresholdEnabled(value); emit inlierProportionThresholdEnabledChanged(value); _settings.storeSettings(); }
    void setInlierProportionThreshold(double inlierProportionThreshold)           { _settings.setInlierProportionThreshold(inlierProportionThreshold); emit inlierProportionThresholdChanged(inlierProportionThreshold); _settings.storeSettings(); }
    void setDivergencePatchSize(int divergencePatchSize)                          { _settings.setDivergencePatchSize(divergencePatchSize); emit divergencePatchSizeChanged(divergencePatchSize); _settings.storeSettings();             }
    void setDivergenceThreshold(double divergenceThreshold)                       { _settings.setDivergenceThreshold(divergenceThreshold); emit divergenceThresholdChanged(divergenceThreshold); _settings.storeSettings();        }

private:
    CollisionAvoidanceSettings& _settings;
    
};

#endif // COLLISIONAVOIDANCE_COLLISIONAVOIDANCES fETTINGSCONTROLLER_H
