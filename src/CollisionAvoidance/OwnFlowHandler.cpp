#include "OwnFlowHandler.h"

#include <QSettings>

#include "QGCToolbox.h"
#include "QtHelper.h"

OwnFlowHandler::OwnFlowHandler(QGCApplication* app)
    : QGCTool(app)
    , _collisionAvoidanceDataProvider(nullptr)
{
	loadSettings();
    QtHelper::registerMetaTypes();
    frame_grabber = new hw::BufferedFrameGrabber(FILENAME, 1, [](cv::Mat input) {return input;});

    converter = new hw::Converter(SUBSAMPLE_AMOUNT);
    converter->moveToThread(&converterThread);

    ownFlow = new hw::OwnFlow(PARTICLES, WINDOW_SIZE);

    ownFlow->moveToThread(&ownFlowThread);
    QObject::connect(converter, &hw::Converter::imageConverted,
                     ownFlow, &hw::OwnFlow::processImage,
                     // Qt::DirectConnection); // can't be use - IPC
                     // Qt::QueuedConnection);
//                      Qt::AutoConnection); // does NOT work, most likely because OwnFlow gets overwhelmed
                     Qt::BlockingQueuedConnection); // works
                     // Qt::UniqueConnection);
                     // https://doc.qt.io/qt-5/qt.html#ConnectionType-enum
                     // https://wiki.qt.io/Threads_Events_QObjects
}

OwnFlowHandler::~OwnFlowHandler()
{
	stop();
    storeSettings();
    delete frame_grabber;
    delete converter;
    delete ownFlow;
}

void OwnFlowHandler::start()
{
    converterThread.start();
    ownFlowThread.start();

    // initialize baseFrame
    converter->convertImage(frame_grabber->next());

    int i = 0;
    while(i<3 && frame_grabber->has_next()) {
        auto currentFrame = frame_grabber->next();
        converter->convertImage(currentFrame);
        //ownFlow.processImage(currentFrame);

        // a.processEvents();
        ++i;
    }
}

void OwnFlowHandler::stop()
{
    converterThread.quit();
    converterThread.wait();

    ownFlowThread.quit();
    ownFlowThread.wait();
}

void OwnFlowHandler::setToolbox(QGCToolbox* toolbox)
{
	QGCTool::setToolbox(toolbox);
    _collisionAvoidanceDataProvider = toolbox->collisionAvoidanceDataProvider();

    QObject::connect(ownFlow, &hw::OwnFlow::foeChanged,
                     _collisionAvoidanceDataProvider, &CollisionAvoidanceDataProvider::foeReady
                     );//Qt::BlockingQueuedConnection);

    QObject::connect(ownFlow, &hw::OwnFlow::foeChanged,
                     this, &OwnFlowHandler::foeReady
                     );//Qt::BlockingQueuedConnection);

    QObject::connect(ownFlow, &hw::OwnFlow::opticalFlowChanged,
                     _collisionAvoidanceDataProvider, &CollisionAvoidanceDataProvider::opticalFlowReady);
    
    QObject::connect(ownFlow, &hw::OwnFlow::histogramChanged,
                     _collisionAvoidanceDataProvider, &CollisionAvoidanceDataProvider::histogramReady);

    start();
}

void OwnFlowHandler::loadSettings()
{
    // Load defaults from settings
    QSettings settings;
    settings.beginGroup("QGC_COLLISION_AVOIDANCE");
    FILENAME = settings.value("FILENAME", "CollisionAvoidance/res/rl/go5.mp4").toString().toStdString();
    OUTPUT_DIR = settings.value("OUTPUT_DIR", "CollisionAvoidance/res/out/RandomCollider/cpp/frame_").toString().toStdString();
    SUBSAMPLE_AMOUNT = settings.value("SUBSAMPLE_AMOUNT", 2).toInt();
    PARTICLES = settings.value("PARTICLES", 20000).toInt();
    WINDOW_SIZE = settings.value("WINDOW_SIZE", 5).toInt();
    INLIER_PROPORTION_THRESHOLD = settings.value("INLIER_PROPORTION_THRESHOLD", 0.01).toDouble();
    DIVERGENCE_PATCHSIZE = settings.value("DIVERGENCE_PATCHSIZE", 20).toInt();
    DIVERGENCE_THRESHOLD = settings.value("DIVERGENCE_THRESHOLD", 0.08).toDouble();
    settings.endGroup();
}

void OwnFlowHandler::storeSettings()
{
    // Store settings
    QSettings settings;
    settings.setValue("FILENAME", QString::fromStdString(FILENAME));
    settings.setValue("OUTPUT_DIR", QString::fromStdString(OUTPUT_DIR));
    settings.setValue("SUBSAMPLE_AMOUNT", SUBSAMPLE_AMOUNT);
    settings.setValue("PARTICLES", PARTICLES);
    settings.setValue("WINDOW_SIZE", WINDOW_SIZE);
    settings.setValue("INLIER_PROPORTION_THRESHOLD", INLIER_PROPORTION_THRESHOLD);
    settings.setValue("DIVERGENCE_PATCHSIZE", DIVERGENCE_PATCHSIZE);
    settings.setValue("DIVERGENCE_THRESHOLD", DIVERGENCE_THRESHOLD);
    settings.endGroup();
}

void OwnFlowHandler::foeReady(const cv::Mat& frame, std::shared_ptr<hw::FocusOfExpansionDto> foeFiltered, std::shared_ptr<hw::FocusOfExpansionDto> foeMeasured, std::shared_ptr<hw::Divergence> divergence) {
  std::cout << "Foe: " << foeFiltered->getFoE() << std::endl;
//  std::cout << "divergence: " << divergence->getDivergence() << std::endl;
//  std::cout << "measuredFoe: " << foeMeasured->getFoE() << std::endl;
//  std::cout << "inlierProportion: " << foeFiltered->getInlierProportion() << std::endl;
//  std::cout << "numberOfInliers: " << foeFiltered->getNumberOfInliers() << std::endl;
//  std::cout << "numberOfParticles: " << foeFiltered->getNumberOfParticles() <<std::endl;
}  
