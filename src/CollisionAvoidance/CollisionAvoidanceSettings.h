#ifndef COLLISIONAVOIDANCESETTINGS
#define COLLISIONAVOIDANCESETTINGS

#include <string>

#include <QSettings>
#include <QDebug>

class CollisionAvoidanceSettings
{
public:
    CollisionAvoidanceSettings() 
    {
		loadSettings();
	}

	~CollisionAvoidanceSettings() 
	{
		storeSettings();
	}

public:
	void loadSettings() {
        QSettings settings;
        settings.beginGroup("CollisionAvoidance");

	    _useRecordedVideoInsteadOfDevice = settings.value("UseRecordedVideoInsteadOfDevice", false).toBool(),
	    _fileName                        = settings.value("FileName", "CollisionAvoidance/res/rl/go5.mp4").toString(),
	    _device                          = settings.value("Device", 0).toInt(),
	    _outputDir                       = settings.value("OutputDir", "CollisionAvoidance/res/out/RandomCollider/cpp/frame_").toString(),
	    _subsampleAmount                 = settings.value("SubsampleAmount", 2).toInt();
	    _particles                       = settings.value("Particles", 200000).toInt();
	    _windowSize                      = settings.value("WindowSize", 5).toInt();
	    _inlierProportionThreshold       = settings.value("InlierProportionThreshold", 0.01).toDouble();
	    _divergencePatchSize             = settings.value("DivergencePatchSize", 20).toInt();
	    _divergenceThreshold             = settings.value("DivergenceThreshold", 0.08).toDouble();

	    settings.endGroup();
	}	

	void storeSettings() {
		QSettings settings;
        settings.beginGroup("CollisionAvoidance");

		settings.setValue("UseRecordedVideoInsteadOfDevice", _useRecordedVideoInsteadOfDevice);
		settings.setValue("FileName", _fileName);
		settings.setValue("Device", _device);
		settings.setValue("OutputDir", _outputDir);
		settings.setValue("SubsampleAmount", _subsampleAmount);
		settings.setValue("Particles", _particles);
		settings.setValue("WindowSize", _windowSize);
		settings.setValue("InlierProportionThreshold", _inlierProportionThreshold);
		settings.setValue("DivergencePatchSize", _divergencePatchSize);
		settings.setValue("DivergenceThreshold", _divergenceThreshold);

		settings.endGroup();
	}

public:
	bool 		      getUseRecordedVideoInsteadOfDevice() const  { return _useRecordedVideoInsteadOfDevice; }
	const QString     getFileName() const                   	  { return _fileName;                  }
	int  			  getDevice() const                     	  { return _device;                    }
	const QString     getOutputDir() const                  	  { return _outputDir;                 }
	int               getSubsampleAmount() const            	  { return _subsampleAmount;           }
	int               getParticles() const                  	  { return _particles;                 }
	int               getWindowSize() const                 	  { return _windowSize;                }
	int               getInlierProportionThreshold() const  	  { return _inlierProportionThreshold; }
	int               getDivergencePatchSize() const        	  { return _divergencePatchSize;       }
	double            getDivergenceThreshold() const        	  { return _divergenceThreshold;       }

	void setUseRecordedVideoInsteadOfDevice(bool useRecordedVideoInsteadOfDevice)                    { _useRecordedVideoInsteadOfDevice = useRecordedVideoInsteadOfDevice;                  }
	void setFileName(QString fileName)                           { _fileName = fileName;                                   }
	void setDevice(int device)                                       { _device = device;                                       }
	void setOutputDir(QString  outputdir)                        { _outputDir  = outputdir;                                }
	void setSubsampleAmount(int subsampleamount)                     { _subsampleAmount = subsampleamount;                     }
    void setParticles(int particles)                                 { _particles = particles;                                  }
	void setWindowSize(int windowsize)                               { _windowSize = windowsize;              				   }
	void setInlierProportionThreshold(int inlierProportionThreshold) { _inlierProportionThreshold = inlierProportionThreshold; }
	void setDivergencePatchSize(int divergencePatchSize)             { _divergencePatchSize = divergencePatchSize;             }
    void setDivergenceThreshold(double divergenceThreshold)          { _divergenceThreshold = divergenceThreshold;        }

private:
	bool _useRecordedVideoInsteadOfDevice;
	QString _fileName;
	int  		_device;
	QString _outputDir;
	int         _subsampleAmount;
	int         _particles;
	int         _windowSize;
	int         _inlierProportionThreshold;
	int         _divergencePatchSize;
	double      _divergenceThreshold;

};

#endif // COLLISIONAVOIDANCESETTINGS

