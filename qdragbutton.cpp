#include "qdragbutton.h"
#include <QMouseEvent>
#include <QMimeData>
#include <QDrag>

void QDragButton::mousePressEvent(QMouseEvent* event)
{
	if(event->button() == Qt::LeftButton)
	{
		QMimeData* curMimeData = new QMimeData();
		curMimeData->setData(mimeFormat(), mimeData());

		QDrag* drag = new QDrag(this);
		drag->setMimeData(curMimeData);
		drag->setHotSpot(event->pos());

		drag->exec(Qt::CopyAction, Qt::CopyAction);
	}
}

void QDragButton::setMimeFormat(QString const& format) {m_format = format;}
void QDragButton::setMimeData(QByteArray const& data) {m_data = data;}
QString QDragButton::mimeFormat() const {return m_format;}
QByteArray QDragButton::mimeData() const {return m_data;}
