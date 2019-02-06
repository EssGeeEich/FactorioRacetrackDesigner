#ifndef QFACTORIOEXPORTER_H
#define QFACTORIOEXPORTER_H
#include "qfactoriotrackeditor.h"
#include <QTextStream>

struct QFactorioExporter {
	static bool ExportScene(QFactorioTrackEditor const&, QTextStream&);
};

#endif // QFACTORIOEXPORTER_H
