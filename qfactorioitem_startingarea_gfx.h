#ifndef QFACTORIOITEM_STARTINGAREA_GFX_H
#define QFACTORIOITEM_STARTINGAREA_GFX_H
#include <QGraphicsRectItem>
#include "qcallbackitem.h"

/*  Create a new Qt Graphics Item for the Starting Area.
 *  This includes the Starting Area itself (in a violet color)
 *  PLUS the heading of the Starting Area (in a red color)
 */
class QGraphicsStartingArea : public QCallbackItem<QGraphicsRectItem>
{
public:
	QGraphicsStartingArea();
};

#endif // QFACTORIOITEM_STARTINGAREA_GFX_H
