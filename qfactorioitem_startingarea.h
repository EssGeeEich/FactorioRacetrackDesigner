#ifndef QFACTORIOITEM_STARTINGAREA_H
#define QFACTORIOITEM_STARTINGAREA_H
#include "qfactorioitem_startingarea_gfx.h"
#include "qfactorioconstants.h"
#include "qfactoriobasicitem.h"
#include "qserializable.h"
#include <QGraphicsItem>
#include <QDoubleSpinBox>

// Starting Area class.
template <> class QFactorioItem<QFIT_STARTING_AREA> :
		public QFactorioBasicItem
{
	std::unique_ptr<QGraphicsStartingArea> m_gfx;

	QDoubleSpinBox* m_propertyX;
	QDoubleSpinBox* m_propertyY;
	QDoubleSpinBox* m_propertyRot;
protected:
	virtual bool initialize() override;
	virtual void unSelect() override;
	virtual void select() override;
	virtual void updatePos() override;
	virtual void updateChildPos() override;
	virtual void populatePropertiesPanel(QWidget*) override;
	virtual void afterUnpopulate() override;
public:
	using QFactorioBasicItem::QFactorioBasicItem;
	virtual int type() const override;
	virtual QDataStream& save(QDataStream&) const override;
	virtual QDataStream& load(QDataStream&) override;

	QGraphicsStartingArea const* graphics() const;
};

#endif // QFACTORIOITEM_STARTINGAREA_H
