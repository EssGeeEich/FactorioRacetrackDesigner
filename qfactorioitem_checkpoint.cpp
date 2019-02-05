#include "qfactorioitem_checkpoint.h"
#include "qfactoriotrackeditor.h"
#include "qcallbackitem.h"
#include <QFormLayout>
#include <QDoubleSpinBox>

bool QFactorioItem<QFIT_CHECKPOINT>::initialize()
{
	m_checkpointSeqId = 0;
	m_propertyX_A = nullptr;
	m_propertyX_B = nullptr;
	m_propertyY_A = nullptr;
	m_propertyY_B = nullptr;

	// Create a new Checkpoint (Graphics only)
	m_gfx = std::make_unique<QGraphicsCheckpoint>();

	// Whenever the line changes, update our position
	m_gfx->setUserCallback(QGraphicsCheckpoint::LineChangedUserCallback,
		std::bind(&QFactorioItem<QFIT_CHECKPOINT>::updatePos, this));

	// Add this graphics item to the editor
	editor()->addItem(m_gfx.get());
	updateChildPos();

	// Assign a new Checkpoint ID
	std::vector<QFactorioItem<QFIT_CHECKPOINT>*>
		otherCheckpoints = editor()->findEntities<QFactorioItem<QFIT_CHECKPOINT>>();

	quint32 ckptId = 1;
	for(auto it = otherCheckpoints.begin(); it != otherCheckpoints.end(); ++it)
	{
		ckptId = std::max(ckptId, (*it)->sequenceId() + 1);
	}
	setSequenceId(ckptId);
	return true;
}

void QFactorioItem<QFIT_CHECKPOINT>::updatePos()
{
	// We need to call this manually, since the parent won't be calling this.
	QLineF l = m_gfx->line();
	if(m_propertyX_A)
		m_propertyX_A->setValue(l.p1().x());
	if(m_propertyY_A)
		m_propertyY_A->setValue(l.p1().y());
	if(m_propertyX_B)
		m_propertyX_B->setValue(l.p2().x());
	if(m_propertyY_B)
		m_propertyY_B->setValue(l.p2().y());
}

void QFactorioItem<QFIT_CHECKPOINT>::updateChildPos()
{
	// Our "real" position is changing.
	// In this function we need to change our graphics item's position.
	// This happens, for example, when we change position from the properties
	// panel, or when we load an entity from file.
	// In this entity, we use this function mainly to handle drag'n'drop.
	// Our load() function will override the position.
	if(m_gfx)
		m_gfx->setPos(pos());
}

int QFactorioItem<QFIT_CHECKPOINT>::type() const
{
	return QFIT_CHECKPOINT;
}

void QFactorioItem<QFIT_CHECKPOINT>::unSelect()
{
	m_gfx->setHighlighted(false);
}

void QFactorioItem<QFIT_CHECKPOINT>::select()
{
	m_gfx->setHighlighted(true);
}

void QFactorioItem<QFIT_CHECKPOINT>::afterUnpopulate()
{
	m_propertyX_A = nullptr;
	m_propertyX_B = nullptr;
	m_propertyY_A = nullptr;
	m_propertyY_B = nullptr;
	m_propertyId = nullptr;
}

