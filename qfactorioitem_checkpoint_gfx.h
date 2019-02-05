#ifndef QFACTORIOITEM_CHECKPOINT_GFX_H
#define QFACTORIOITEM_CHECKPOINT_GFX_H
#include <QGraphicsEllipseItem>
#include <QGraphicsLineItem>
#include "qcallbackitem.h"


/* Create a new Qt Graphics Item for the Checkpoint.
 * This entity is composed of two points and a joining line.
 * The line is green, with two yellow points at the two ends.
 */
class QGraphicsCheckpointPoint : public QCallbackItem<QGraphicsEllipseItem>
{
public:
	QGraphicsCheckpointPoint(qreal, QGraphicsItem*);
};
class QGraphicsCheckpoint : public QCallbackItem<QGraphicsLineItem>
{
	QGraphicsCheckpointPoint* ptA;
	QGraphicsCheckpointPoint* ptB;
public:
	enum {
		LineChangedUserCallback = 0
	};

	QGraphicsCheckpoint();
	void updateChildPos();
};

#endif // QFACTORIOITEM_CHECKPOINT_GFX_H
