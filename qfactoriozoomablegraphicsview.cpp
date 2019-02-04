#include "qfactoriozoomablegraphicsview.h"
#include <QWheelEvent>
#include <QMouseEvent>
#include <QDragEnterEvent>
#include <QDragMoveEvent>
#include <QDropEvent>
#include <QMimeData>
#include <QGraphicsTextItem>
#include "qfactorioconstants.h"

void QFactorioZoomableGraphicsView::wheelEvent(QWheelEvent* event)
{
	int steps = event->angleDelta().y();
	steps /= 120;

	double fScale = 1.0;
	if(steps > 0)
		fScale += static_cast<double>(steps);
	else
	{
		while(steps < 0)
		{
			fScale /= 2.0;
			++steps;
		}
	}

	this->scale(fScale, fScale);
}

void QFactorioZoomableGraphicsView::mousePressEvent(QMouseEvent* event)
{
	if(event->button() == Qt::MidButton)
		this->resetMatrix();

	QGraphicsView::mousePressEvent(event);
}

void QFactorioZoomableGraphicsView::dragEnterEvent(QDragEnterEvent* event)
{
	if(m_editor && event->mimeData()->hasFormat(g_factorioMimeFormat))
		event->acceptProposedAction();
}

void QFactorioZoomableGraphicsView::dragMoveEvent(QDragMoveEvent* event)
{
	if(m_editor && event->mimeData()->hasFormat(g_factorioMimeFormat))
		event->acceptProposedAction();
}

void QFactorioZoomableGraphicsView::dropEvent(QDropEvent* event)
{
	if(m_editor && event->mimeData()->hasFormat(g_factorioMimeFormat))
	{
		m_editor->createNew(event->mimeData()->data(g_factorioMimeFormat),
			this->mapToScene(event->pos()));
	}
}

void QFactorioZoomableGraphicsView::setScene(QFactorioTrackEditor* editor)
{
	QGraphicsView::setScene(editor);
	m_editor = editor;
}

void QFactorioZoomableGraphicsView::setEditor(QFactorioTrackEditor* editor)
{
	m_editor = editor;
}
