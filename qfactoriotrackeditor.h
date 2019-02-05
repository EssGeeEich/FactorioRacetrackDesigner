#ifndef QFACTORIOTRACKEDITOR_H
#define QFACTORIOTRACKEDITOR_H
#include <QGraphicsScene>
#include <QMimeData>
#include <QGraphicsItem>
#include <QTreeWidget>
#include <vector>
#include <memory>
#include <map>
#include "qfactorioconstants.h"
#include "qfactoriobasicitem.h"
#include "qserializable.h"

class QFactorioTrackEditor :
		public QGraphicsScene,
		public QSerializable
{
	QGraphicsPixmapItem* m_backgroundImage;
	QTreeWidget* m_tree;
	std::map<QFactorioItemType, std::unique_ptr<QTreeWidgetItem>> m_categories;
	std::vector<std::unique_ptr<QFactorioBasicItem>> m_items;
protected:
	quint32 getFreeId() const;
	QTreeWidgetItem* category(QFactorioItemType);
public:
	explicit QFactorioTrackEditor();
	void initialize(QTreeWidget*);

	void setBackgroundPixmap(QPixmap const&);
	QFactorioBasicItem* createNew(QFactorioItemType, QPointF);
	QFactorioBasicItem* createNew(QByteArray const&, QPointF);
	void clear();

	template <typename T>
	std::vector<T*> findEntities()
	{
		std::vector<T*> ents;
		ents.reserve(m_items.size());
		for(auto it = m_items.begin(); it != m_items.end(); ++it)
		{
			T* e = dynamic_cast<T*>(it->get());
			if(e)
				ents.push_back(e);
		}
		ents.shrink_to_fit();
		return ents;
	}

	template <typename T>
	std::vector<T const*> findEntities() const
	{
		std::vector<T const*> ents;
		ents.reserve(m_items.size());
		for(auto it = m_items.begin(); it != m_items.end(); ++it)
		{
			T const* e = dynamic_cast<T const*>(it->get());
			if(e)
				ents.push_back(e);
		}
		ents.shrink_to_fit();
		return ents;
	}

	QSize size() const;
	bool checkId() const;
	QFactorioBasicItem* getItem(quint32) const;

	virtual QDataStream& save(QDataStream&) const override;
	virtual QDataStream& load(QDataStream&) override;
};

#endif // QFACTORIOTRACKEDITOR_H
