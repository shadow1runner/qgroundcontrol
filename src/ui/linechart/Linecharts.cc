#include <QShowEvent>

#include "Linecharts.h"
#include "MultiVehicleManager.h"
#include "MainWindow.h"
#include "UAS.h"
#include "OwnFlowWorker.h"

Linecharts::Linecharts(const QString& title, QAction* action, MAVLinkDecoder* decoder, OwnFlowWorker* ownFlowWorker, QWidget *parent)
    : MultiVehicleDockWidget(title, action, parent)
    , _mavlinkDecoder(decoder)
    , _ownFlowWorker(ownFlowWorker)
{
    init();

    this->setVisible(false);
}

QWidget* Linecharts::_newVehicleWidget(Vehicle* vehicle, QWidget* parent)
{
    LinechartWidget* widget = new LinechartWidget(vehicle->id(), parent);

    // Connect valueChanged signals
    connect(vehicle->uas(), &UAS::valueChanged, widget, &LinechartWidget::appendData);

    // Connect decoder
    connect(_mavlinkDecoder, &MAVLinkDecoder::valueChanged, widget, &LinechartWidget::appendData);

    // Connect ownFlowWorker
    connect(_ownFlowWorker, &OwnFlowWorker::valueChanged, widget, &LinechartWidget::appendData);

    // Select system
    widget->setActive(true);

    return widget;
}
