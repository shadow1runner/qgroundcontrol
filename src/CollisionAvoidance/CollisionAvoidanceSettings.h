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
	    // Load defaults from settings
        QSettings settings;
        settings.beginGroup("CollisionAvoidance");

	    _fileName                  = settings.value("FileName", "CollisionAvoidance/res/rl/go5.mp4").toString().toStdString(),
	    _outputDir                 = settings.value("OutputDir", "CollisionAvoidance/res/out/RandomCollider/cpp/frame_").toString().toStdString(),
	    _subsampleAmount           = settings.value("SubsampleAmount", 2).toInt();
	    _particles                 = settings.value("Particles", 200000).toInt();
	    _windowSize                = settings.value("WindowSize", 5).toInt();
	    _inlierProportionThreshold = settings.value("InlierProportionThreshold", 0.01).toDouble();
	    _divergencePatchSize       = settings.value("DivergencePatchSize", 20).toInt();
	    _divergenceThreshold       = settings.value("DivergenceThreshold", 0.08).toDouble();

	    settings.endGroup();
	}	

	void storeSettings() {
		// Store settings
		QSettings settings;
        settings.beginGroup("CollisionAvoidance");

		settings.setValue("FileName", QString::fromStdString(_fileName));
		settings.setValue("OutputDir", QString::fromStdString(_outputDir));
		settings.setValue("SubsampleAmount", _subsampleAmount);
		settings.setValue("Particles", _particles);
		settings.setValue("WindowSize", _windowSize);
		settings.setValue("InlierProportionThreshold", _inlierProportionThreshold);
		settings.setValue("DivergencePatchSize", _divergencePatchSize);
		settings.setValue("DivergenceThreshold", _divergenceThreshold);

		settings.endGroup();
	}

public:
	const std::string getFileName() const                   { return _fileName;                  }
	const std::string getOutputDir() const                  { return _outputDir;                 }
	int               getSubsampleAmount() const            { return _subsampleAmount;           }
	int               getParticles() const                  { return _particles;                 }
	int               getWindowSize() const                 { return _windowSize;                }
	int               getInlierProportionThreshold() const  { return _inlierProportionThreshold; }
	int               getDivergencePatchSize() const        { return _divergencePatchSize;       }
	double            getDivergenceThreshold() const        { return _divergenceThreshold;       }

private:
	std::string _fileName;
	std::string _outputDir;
	int         _subsampleAmount;
	int         _particles;
	int         _windowSize;
	int         _inlierProportionThreshold;
	int         _divergencePatchSize;
	double      _divergenceThreshold;

};

#endif // COLLISIONAVOIDANCESETTINGS

