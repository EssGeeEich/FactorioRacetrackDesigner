#ifndef QCALLBACKITEM_H
#define QCALLBACKITEM_H
#include <QGraphicsItem>
#include <QVariant>
#include <QPainter>
#include <QStyleOptionGraphicsItem>
#include <map>
#include <functional>

template <typename T, T val>
struct DefaultValue {
	T m_val;
	inline DefaultValue()
		: m_val(val) {}
};

template <typename T>
class QCallbackItem : public T
{
	std::map<QGraphicsItem::GraphicsItemChange,
		std::function<void(QVariant const&)>> m_callbacks;
	std::map<int,
		std::function<void(QVariant const&)>> m_userCallbacks;
	DefaultValue<bool, false> m_highlighted;
public:
	using T::T;

	void setHighlighted(bool val)
	{
		if(m_highlighted.m_val != val)
		{
			m_highlighted.m_val = val;
			T::update();
		}
	}

	bool highlighted() const
	{
		return m_highlighted.m_val;
	}

	void setCallback(QGraphicsItem::GraphicsItemChange val,
					 std::function<void(QVariant const&)> fnc)
	{
		m_callbacks[val] = fnc;
	}

	void setUserCallback(int val,
					 std::function<void(QVariant const&)> fnc)
	{
		m_userCallbacks[val] = fnc;
	}

	void runCallback(QGraphicsItem::GraphicsItemChange change, QVariant const& value)
	{
		auto it = m_callbacks.find(change);
		if(it != m_callbacks.end())
			it->second(value);
	}

	void runUserCallback(int change, QVariant const& value)
	{
		auto it = m_userCallbacks.find(change);
		if(it != m_userCallbacks.end())
			it->second(value);
	}

	QVariant itemChange(QGraphicsItem::GraphicsItemChange change,
						QVariant const& value) override
	{
		runCallback(change, value);
		return T::itemChange(change, value);
	}

	void paint(QPainter* painter,
			   const QStyleOptionGraphicsItem* option,
			   QWidget* widget)
	{
		T::paint(painter, option, widget);
		if(highlighted())
		{
			painter->setPen(QPen(QColor(255,255,0), 1, Qt::DashLine));
			painter->setBrush(Qt::NoBrush);
			painter->drawRect(T::boundingRect().adjusted(-2,-2,2,2));
		}
	}
};

#endif // QCALLBACKITEM_H
