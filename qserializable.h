#ifndef QSERIALIZABLE_H
#define QSERIALIZABLE_H
#include <QDataStream>

class QSerializable {
public:
	virtual ~QSerializable();
	virtual QDataStream& save(QDataStream&) const =0;
	virtual QDataStream& load(QDataStream&) =0;
};

#endif // QSERIALIZABLE_H
