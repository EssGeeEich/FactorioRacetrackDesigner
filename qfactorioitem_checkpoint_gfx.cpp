#include "qfactorioitem_checkpoint_gfx.h"
#include <QBrush>
#include <QPen>

/* Create a new Qt Graphics Item for the Checkpoint.
 * This entity is composed of two points and a joining line.
 * The line is green, with two yellow points at the two ends.
 * First things first, create the point class.
 */
QGraphicsCheckpointPoint::QGraphicsCheckpointPoint(qreal x, QGraphicsItem* parent)
		: QCallbackItem<QGraphicsEllipseItem>(-3,-3,6,6,parent)
{
	setBrush(QBrush(QColor(251, 192, 45, 200)));
	setPen(Qt::NoPen);
	setPos(QPointF(x,0));

	// Required so that our item can tell us when the user
	// is dragging him around.
	setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);

	// Let the user drag our graphics item around.
	setFlag(QGraphicsItem::ItemIsSelectable, false);
	setFlag(QGraphicsItem::ItemIsMovable, true);
}

QGraphicsCheckpoint::QGraphicsCheckpoint()
	: QCallbackItem<QGraphicsLineItem>(-20,0,20,0),
	  ptA(nullptr),
	  ptB(nullptr)
{
	// Set our color
	setPen(QPen(
		QBrush(QColor(56, 142, 60)),
		1,
		Qt::SolidLine,
		Qt::FlatCap,
		Qt::MiterJoin
	));


	// Set up our points
	ptA = new QGraphicsCheckpointPoint(line().p1().x(), this);
	ptA->setCallback(QGraphicsItem::ItemPositionHasChanged,
		[=](QVariant const& var)
	{
		QLineF l = line();
		l.setP1(var.toPointF());
		setLine(l);

		runUserCallback(LineChangedUserCallback, var);
	});

	ptB = new QGraphicsCheckpointPoint(line().p2().x(), this);
	ptB->setCallback(QGraphicsItem::ItemPositionHasChanged,
		[=](QVariant const& var)
	{
		QLineF l = line();
		l.setP2(var.toPointF());
		setLine(l);

		runUserCallback(LineChangedUserCallback, var);
	});

	setCallback(QGraphicsItem::ItemPositionHasChanged,
		std::bind(&QGraphicsCheckpoint::updateChildPos, this));

	// Required so that our item can tell us when it's being moved
	setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);

	// Don't let the user select and drag our graphics item around.
	setFlag(QGraphicsItem::ItemIsSelectable, false);
	setFlag(QGraphicsItem::ItemIsMovable, false);
}

void QGraphicsCheckpoint::updateChildPos()
{
	QLineF l = line();
	ptA->setPos(l.p1());
	ptB->setPos(l.p2());
}
