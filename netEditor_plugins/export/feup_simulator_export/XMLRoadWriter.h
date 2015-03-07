#ifndef XMLROADWRITER_H_
#define XMLROADWRITER_H_
#include <QXmlStreamWriter>
#include "point.h"
#include "realRoad.h"

class XMLRoadWriter:public QXmlStreamWriter
{
	QList<realRoad *> roads;
	realRoad *currentRealRoad;
public:

	XMLRoadWriter(QList<realRoad *>);
	bool writeFile(QIODevice *);
        void writeRoad(QList <point *> );
        void writeRoadSegment(point *,point *,int);
};

#endif /*XMLROADWRITER_H_*/
