#ifndef QDRAGBUTTON_H
#define QDRAGBUTTON_H
#include <QPushButton>
#include <QMimeData>

class QDragButton : public QPushButton
{
	using QPushButton::QPushButton;
	void mousePressEvent(QMouseEvent*);

	QString m_format;
	QByteArray m_data;
public:
	void setMimeFormat(QString const&);
	void setMimeData(QByteArray const&);

	QString mimeFormat() const;
	QByteArray mimeData() const;
};

#endif // QDRAGBUTTON_H
