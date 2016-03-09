#ifndef COLLISIONAVOIDANCE_COLLISIONAVOIDANCEWIDGET_H
#define COLLISIONAVOIDANCE_COLLISIONAVOIDANCEWIDGET_H

#include "QGCQmlWidgetHolder.h"

class CollisionAvoidanceWidget : public QGCQmlWidgetHolder
{
    Q_OBJECT
public:
    CollisionAvoidanceWidget(const QString& title, QAction* action, QWidget *parent = 0);
};

#endif // COLLISIONAVOIDANCE_COLLISIONAVOIDANCEWIDGET_H
