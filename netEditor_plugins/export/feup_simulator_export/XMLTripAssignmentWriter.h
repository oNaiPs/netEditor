#ifndef XMLTRIPASSIGNMENTWRITER_H_
#define XMLTRIPASSIGNMENTWRITER_H_

#include <QXmlStreamWriter>
#include "point.h"

class XMLTripAssignmentWriter : public QXmlStreamWriter {

        QList <point *> nodeList;

public:

        XMLTripAssignmentWriter(QList <point *>);

	bool writeFile(QIODevice *);
};

#endif /*XMLTRIPASSIGNMENTWRITER_H_*/
