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
        settings.beginGroup("QGC_COLLISION_AVOIDANCE");

	    _fileName                  = settings.value("FILENAME", "CollisionAvoidance/res/rl/go5.mp4").toString().toStdString(),
	    _outputDir                 = settings.value("OUTPUT_DIR", "CollisionAvoidance/res/out/RandomCollider/cpp/frame_").toString().toStdString(),
	    _subsampleAmount           = settings.value("SUBSAMPLE_AMOUNT", 2).toInt();
	    _particles                 = settings.value("PARTICLES", 200000).toInt();
	    _windowSize                = settings.value("WINDOW_SIZE", 5).toInt();
	    _inlierProportionThreshold = settings.value("INLIER_PROPORTION_THRESHOLD", 0.01).toDouble();
	    _divergencePatchSize       = settings.value("DIVERGENCE_PATCHSIZE", 20).toInt();
	    _divergenceThreshold       = settings.value("DIVERGENCE_THRESHOLD", 0.08).toDouble();

	    settings.endGroup();
	}	

	void storeSettings() {
		// Store settings
		QSettings settings;
        settings.beginGroup("QGC_COLLISION_AVOIDANCE");

		settings.setValue("FILENAME", QString::fromStdString(_fileName));
		settings.setValue("OUTPUT_DIR", QString::fromStdString(_outputDir));
		settings.setValue("SUBSAMPLE_AMOUNT", _subsampleAmount);
		settings.setValue("PARTICLES", _particles);
		settings.setValue("WINDOW_SIZE", _windowSize);
		settings.setValue("INLIER_PROPORTION_THRESHOLD", _inlierProportionThreshold);
		settings.setValue("DIVERGENCE_PATCHSIZE", _divergencePatchSize);
		settings.setValue("DIVERGENCE_THRESHOLD", _divergenceThreshold);
		
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

