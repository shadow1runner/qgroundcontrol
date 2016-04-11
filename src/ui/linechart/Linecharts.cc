#include <QShowEvent>

#include "Linecharts.h"
#include "MultiVehicleManager.h"
#include "MainWindow.h"
#include "UAS.h"
#include "OwnFlowGrapher.h"

Linecharts::Linecharts(const QString& title, QAction* action, MAVLinkDecoder* decoder, OwnFlowGrapher* ownFlowGrapher, QWidget *parent)
    : MultiVehicleDockWidget(title, action, parent)
    , _mavlinkDecoder(decoder)
    , _ownFlowGrapher(ownFlowGrapher)
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
    connect(_ownFlowGrapher, &OwnFlowGrapher::valueChanged, widget, &LinechartWidget::appendData);

    // Select system
    widget->setActive(true);

    return widget;
}
