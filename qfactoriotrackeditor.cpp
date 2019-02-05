#include "qfactoriotrackeditor.h"
#include <QGraphicsPixmapItem>
#include <QGraphicsTextItem>
#include <QMessageBox>

#include "qfactorioitem_checkpoint.h"
#include "qfactorioitem_startingarea.h"

QFactorioTrackEditor::QFactorioTrackEditor()
	:
	  m_backgroundImage(nullptr),
	  m_tree(nullptr)
{}

void QFactorioTrackEditor::setBackgroundPixmap(QPixmap const& pixmap)
{
	if(m_backgroundImage)
	{
		removeItem(m_backgroundImage);
		m_backgroundImage = nullptr;
	}
	if(!pixmap.isNull())
	{
		m_backgroundImage = this->addPixmap(pixmap);
		if(m_backgroundImage)
		{
			m_backgroundImage->setZValue(-1);
			m_backgroundImage->setFlag(QGraphicsItem::ItemIsSelectable, false);
			m_backgroundImage->setFlag(QGraphicsItem::ItemIsMovable, false);
			m_backgroundImage->setFlag(QGraphicsItem::ItemIsFocusable, false);
			m_backgroundImage->setFlag(QGraphicsItem::ItemSendsGeometryChanges, false);
		}
	}
}

quint32 QFactorioTrackEditor::getFreeId() const
{
	quint32 id = 0;
	while((++id) > 0)
	{
		if(!getItem(id))
			return id;
	}
	return 0;
}

QTreeWidgetItem* QFactorioTrackEditor::category(QFactorioItemType type)
{
	{
		auto it = m_categories.find(type);
		if(it != m_categories.end())
			return it->second.get();
	}

	if(static_cast<std::size_t>(type) > g_mimeEquivalence.size())
		return nullptr;

	auto it = g_mimeEquivalence.begin();
	std::advance(it, type);

	std::unique_ptr<QTreeWidgetItem> item
		= std::make_unique<QTreeWidgetItem>(m_tree);
	item->setText(0, QObject::tr(qPrintable(it->m_name)));
	return m_categories.insert(std::make_pair(type,std::move(item))).first
			->second.get();
}

bool QFactorioTrackEditor::checkId() const
{
	std::map<quint32, bool> m_storage;

	for(auto it = m_items.begin(); it != m_items.end(); ++it)
	{
		quint32 id = (*it)->id();

		if(m_storage.find(id) != m_storage.end())
			return false;
		m_storage.insert(std::make_pair(id, true));
	}
	return true;
}

QFactorioBasicItem* QFactorioTrackEditor::getItem(quint32 id) const
{
	for(auto it = m_items.begin(); it != m_items.end(); ++it)
	{
		if((*it)->id() == id)
			return it->get();
	}
	return nullptr;
}

void QFactorioTrackEditor::initialize(QTreeWidget* tree)
{
	m_tree = tree;
	clear();
}

void QFactorioTrackEditor::clear()
{
	setBackgroundPixmap(QPixmap());
	m_items.clear();
}

static quint32 const byteMark = 1180779339;

QDataStream& QFactorioTrackEditor::save(QDataStream& dst) const
{
	if(!m_backgroundImage)
		dst.setStatus(QDataStream::WriteFailed);
	else
	{
		quint32 size = m_items.size();
		dst << byteMark
			<< m_backgroundImage->pixmap()
			<< size;
		bool bDoWarn = true;
		for(quint32 i = 0; i < size; ++i)
		{
			QByteArray data;
			QDataStream item_stream(&data, QIODevice::WriteOnly);
			m_items[i]->save(item_stream);

			if(item_stream.status() == QDataStream::Ok)
			{
				dst << m_items[i]->type()
					<< m_items[i]->id()
					<< m_items[i]->pos()
					<< data;
			}
			else if(bDoWarn)
			{
				switch(QMessageBox::warning(nullptr,
						tr("Unable to save an entity!"),
tr("I'm unable to save an entity!\r\nDo you want to continue saving this file?"),
						QMessageBox::Yes,
						QMessageBox::YesToAll,
						QMessageBox::No))
				{
				case QMessageBox::No:
					dst.setStatus(QDataStream::WriteFailed);
					return dst;
				case QMessageBox::YesToAll:
					bDoWarn = false;
					break;
				default:
					break;
				}
			}
			if(dst.status() != QDataStream::Ok)
				return dst;
		}
	}
	return dst;
}

