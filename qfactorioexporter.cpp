#include "qfactorioexporter.h"
#include <QTextStream>
#include "qfactorioitem_checkpoint.h"
#include "qfactorioitem_startingarea.h"

namespace Generated {

	QTextStream& CheckpointRow::Print(QTextStream& s) const
	{
		s << "[" << row << "]={";
		for(auto it = xCoords.begin(); it != xCoords.end(); ++it)
		{
			if(it != xCoords.begin())
				s << ",";
			s << *it;
		}
		s << "}";
		return s;
	}

	QTextStream& CheckpointLevel::Print(QTextStream& s) const
	{
		s << "{\n-- New Checkpoint --\n";
		for(auto it = points.begin(); it != points.end(); ++it)
		{
			if(it != points.begin())
				s << ",\n";
			it->Print(s);
		}
		s << "}";
		return s;
	}

	QTextStream& StartingPoint::Print(QTextStream& s) const
	{
		return s << "{" << px << "," << py << "," << rot << "}";
	}

	QTextStream& LuaFile::Print(QTextStream& s) const
	{
		s <<"mapEnts = {\n"
			"\tstarts = {\n"
			"\t\t"
			"";

		for(auto it = starts.begin(); it != starts.end(); ++it)
		{
			if(it != starts.begin())
				s << ",\n\t\t";
			it->Print(s);
		}
		s <<"\n"
			"\t},\n"
			"\tcheckpoints = {\n"
			"\t\t"
			"";
		for(auto it = levels.begin(); it != levels.end(); ++it)
		{
			if(it != levels.begin())
				s << ",\n";
			it->Print(s);
		}
		s << "\n"
			 "\t}\n"
			 "}";
		return s;
	}

}

bool QFactorioExporter::ExportScene(const QFactorioTrackEditor& m_scene, QTextStream& stream)
{
	Generated::LuaFile genFile;

	// Generate the starting points
	{
		std::vector<QFactorioItem<QFIT_STARTING_AREA> const*> starts =
			m_scene.findEntities<QFactorioItem<QFIT_STARTING_AREA>>();
		for(auto it = starts.begin(); it != starts.end(); ++it)
		{
			QFactorioItem<QFIT_STARTING_AREA> const* p = *it;
			genFile.starts.emplace_back(p->graphics()->pos(), p->graphics()->rotation());
		}
	}

	// Generate the checkpoints
	{
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

			const uchar* bits = imgRaster.bits();
			int w = imgRaster.width();
			int h = imgRaster.height();

			Generated::CheckpointLevel level;
			Generated::CheckpointRow row;
			for(int y = 0; y < h; ++y)
			{
				row.row = y;
				for(int x = 0; x < w; ++x)
				{
					if(bits[(y * w) + x] > 64)
						row.xCoords.push_back(x);
				}

				if(row.xCoords.size())
				{
					level.points.push_back(std::move(row));
					row.xCoords.clear();
				}
			}
			if(level.points.size())
				genFile.levels.push_back(std::move(level));
		}
	}

	genFile.Print(stream);
	return stream.status() == QTextStream::Ok;
}
