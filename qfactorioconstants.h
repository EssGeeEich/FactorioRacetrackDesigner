#ifndef QFACTORIOCONSTANTS_H
#define QFACTORIOCONSTANTS_H
#include <QByteArray>
#include <QString>
#include <QRgb>
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

// Palette
enum QFactorioPalette {
	QFP_INVALID = 0,
	QFP_FALSE,
	QFP_TRUE,
	QFP_CONCRETE,
	QFP_DEEPWATER_GREEN,
	QFP_DEEPWATER,
	QFP_DIRT_1,
	QFP_DIRT_2,
	QFP_DIRT_3,
	QFP_DIRT_4,
	QFP_DIRT_5,
	QFP_DIRT_6,
	QFP_DIRT_7,
	QFP_DRY_DIRT,
	QFP_GRASS_1,
	QFP_GRASS_2,
	QFP_GRASS_3,
	QFP_GRASS_4,
	QFP_HAZARD_CONCRETE_LEFT,
	QFP_HAZARD_CONCRETE_RIGHT,
	QFP_LAB_DARK_1,
	QFP_LAB_DARK_2,
	QFP_LAB_WHITE,
	QFP_OUT_OF_MAP,
	QFP_RED_DESERT_0,
	QFP_RED_DESERT_1,
	QFP_RED_DESERT_2,
	QFP_RED_DESERT_3,
	QFP_SAND_1,
	QFP_SAND_2,
	QFP_SAND_3,
	QFP_STONE_PATH,
	QFP_WATER_GREEN,
	QFP_WATER,

	QFP_COUNT
};

static QRgb const g_palette[] = {
	qRgb(0,0,1),		// QFP_INVALID
	qRgb(0,0,2),		// QFP_FALSE
	qRgb(255,255,254),	// QFP_TRUE
	qRgb(99,101,99),	// QFP_CONCRETE
	qRgb(16,36,11),		// QFP_DEEPWATER_GREEN
	qRgb(33,65,74),		// QFP_DEEPWATER
	qRgb(115,105,82),	// QFP_DIRT_1
	qRgb(107,93,74),	// QFP_DIRT_2
	qRgb(99,85,66),		// QFP_DIRT_3
	qRgb(90,73,57),		// QFP_DIRT_4
	qRgb(99,81,66),		// QFP_DIRT_5
	qRgb(44,39,29),		// QFP_DIRT_6
	qRgb(57,48,41),		// QFP_DIRT_7
	qRgb(107,93,66),	// QFP_DRY_DIRT
	qRgb(49,52,24),		// QFP_GRASS_1
	qRgb(57,52,33),		// QFP_GRASS_2
	qRgb(57,56,41),		// QFP_GRASS_3
	qRgb(49,44,33),		// QFP_GRASS_4
	qRgb(123,125,0),	// QFP_HAZARD_CONCRETE_LEFT
	qRgb(123,125,1),	// QFP_HAZARD_CONCRETE_RIGHT
	qRgb(1,0,0),		// QFP_LAB_DARK_1
	qRgb(29,29,29),		// QFP_LAB_DARK_2
	qRgb(255,255,255),	// QFP_LAB_WHITE
	qRgb(0,0,0),		// QFP_OUT_OF_MAP
	qRgb(90,73,57),		// QFP_RED_DESERT_0
	qRgb(107,85,66),	// QFP_RED_DESERT_1
	qRgb(123,93,74),	// QFP_RED_DESERT_2
	qRgb(115,85,74),	// QFP_RED_DESERT_3
	qRgb(148,130,107),	// QFP_SAND_1
	qRgb(132,113,90),	// QFP_SAND_2
	qRgb(123,109,82),	// QFP_SAND_3
	qRgb(29,29,29),		// QFP_STONE_PATH
	qRgb(24,48,16),		// QFP_WATER_GREEN
	qRgb(49,81,90)		// QFP_WATER
};

static_assert((sizeof(g_palette)/sizeof(*g_palette)) == QFP_COUNT, "Invalid palette configuration");

inline QFactorioPalette GetPalette(QRgb color)
{
	for(std::size_t i = 0; i < QFP_COUNT; ++i)
	{
		if(color == g_palette[i])
			return static_cast<QFactorioPalette>(i);
	}
	return QFP_INVALID;
}

inline bool IsSameColor(QRgb origin, QFactorioPalette originPalette, QRgb comparison)
{
	return (origin == comparison) ||
			(GetPalette(comparison) == originPalette);
}

#endif // QFACTORIOCONSTANTS_H
