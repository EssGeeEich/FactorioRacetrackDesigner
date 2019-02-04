#include "qfactoriobasicitem.h"

QFactorioBasicItem::QFactorioBasicItem(QTreeWidgetItem* item) : m_item(item) {}

bool QFactorioBasicItem::newItem(QFactorioTrackEditor* editor, QPointF pos)
{
	m_editor = editor;
	m_pos = pos;
	return initialize();
}

void QFactorioBasicItem::setPos(QPointF const& pos)
{
	if(m_pos != pos)
	{
		m_pos = pos;
		updatePos();
		updateChildPos();
	}
}

void QFactorioBasicItem::updateLocalPos(const QPointF& pos)
{
	if(m_pos != pos)
	{
		m_pos = pos;
		updatePos();
	}
}

void QFactorioBasicItem::setId(quint32 id)
{
	m_id = id;
	m_item.setData(0, QF_USERROLE_ITEM, static_cast<qulonglong>(m_id));
}

void QFactorioBasicItem::updatePos(){}
void QFactorioBasicItem::updateChildPos(){}
void QFactorioBasicItem::populatePropertiesPanel(QWidget *){}
void QFactorioBasicItem::beforeUnpopulate(){}
void QFactorioBasicItem::afterUnpopulate(){}

QFactorioTrackEditor* QFactorioBasicItem::editor() const { return m_editor; }
QTreeWidgetItem* QFactorioBasicItem::item() { return &m_item; }
QTreeWidgetItem const* QFactorioBasicItem::item() const { return &m_item; }
QPointF QFactorioBasicItem::pos() const { return m_pos; }
quint32 QFactorioBasicItem::id() const { return m_id; }
