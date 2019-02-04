#ifndef QFACTORIOZOOMABLEGRAPHICSVIEW_H
#define QFACTORIOZOOMABLEGRAPHICSVIEW_H
#include <QGraphicsView>
#include "qfactoriotrackeditor.h"
#include "qfactorioconstants.h"

class QFactorioZoomableGraphicsView : public QGraphicsView
{
	using QGraphicsView::QGraphicsView;
	void wheelEvent(QWheelEvent*);
	void mousePressEvent(QMouseEvent*);
	void dragEnterEvent(QDragEnterEvent*);
	void dragMoveEvent(QDragMoveEvent*);
	void dropEvent(QDropEvent*);
	QFactorioTrackEditor* m_editor;
public:
	void setScene(QFactorioTrackEditor*);
	void setEditor(QFactorioTrackEditor*);
};

#endif // QFACTORIOZOOMABLEGRAPHICSVIEW_H
