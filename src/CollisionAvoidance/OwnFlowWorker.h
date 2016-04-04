#ifndef OWNFLOWWORKER_H
#define OWNFLOWWORKER_H

#include <string>

#include <QObject>

class OwnFlowHandler;

class OwnFlowWorker : public QObject {
	Q_OBJECT

public:
	OwnFlowWorker(const std::string& fileName, OwnFlowHandler* const ownFlowHandler);

private slots:
	void start();
	void stop();

private:
	const std::string& _fileName;
};

#endif // OWNFLOWWORKER_H
