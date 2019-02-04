#include "qfactorioitem_checkpoint.h"
#include "qfactoriotrackeditor.h"
#include "qcallbackitem.h"

#include <QGraphicsLineItem>
#include <QGraphicsEllipseItem>
#include <QFormLayout>
#include <QDoubleSpinBox>

/* Create a new Qt Graphics Item for the Checkpoint.
 * This entity is composed of two points and a joining line.
 * The line is green, with two yellow points at the two ends.
 * First things first, create the point class.
 */
class QGraphicsCheckpointPoint : public QCallbackItem<QGraphicsEllipseItem>
{
public:
	QGraphicsCheckpointPoint(qreal x, QGraphicsItem* parent)
		: QCallbackItem<QGraphicsEllipseItem>(-3,-3,6,6,parent)
	{
		setBrush(QBrush(QColor(251, 192, 45, 200)));
		setPen(Qt::NoPen);
		setPos(QPointF(x,0));

		// Required so that our item can tell us when the user
		// is dragging him around.
		setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);

		// Let the user select and drag our graphics item around.
		setFlag(QGraphicsItem::ItemIsSelectable, true);
		setFlag(QGraphicsItem::ItemIsMovable, true);
	}
};
class QGraphicsCheckpoint : public QCallbackItem<QGraphicsLineItem>
{
	QGraphicsCheckpointPoint* ptA;
	QGraphicsCheckpointPoint* ptB;
public:
	QGraphicsCheckpoint()
		: QCallbackItem<QGraphicsLineItem>(-20,0,20,0),
		  ptA(nullptr),
		  ptB(nullptr)
	{
		// Set our color
		setPen(QPen(
			QBrush(QColor(56, 142, 60)),
			1,
			Qt::SolidLine,
			Qt::FlatCap,
			Qt::MiterJoin
		));


		// Set up our points
		ptA = new QGraphicsCheckpointPoint(line().p1().x(), this);
		ptA->setCallback(QGraphicsItem::ItemPositionHasChanged,
			[=](QVariant const& var)
		{
			QLineF l = line();
			l.setP1(var.toPointF());
			setLine(l);
		});
		ptB = new QGraphicsCheckpointPoint(line().p2().x(), this);
		ptB->setCallback(QGraphicsItem::ItemPositionHasChanged,
			[=](QVariant const& var)
		{
			QLineF l = line();
			l.setP2(var.toPointF());
			setLine(l);
		});

		setCallback(QGraphicsItem::ItemPositionHasChanged,
			std::bind(&QGraphicsCheckpoint::updateChildPos, this));

		// Required so that our item can tell us when the user
		// is dragging him around.
		setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);

		// Let the user select and drag our graphics item around.
		setFlag(QGraphicsItem::ItemIsSelectable, false);
		setFlag(QGraphicsItem::ItemIsMovable, false);
	}

	void updateChildPos()
	{
		QLineF l = line();
		ptA->setPos(l.p1());
		ptB->setPos(l.p2());
	}
};

bool QFactorioItem<QFIT_CHECKPOINT>::initialize()
{
	m_checkpointSeqId = 0;
	m_propertyX_A = nullptr;
	m_propertyX_B = nullptr;
	m_propertyY_A = nullptr;
	m_propertyY_B = nullptr;
	QGraphicsCheckpoint* ckpt = new QGraphicsCheckpoint;

	m_gfx.reset(ckpt);
	editor()->addItem(ckpt);
	updateChildPos();

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
	// This should never get called, since we don't handle the parent's position.
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

void QFactorioItem<QFIT_CHECKPOINT>::populatePropertiesPanel(QWidget*)
{

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
	QGraphicsLineItem* line = dynamic_cast<QGraphicsLineItem*>(m_gfx.get());
	// Save all the data required to represent this entity.
	if(!line)
	{
		dst.setStatus(QDataStream::WriteFailed);
		return dst;
	}

	dst << line->line() << sequenceId();
	return dst;
}

QDataStream& QFactorioItem<QFIT_CHECKPOINT>::load(QDataStream& src)
{
	QGraphicsCheckpoint* line = dynamic_cast<QGraphicsCheckpoint*>(m_gfx.get());
	// Save all the data required to represent this entity.
	if(!line)
	{
		src.setStatus(QDataStream::WriteFailed);
		return src;
	}

	QLineF lineF;
	quint32 seqId=0;
	src >> lineF >> seqId;
	if(src.status() == QDataStream::Ok)
	{
		line->setLine(lineF);
		setSequenceId(seqId);

		line->updateChildPos();
	}
	return src;
}
