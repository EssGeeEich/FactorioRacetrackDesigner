#include "qfactorioitem_startingarea.h"
#include "qfactoriotrackeditor.h"
#include "qcallbackitem.h"

#include <QGraphicsRectItem>
#include <QFormLayout>
#include <QDoubleSpinBox>

// Initialize our entity
bool QFactorioItem<QFIT_STARTING_AREA>::initialize()
{
	m_propertyX = nullptr;
	m_propertyY = nullptr;
	m_propertyRot = nullptr;

	// Create a new Starting Area (Graphics only)
	m_gfx = std::make_unique<QGraphicsStartingArea>();

	// Setup our rotation point to be at our center
	m_gfx->setTransformOriginPoint(m_gfx->boundingRect().center());

	// Add a callback for whenever an user is moving this graphics item around.
	m_gfx->setCallback(QGraphicsItem::ItemPositionHasChanged,
		[=](QVariant const& var)
	{
		// Make it update our "real" position
		updateLocalPos(var.toPointF());
	});

	// Add this graphics item to the editor
	editor()->addItem(m_gfx.get());
	updateChildPos();

	// Set this object's name in the Scene Graph
	item()->setText(0,QObject::tr("Starting Area (%1)").arg(id()));

	// Successfully initialized!
	return true;
}

void QFactorioItem<QFIT_STARTING_AREA>::updatePos()
{
	// Our position changed.
	// Update the properties panels, if they're enabled.
	QPointF pf = pos();
	if(m_propertyX)
		m_propertyX->setValue(pf.x());
	if(m_propertyY)
		m_propertyY->setValue(pf.y());
}

void QFactorioItem<QFIT_STARTING_AREA>::updateChildPos()
{
	// Our "real" position is changing.
	// In this function we need to change our graphics item's position.
	// This happens, for example, when we change position from the properties
	// panel, or when we load an entity from file.
	if(m_gfx)
		m_gfx->setPos(pos());
}

int QFactorioItem<QFIT_STARTING_AREA>::type() const
{
	// This function MUST return our type!
	// Failure to do so will corrupt our file saves.
	return QFIT_STARTING_AREA;
}

void QFactorioItem<QFIT_STARTING_AREA>::unSelect()
{
	m_gfx->setHighlighted(false);
}

void QFactorioItem<QFIT_STARTING_AREA>::select()
{
	m_gfx->setHighlighted(true);
}

void QFactorioItem<QFIT_STARTING_AREA>::afterUnpopulate()
{
	m_propertyX = nullptr;
	m_propertyY = nullptr;
	m_propertyRot = nullptr;
}
void QFactorioItem<QFIT_STARTING_AREA>::populatePropertiesPanel(QWidget* panel)
{
	// Set up the properties panel...

	// Create a Form layout and add three properties
	QFormLayout* layout = new QFormLayout(panel);
	m_propertyX = new QDoubleSpinBox(panel);
	m_propertyY = new QDoubleSpinBox(panel);
	m_propertyRot = new QDoubleSpinBox(panel);

	// Put them in a form layout
	layout->addRow(QObject::tr("Position (X)"), m_propertyX);
	layout->addRow(QObject::tr("Position (Y)"), m_propertyY);
	layout->addRow(QObject::tr("Rotation"), m_propertyRot);

	// Set their limits
	m_propertyX->setRange(-99999,99999);
	m_propertyY->setRange(-99999,99999);
	m_propertyRot->setRange(-360, 360);

	// Set their current values
	m_propertyX->setValue(pos().x());
	m_propertyY->setValue(pos().y());
	m_propertyRot->setValue(m_gfx->rotation());

	// When they change, update our values
	QObject::connect(m_propertyX, qOverload<double>(&QDoubleSpinBox::valueChanged),
		[=](double val)
	{
		QPointF s = pos();
		s.rx() = val;
		setPos(s);
	});
	QObject::connect(m_propertyY, qOverload<double>(&QDoubleSpinBox::valueChanged),
		[=](double val)
	{
		QPointF s = pos();
		s.ry() = val;
		setPos(s);
	});
	QObject::connect(m_propertyRot, qOverload<double>(&QDoubleSpinBox::valueChanged),
		[=](double val)
	{
		m_gfx->setRotation(val);
	});
}

QGraphicsStartingArea const* QFactorioItem<QFIT_STARTING_AREA>::graphics() const
{
	return m_gfx.get();
}

QDataStream& QFactorioItem<QFIT_STARTING_AREA>::save(QDataStream& dst) const
{
	// Save all the data required to represent this entity.
	if(!m_gfx)
	{
		dst.setStatus(QDataStream::WriteFailed);
		return dst;
	}

	// Take into consideration that the position is a "core" property,
	// and will always be saved and loaded (using pos() and setPos()).
	dst << m_gfx->rotation();
	return dst;
}

QDataStream& QFactorioItem<QFIT_STARTING_AREA>::load(QDataStream& src)
{
	// Load all the data back...
	if(!m_gfx)
	{
		src.setStatus(QDataStream::ReadCorruptData);
		return src;
	}

	qreal rotation;
	src >> rotation;
	if(src.status() == QDataStream::Ok)
	{
		m_gfx->setRotation(rotation);
	}
	return src;
}
