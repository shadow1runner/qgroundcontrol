#include "CollisionAvoidanceWidget.h"

CollisionAvoidanceWidget::CollisionAvoidanceWidget(const QString& title, QAction* action, QWidget *parent) :
    QGCQmlWidgetHolder(title, action, parent)
{
    Q_UNUSED(title);
    Q_UNUSED(action);
    setSource(QUrl::fromUserInput("qrc:/qml/CollisionAvoidance.qml"));

    loadSettings();
}