QFactorioBasicItem* QFactorioTrackEditor::createNew(QByteArray const& mimeFormat, QPointF pos)
{
	for(auto it = g_mimeEquivalence.begin(); it != g_mimeEquivalence.end(); ++it)
	{
		if(it->m_mime == mimeFormat)
			return createNew(static_cast<QFactorioItemType>(
					std::distance(g_mimeEquivalence.begin(), it)), pos);
	}
	return nullptr;
}

QDataStream& QFactorioTrackEditor::load(QDataStream& src)
{
	quint32 v = 0;
	src >> v;
	if(v != byteMark)
		src.setStatus(QDataStream::ReadCorruptData);
	else
	{
		QPixmap pxmp;
		src >> pxmp;
		if(pxmp.isNull())
			src.setStatus(QDataStream::ReadCorruptData);
		else
		{
			setBackgroundPixmap(pxmp);
			bool bDoWarn = true;
			quint32 count = 0;
			src >> count;
			for(quint32 i = 0; (src.status() == QDataStream::Ok) && (i < count); ++i)
			{
				int type;
				quint32 id=0;
				QPointF pos;
				QByteArray data;
				bool bReadSuccess = false;
				src >> type >> id >> pos >> data;

				QDataStream item_stream(&data, QIODevice::ReadOnly);
				QFactorioBasicItem* item = createNew(static_cast<QFactorioItemType>(type), pos);
				if(item)
				{
					item->setId(id);
					if(item->load(item_stream).status() == QDataStream::Ok)
						bReadSuccess = true;
				}


				if(!bReadSuccess && bDoWarn)
				{
					switch(QMessageBox::warning(nullptr,
							tr("Unable to load an entity!"),
	tr("I'm unable to load an entity!\r\nDo you want to continue loading this file?"),
							QMessageBox::Yes,
							QMessageBox::YesToAll,
							QMessageBox::No))
					{
					case QMessageBox::No:
						src.setStatus(QDataStream::ReadCorruptData);
						return src;
					case QMessageBox::YesToAll:
						bDoWarn = false;
						break;
					default:
						break;
					}
				}
			}
			if(src.status() == QDataStream::Ok && !checkId())
				src.setStatus(QDataStream::ReadCorruptData);
		}
	}
	return src;
}

template <QFactorioItemType> struct QFactorioItemAllocator;
template <> struct QFactorioItemAllocator<QFIT_COUNT>
{
	static QFactorioBasicItem* Allocate(QFactorioItemType, QTreeWidgetItem*) { return nullptr; }
};

template <QFactorioItemType Type> struct QFactorioItemAllocator
{
	static QFactorioBasicItem* Allocate(QFactorioItemType type, QTreeWidgetItem* item)
	{
		if(type == Type)
			return new QFactorioItem<Type>(item);
		return QFactorioItemAllocator<
				static_cast<QFactorioItemType>(Type + 1)
				>::Allocate(type, item);
	}
};

QFactorioBasicItem* QFactorioTrackEditor::createNew(QFactorioItemType type, QPointF pos)
{
	std::unique_ptr<QFactorioBasicItem> item;

	QTreeWidgetItem* cat = category(type);
	if(!cat)
		return nullptr;

	item.reset(
		QFactorioItemAllocator<static_cast<QFactorioItemType>(0)>
			::Allocate(type, cat)
	);

	if(!item)
		return nullptr;

	item->setId(getFreeId());
	item->newItem(this, pos);
	m_items.push_back(std::move(item));
	return m_items.back().get();
}
