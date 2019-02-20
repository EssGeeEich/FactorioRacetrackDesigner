#include "qfactorioexporter.h"
#include <QTextStream>
#include <QRegExp>
#include "qfactorioitem_checkpoint.h"
#include "qfactorioitem_startingarea.h"
#include "qserializable.h"

namespace LuaGen {

class Obj {
public:
	virtual ~Obj() {}
	virtual QTextStream& save(QTextStream& s) const =0;
	virtual bool isEmpty() { return true; }
};

QTextStream& escapeQStringKey(QTextStream& str, QString const& s)
{
	bool requireExpansion = false;
	for(auto it = s.begin(); it != s.end(); ++it)
	{
		if(it->isLetter() || (it->isNumber() && it != s.begin()))
			continue;
		requireExpansion = true;
		break;
	}

	if(requireExpansion)
	{
		QString eqNum = "=";
		int n = 0;
		while(s.contains("]" + eqNum.repeated(n) + "]"))
			++n;

		str << "[ [" << eqNum.repeated(n) << "[" <<
			 s <<
			 "]" << eqNum.repeated(n) << "] ]";
	}
	else
	{
		str << s;
	}
	return str;
}

QTextStream& escapeQString(QTextStream& str, QString const& s)
{
	bool requireExpansion = false;
	for(auto it = s.begin(); it != s.end(); ++it)
	{
		if(it->isLetterOrNumber() || (*it) == QChar(' '))
			continue;
		requireExpansion = true;
		break;
	}

	if(requireExpansion)
	{
		QString eqNum = "=";
		int n = 0;
		while(s.contains("]" + eqNum.repeated(n) + "]"))
			++n;

		str << " [" << eqNum.repeated(n) << "[" <<
			 s <<
			 "]" << eqNum.repeated(n) << "] ";
	}
	else
	{
		str << "\"" << s << "\"";
	}
	return str;
}

template <typename T>
class PlainObj : public Obj {
	T m_data;
public:
	inline PlainObj(T data) : m_data(std::move(data)) {}
	void setData(T const& d) { m_data = d; }
	T const& data() const { return m_data; }
	T& data() { return m_data; }
	virtual bool isEmpty() override { return false; }
};

class StringObj : public PlainObj<QString> {
	virtual QTextStream& save(QTextStream& s) const override {
		return escapeQString(s, PlainObj<QString>::data());
	}
public:
	using PlainObj<QString>::PlainObj;
};

template <typename T>
class NumericObj : public PlainObj<T> {
	static_assert (std::is_arithmetic<T>::value, "NumericObj<T>: T must be numeric.");
	virtual QTextStream& save(QTextStream& s) const override {
		return s << PlainObj<T>::data();
	}
public:
	using PlainObj<T>::PlainObj;
};

class LiteralStringObj : public PlainObj<QString> {
	virtual QTextStream& save(QTextStream& s) const override {
		return s << PlainObj<QString>::data();
	}
public:
	using PlainObj<QString>::PlainObj;
};

class ArrayObj : public Obj {
	std::vector<std::unique_ptr<Obj>> m_data;
public:
	virtual bool isEmpty() override {
		for(auto it = m_data.begin(); it != m_data.end(); ++it)
		{
			if(*it && (*it)->isEmpty())
				return false;
		}
		return true;
	}
	std::vector<std::unique_ptr<Obj>>& data() {
		return m_data;
	}
	std::vector<std::unique_ptr<Obj>> const& data() const {
		return m_data;
	}
	virtual QTextStream& save(QTextStream& s) const override {
		s << "{";
		for(auto it = m_data.begin(); it != m_data.end(); ++it)
		{
			if(it != m_data.begin())
				s << ",";

			if(*it)
				(*it)->save(s);
			else
				s << "nil";
		}
		s << "}";
		return s;
	}
};

class MapObj : public Obj {
	std::map<QString, std::unique_ptr<Obj>> m_data;
public:
	virtual bool isEmpty() override {
		for(auto it = m_data.begin(); it != m_data.end(); ++it)
		{
			if(!it->second->isEmpty())
				return false;
		}
		return true;
	}
	void compact() {
		auto it = m_data.begin();

		while(it != m_data.end())
		{
			if(it->second->isEmpty())
			{
				it = m_data.erase(it);
			}
			else
			{
				++it;
			}
		}
	}
	std::map<QString, std::unique_ptr<Obj>>& data() {
		return m_data;
	}
	std::map<QString, std::unique_ptr<Obj>> const& data() const{
		return m_data;
	}
	virtual QTextStream& save(QTextStream& s) const override {
		s << "{";
		for(auto it = m_data.begin(); it != m_data.end(); ++it)
		{
			if(it != m_data.begin())
				s << ",";

			bool isInt = false;
			int val = it->first.toInt(&isInt);
			if(isInt)
			{
				s << "[" << val << "]";
			}
			else {
				escapeQStringKey(s,it->first);
			}


			s << "=";
			if(it->second)
				it->second->save(s);
			else
				s << "nil";
		}
		return s << "}";
	}
};

}

