#ifndef QFACTORIOEXPORTER_H
#define QFACTORIOEXPORTER_H
#include "qfactoriotrackeditor.h"
#include <QTextStream>

struct QFactorioExporter {
	static bool ExportScene(QFactorioTrackEditor const&, QTextStream&);
};

namespace Generated {
	struct CheckpointRow {
		std::vector<int> yCoords;
		int row;
		QTextStream& Print(QTextStream&) const;
	};
	struct CheckpointLevel {
		std::vector<CheckpointRow> points;
		QTextStream& Print(QTextStream&) const;
	};
	struct StartingPoint {
		inline StartingPoint(QPointF const& pos = QPointF(), qreal rot = 0.0)
			: px(pos.x()), py(pos.y()), rot(rot) {}
		qreal px;
		qreal py;
		qreal rot;

		QTextStream& Print(QTextStream&) const;
	};

	struct LuaFile {
		std::vector<CheckpointLevel> levels;
		std::vector<StartingPoint> starts;

		QTextStream& Print(QTextStream&) const;
	};
}

#endif // QFACTORIOEXPORTER_H
