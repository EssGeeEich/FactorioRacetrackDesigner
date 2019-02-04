#ifndef QCALLBACKITEM_H
#define QCALLBACKITEM_H
#include <QGraphicsItem>
#include <QVariant>
#include <map>
#include <functional>

template <typename T>
class QCallbackItem : public T
{
	std::map<QGraphicsItem::GraphicsItemChange,
		std::function<void(QVariant const&)>> m_callbacks;
public:
	using T::T;

	void setCallback(QGraphicsItem::GraphicsItemChange val,
					 std::function<void(QVariant const&)> fnc)
	{
		m_callbacks[val] = fnc;
	}

	QVariant itemChange(QGraphicsItem::GraphicsItemChange change,
						QVariant const& value) override
	{
		auto it = m_callbacks.find(change);
		if(it != m_callbacks.end())
			it->second(value);
		return T::itemChange(change, value);
	}
};

#endif // QCALLBACKITEM_H