bool QFactorioExporter::ExportScene(const QFactorioTrackEditor& m_scene, QTextStream& stream)
{
	LuaGen::MapObj genFile;
	LuaGen::MapObj info;
	LuaGen::MapObj trackFile;

	// Generate the starting points
	{
		LuaGen::ArrayObj* startPoints = new LuaGen::ArrayObj;
		info.data()["starts"].reset(startPoints);

		std::vector<QFactorioItem<QFIT_STARTING_AREA> const*> starts =
			m_scene.findEntities<QFactorioItem<QFIT_STARTING_AREA>>();
		for(auto it = starts.begin(); it != starts.end(); ++it)
		{
			QFactorioItem<QFIT_STARTING_AREA> const* p = *it;
			LuaGen::MapObj* startPoint = new LuaGen::MapObj;
			startPoints->data().emplace_back(startPoint);

			QPointF pos = p->graphics()->pos();
			double rot = p->graphics()->rotation();
			startPoint->data()["X"].reset(new LuaGen::NumericObj<double>(
				pos.x()
			));
			startPoint->data()["Y"].reset(new LuaGen::NumericObj<double>(
				pos.y()
			));
			startPoint->data()["R"].reset(new LuaGen::NumericObj<double>(
				rot
			));
		}
	}

	LuaGen::MapObj* objectMap = new LuaGen::MapObj;
	genFile.data()["map"].reset(objectMap);

	auto getCoord = [=](int x, int y, QString subObj) -> LuaGen::MapObj*
	{
		std::unique_ptr<LuaGen::Obj>& xo = objectMap->data()[QString::number(x)];
		if(!xo)
			xo.reset(new LuaGen::MapObj);
		LuaGen::MapObj* xo_map = dynamic_cast<LuaGen::MapObj*>(xo.get());
		if(!xo_map)
			return nullptr;

		std::unique_ptr<LuaGen::Obj>& yo = xo_map->data()[QString::number(y)];
		if(!yo)
			yo.reset(new LuaGen::MapObj);
		LuaGen::MapObj* yo_map = dynamic_cast<LuaGen::MapObj*>(yo.get());
		if(!yo_map)
			return nullptr;

		if(subObj.isEmpty())
			return yo_map;

		std::unique_ptr<LuaGen::Obj>& sz = yo_map->data()[subObj];
		if(!sz)
			sz.reset(new LuaGen::MapObj);
		return dynamic_cast<LuaGen::MapObj*>(sz.get());
	};

	// Generate the checkpoints
	{
		LuaGen::ArrayObj* checkPoints = new LuaGen::ArrayObj;
		info.data()["checkpoints"].reset(checkPoints);

		std::vector<QFactorioItem<QFIT_CHECKPOINT> const*> checkpoints =
			m_scene.findEntities<QFactorioItem<QFIT_CHECKPOINT>>();

		QPen checkpointPen(Qt::SolidPattern, 1.0);
		checkpointPen.setColor(QColor(255,255,255,255));

		for(quint32 i = 1; ; ++i)
		{
			QImage imgRaster(m_scene.size(), QImage::Format_Grayscale8);
			imgRaster.fill(QColor(0,0,0));

			QPainter raster(&imgRaster);
			raster.setPen(checkpointPen);

			quint32 count = 0;
			for(auto it = checkpoints.begin(); it != checkpoints.end(); ++it)
			{
				QFactorioItem<QFIT_CHECKPOINT> const* p = *it;
				if(p->sequenceId() == i)
				{
					++count;
					raster.translate(p->graphics()->pos());
					raster.drawLine(p->graphics()->line());
				}
			}
			if(!count)
				break;

			LuaGen::MapObj* checkpointInfo = new LuaGen::MapObj;
			checkPoints->data().emplace_back(checkpointInfo);
			checkpointInfo->data()["Enabled"].reset(new LuaGen::LiteralStringObj("true"));

			const uchar* bits = imgRaster.bits();
			int w = imgRaster.width();
			int h = imgRaster.height();

			for(int x = 0; x < w; ++x)
			{
				for(int y = 0; y < h; ++y)
				{
					if(bits[(y * w) + x] > 64)
					{
						LuaGen::MapObj* mapSpot = getCoord(x, y, "checkpoints");

						mapSpot->data()[QString::number(i)].reset(
							new LuaGen::LiteralStringObj("info.checkpoints[" + QString::number(i) + "]")
						);
					}
				}
			}
		}
	}

	// Generate the Track File
	{
		QPixmap pixmap = m_scene.backgroundPixmap();
		QImage img;
		if(!pixmap.isNull())
		{
			img = pixmap.toImage();
		}
		if(!img.isNull())
		{
			trackFile.data()["width"].reset(new LuaGen::NumericObj<int>(img.width()));
			trackFile.data()["height"].reset(new LuaGen::NumericObj<int>(img.height()));
			LuaGen::ArrayObj* rows = new LuaGen::ArrayObj;
			trackFile.data()["data"].reset(rows);

			LuaGen::ArrayObj* items = nullptr;
			auto push_int = [&](int num) {
				items->data().push_back(std::unique_ptr<LuaGen::Obj>(new LuaGen::NumericObj<int>(
					num
				)));
			};
			for(int y = 0; y < img.height(); ++y)
			{
				items = new LuaGen::ArrayObj;
				rows->data().push_back(std::unique_ptr<LuaGen::Obj>(items));

				for(int x = 0; x < img.width(); )
				{
					QRgb firstPixel = img.pixel(x,y);
					QFactorioPalette firstPalette = GetPalette(firstPixel);

					int count = 1;
					while(
						((count+x) < img.width()) &&
						IsSameColor(firstPixel, firstPalette, img.pixel(x+count,y))
					)
						++count;

					push_int(static_cast<int>(firstPalette));
					push_int(count);
					x += count;
				}
			}
		}
	}

	objectMap->compact();

	stream << "local info=";
	info.save(stream);

	stream << "\n"
			  "local gen=";
	genFile.save(stream);

	stream << "\n"
			  "local track=";
	trackFile.save(stream);
	stream << "\n"
			  "return {info=info,gen=gen,track=track}";
	return stream.status() == QTextStream::Ok;
}
