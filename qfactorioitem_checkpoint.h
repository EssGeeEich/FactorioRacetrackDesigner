#ifndef QFACTORIOITEM_CHECKPOINT_H
#define QFACTORIOITEM_CHECKPOINT_H
#include "qfactorioconstants.h"
#include "qfactoriobasicitem.h"
#include "qserializable.h"
#include <QGraphicsItem>
#include <QDoubleSpinBox>

// Checkpoint class.
template <> class QFactorioItem<QFIT_CHECKPOINT> :
		public QFactorioBasicItem
{
	std::unique_ptr<QGraphicsItem> m_gfx;
	quint32 m_checkpointSeqId;

	QDoubleSpinBox* m_propertyX_A;
	QDoubleSpinBox* m_propertyY_A;
	QDoubleSpinBox* m_propertyX_B;
	QDoubleSpinBox* m_propertyY_B;
protected:
	virtual bool initialize() override;
	virtual void updatePos() override;
	virtual void updateChildPos() override;
	virtual void populatePropertiesPanel(QWidget*) override;
public:
	using QFactorioBasicItem::QFactorioBasicItem;
	virtual int type() const override;
	virtual QDataStream& save(QDataStream&) const override;
	virtual QDataStream& load(QDataStream&) override;

	void setSequenceId(quint32);
	quint32 sequenceId() const;
};

#endif // QFACTORIOITEM_CHECKPOINT_H