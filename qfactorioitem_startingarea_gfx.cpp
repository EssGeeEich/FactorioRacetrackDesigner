#include "qfactorioitem_startingarea_gfx.h"
#include <QColor>
#include <QPen>

QGraphicsStartingArea::QGraphicsStartingArea()
: QCallbackItem<QGraphicsRectItem>(QRectF(0,0,16,8))
{
	// Set our color
	setPen(QColor(152,36,170));

	// Set up our "flashlights"
	QGraphicsRectItem* head = new QGraphicsRectItem(0,0,0.6,rect().height(), this);
	head->setPen(QPen(QColor(255,0,0)));

	// Required so that our item can tell us when the user
	// is dragging him around.
	setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);

	// Let the user drag our graphics item around.
	setFlag(QGraphicsItem::ItemIsSelectable, false);
	setFlag(QGraphicsItem::ItemIsMovable, true);
}
