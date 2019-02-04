#ifndef QFACTORIOCONSTANTS_H
#define QFACTORIOCONSTANTS_H
#include <QByteArray>
#include <QString>
#include <vector>
#include <map>
#include <array>

// NEW ENTITY
enum QFactorioItemType {
	QFIT_CHECKPOINT = 0,
	QFIT_STARTING_AREA,

	QFIT_COUNT,
};

enum {
	QF_USERROLE_ROOT = Qt::UserRole + 1,
	QF_USERROLE_CATEGORY,
	QF_USERROLE_ITEM
};

struct QFItemProperties {
	bool m_assert;
	QByteArray m_mime;
	QString m_name;
};

// Mime Data
extern QString const g_factorioMimeFormat;
extern std::array<QFItemProperties, QFIT_COUNT> const g_mimeEquivalence;

// Declarations
class QFactorioTrackEditor;
class QFactorioItemStorage;

// Empty Template Classes (to be specialized)
template <QFactorioItemType> class QFactorioItem;
template <QFactorioItemType> struct QFactorioTypeSelector;
template <QFactorioItemType> struct QFactorioItemTypeStorage;

// Storage
template <typename T>
using QFactorioItemTypeUnderlyingStorage = std::vector<std::unique_ptr<T>>;

#endif // QFACTORIOCONSTANTS_H
