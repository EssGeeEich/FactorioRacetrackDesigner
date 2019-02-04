#ifndef QFACTORIOBASICITEM_H
#define QFACTORIOBASICITEM_H
#include "qfactorioconstants.h"
#include <QPointF>
#include <QWidget>
#include <QTreeWidgetItem>
#include "qserializable.h"

// Basic item. Every item class must derive from this class.
class QFactorioBasicItem : public QSerializable
{
private:
	quint32 m_id;
	QFactorioTrackEditor* m_editor;
	QPointF m_pos;
	QTreeWidgetItem m_item;

	bool newItem(QFactorioTrackEditor*, QPointF);
	void setId(quint32);
protected:
	friend class MainWindow;
	friend class QFactorioTrackEditor;
	friend class QFactorioItemLoader;

	void updateLocalPos(QPointF const&);
	virtual bool initialize() =0;

	virtual void updatePos();
	virtual void updateChildPos();
	virtual void populatePropertiesPanel(QWidget*);
	virtual void beforeUnpopulate();
	virtual void afterUnpopulate();
public:
	QFactorioBasicItem(QTreeWidgetItem*);
	QFactorioTrackEditor* editor() const;
	QTreeWidgetItem* item();
	QTreeWidgetItem const* item() const;

	virtual QPointF pos() const;
	virtual void setPos(QPointF const&);

	virtual int type() const =0;
	quint32 id() const;
	virtual ~QFactorioBasicItem() {}
};


#endif // QFACTORIOBASICITEM_H
