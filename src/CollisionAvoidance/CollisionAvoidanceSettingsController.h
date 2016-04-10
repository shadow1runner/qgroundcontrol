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
    {
    }

    ~CollisionAvoidanceSettingsController() {
       settings.storeSettings();
    }

   Q_PROPERTY(bool useRecordedVideoInsteadOfDevice READ getUseRecordedVideoInsteadOfDevice WRITE setUseRecordedVideoInsteadOfDevice NOTIFY useRecordedVideoInsteadOfDeviceChanged);
   Q_PROPERTY(QString fileName                     READ getFileName WRITE setFileName NOTIFY fileNameChanged);
   Q_PROPERTY(int device                           READ getDevice WRITE setDevice NOTIFY deviceChanged);
   Q_PROPERTY(QString outputDir                    READ getOutputDir WRITE setOutputDir NOTIFY outputDirChanged);
   Q_PROPERTY(int subsampleAmount                  READ getSubsampleAmount WRITE setSubsampleAmount NOTIFY subsampleAmountChanged);
   Q_PROPERTY(int particles                        READ getParticles WRITE setParticles NOTIFY particlesChanged);
   Q_PROPERTY(int windowSize                       READ getWindowSize WRITE setWindowSize NOTIFY windowSizeChanged);
   Q_PROPERTY(int inlierProportionThreshold        READ getInlierProportionThreshold WRITE setInlierProportionThreshold NOTIFY inlierProportionThresholdChanged);
   Q_PROPERTY(int divergencePatchSize              READ getDivergencePatchSize WRITE setDivergencePatchSize NOTIFY divergencePatchSizeChanged);
   Q_PROPERTY(double divergenceThreshold           READ getDivergenceThreshold WRITE setDivergenceThreshold NOTIFY divergenceThresholdChanged);

    bool              getUseRecordedVideoInsteadOfDevice() const  { return settings.getUseRecordedVideoInsteadOfDevice(); }
    const QString     getFileName() const                         { return settings.getFileName();                  }
    int               getDevice() const                           { return settings.getDevice();                    }
    const QString     getOutputDir() const                        { return settings.getOutputDir();                 }
    int               getSubsampleAmount() const                  { return settings.getSubsampleAmount();           }
    int               getParticles() const                        { return settings.getParticles();                 }
    int               getWindowSize() const                       { return settings.getWindowSize();                }
    int               getInlierProportionThreshold() const        { return settings.getInlierProportionThreshold(); }
    int               getDivergencePatchSize() const              { return settings.getDivergencePatchSize();       }
    double            getDivergenceThreshold() const              { return settings.getDivergenceThreshold();       }

signals:
    void useRecordedVideoInsteadOfDeviceChanged(bool value);
    void fileNameChanged(const QString value);
    void deviceChanged(int value);
    void outputDirChanged(const QString value);
    void subsampleAmountChanged(int value);
    void particlesChanged(int value);
    void windowSizeChanged(int value);
    void inlierProportionThresholdChanged(int value);
    void divergencePatchSizeChanged(int value);
    void divergenceThresholdChanged(double value);

public slots:
    void setUseRecordedVideoInsteadOfDevice(bool useRecordedVideoInsteadOfDevice) { settings.setUseRecordedVideoInsteadOfDevice(useRecordedVideoInsteadOfDevice); emit useRecordedVideoInsteadOfDeviceChanged(useRecordedVideoInsteadOfDevice); settings.storeSettings();                  }
    void setFileName(QString fileName)                                            { settings.setFileName(fileName); emit fileNameChanged(fileName); settings.storeSettings();                                   }
    void setDevice(int device)                                                    { settings.setDevice(device); emit deviceChanged(device); settings.storeSettings();                                       }
    void setOutputDir(QString  outputDir)                                         { settings.setOutputDir(outputDir); emit outputDirChanged(outputDir); settings.storeSettings();                                }
    void setSubsampleAmount(int subsampleAmount)                                  { settings.setSubsampleAmount(subsampleAmount); emit subsampleAmountChanged(subsampleAmount); settings.storeSettings();                     }
    void setParticles(int particles)                                              { settings.setParticles(particles); emit particlesChanged(particles); settings.storeSettings();                                  }
    void setWindowSize(int windowSize)                                            { settings.setWindowSize(windowSize); emit windowSizeChanged(windowSize); settings.storeSettings();                               }
    void setInlierProportionThreshold(int inlierProportionThreshold)              { settings.setInlierProportionThreshold(inlierProportionThreshold); emit inlierProportionThresholdChanged(inlierProportionThreshold); settings.storeSettings(); }
    void setDivergencePatchSize(int divergencePatchSize)                          { settings.setDivergencePatchSize(divergencePatchSize); emit divergencePatchSizeChanged(divergencePatchSize); settings.storeSettings();             }
    void setDivergenceThreshold(double divergenceThreshold)                       { settings.setDivergenceThreshold(divergenceThreshold); emit divergenceThresholdChanged(divergenceThreshold); settings.storeSettings();        }

private:
    CollisionAvoidanceSettings settings;
    
};

#endif // COLLISIONAVOIDANCE_COLLISIONAVOIDANCES fETTINGSCONTROLLER_H
