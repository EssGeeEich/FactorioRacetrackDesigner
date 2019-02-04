#include "qfactorioconstants.h"

template <bool> struct TrueAssert;
template <> struct TrueAssert<true> { static bool ok() { return true; } };

QString const g_factorioMimeFormat = "application/x-factorio-trkitemtype";

// NEW ENTITY
extern std::array<QFItemProperties, QFIT_COUNT> const g_mimeEquivalence =
{{
	// QFIT_CHECKPOINT
	{
		TrueAssert<QFIT_CHECKPOINT == 0>::ok(),
		QByteArray("CKPT"),
		QString::fromUtf8(u8"🏁 Checkpoint")
	},

	// QFIT_STARTING_AREA
	{
		TrueAssert<QFIT_STARTING_AREA == 1>::ok(),
		QByteArray("SPNT"),
		QString::fromUtf8(u8"🚙 Starting Point")
	}
}};