void QFactorioItem<QFIT_CHECKPOINT>::populatePropertiesPanel(QWidget* panel)
{
	// Set up the properties panel...
	if(!m_gfx)
		return;

	// Create a Form layout and add three properties
	QFormLayout* layout = new QFormLayout(panel);
	m_propertyX_A = new QDoubleSpinBox(panel);
	m_propertyY_A = new QDoubleSpinBox(panel);
	m_propertyX_B = new QDoubleSpinBox(panel);
	m_propertyY_B = new QDoubleSpinBox(panel);
	m_propertyId = new QSpinBox(panel);

	// Put them in a form layout
	layout->addRow(QObject::tr("Position A (X)"), m_propertyX_A);
	layout->addRow(QObject::tr("Position A (Y)"), m_propertyY_A);
	layout->addRow(QObject::tr("Position B (X)"), m_propertyX_B);
	layout->addRow(QObject::tr("Position B (Y)"), m_propertyY_B);
	layout->addRow(QObject::tr("Checkpoint ID"), m_propertyId);

	// Set their limits
	m_propertyX_A->setRange(-99999,99999);
	m_propertyY_A->setRange(-99999,99999);
	m_propertyX_B->setRange(-99999,99999);
	m_propertyY_B->setRange(-99999,99999);
	m_propertyId->setRange(1,99999);

	// Set their current values
	QLineF l = m_gfx->line();
	m_propertyX_A->setValue(l.p1().x());
	m_propertyY_A->setValue(l.p1().y());
	m_propertyX_B->setValue(l.p2().x());
	m_propertyY_B->setValue(l.p2().y());
	m_propertyId->setValue(static_cast<int>(m_checkpointSeqId));

	// When they change, update our values
	QObject::connect(m_propertyX_A, qOverload<double>(&QDoubleSpinBox::valueChanged),
		[=](double val)
	{
		QLineF line = m_gfx->line();
		QPointF pt = line.p1();
			pt.rx() = val;
		line.setP1(pt);
		m_gfx->setLine(line);
		m_gfx->updateChildPos();
	});
	QObject::connect(m_propertyY_A, qOverload<double>(&QDoubleSpinBox::valueChanged),
		[=](double val)
	{
		QLineF line = m_gfx->line();
		QPointF pt = line.p1();
			pt.ry() = val;
		line.setP1(pt);
		m_gfx->setLine(line);
		m_gfx->updateChildPos();
	});
	QObject::connect(m_propertyX_B, qOverload<double>(&QDoubleSpinBox::valueChanged),
		[=](double val)
	{
		QLineF line = m_gfx->line();
		QPointF pt = line.p2();
			pt.rx() = val;
		line.setP2(pt);
		m_gfx->setLine(line);
		m_gfx->updateChildPos();
	});
	QObject::connect(m_propertyY_B, qOverload<double>(&QDoubleSpinBox::valueChanged),
		[=](double val)
	{
		QLineF line = m_gfx->line();
		QPointF pt = line.p2();
			pt.ry() = val;
		line.setP2(pt);
		m_gfx->setLine(line);
		m_gfx->updateChildPos();
	});
	QObject::connect(m_propertyId, qOverload<int>(&QSpinBox::valueChanged),
		[=](int val)
	{
		if(val <= 0)
			return;
		setSequenceId(static_cast<quint32>(val));
	});
}

void QFactorioItem<QFIT_CHECKPOINT>::setSequenceId(quint32 seqId)
{
	if(seqId != m_checkpointSeqId)
	{
		m_checkpointSeqId = seqId;
		item()->setText(0,QObject::tr("Checkpoint #%1 (%2)")
			.arg(m_checkpointSeqId)
			.arg(id()));
	}
}

quint32 QFactorioItem<QFIT_CHECKPOINT>::sequenceId() const
{
	return m_checkpointSeqId;
}

QDataStream& QFactorioItem<QFIT_CHECKPOINT>::save(QDataStream& dst) const
{
	// Save all the data required to represent this entity.
	if(!m_gfx)
	{
		dst.setStatus(QDataStream::WriteFailed);
		return dst;
	}

	dst << m_gfx->line() << sequenceId();
	return dst;
}

QDataStream& QFactorioItem<QFIT_CHECKPOINT>::load(QDataStream& src)
{
	// Save all the data required to represent this entity.
	if(!m_gfx)
	{
		src.setStatus(QDataStream::WriteFailed);
		return src;
	}

	QLineF lineF;
	quint32 seqId=0;
	src >> lineF >> seqId;
	if(src.status() == QDataStream::Ok)
	{
		m_gfx->setLine(lineF);
		setSequenceId(seqId);

		m_gfx->updateChildPos();
	}
	return src;
}
