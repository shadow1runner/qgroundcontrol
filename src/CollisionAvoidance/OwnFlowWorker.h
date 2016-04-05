#ifndef OWNFLOWWORKER_H
#define OWNFLOWWORKER_H

#include <string>

#include <QObject>

class OwnFlowHandler;

class OwnFlowWorker : public QObject {
	Q_OBJECT

public:
    OwnFlowWorker(const std::string& fileName, const OwnFlowHandler* const ownFlowHandler);

public slots:
	void start();
	void stop();

private:
	const std::string& _fileName;
	const OwnFlowHandler* const _ownFlowHandler;
};

#endif // OWNFLOWWORKER_H
