#ifndef XMLNODE_H_
#define XMLNODE_H_

#include <QXmlStreamWriter>
#include "point.h"
#include "realRoad.h"


class XMLNodeWriter:public QXmlStreamWriter
{

        QList<point*> nodes;
	QList<realRoad *> roadList;

        qreal getRoadAngleToAxis_connectedToNode(segment *r,point *tmpNode);
	void insertionSort(QList<realRoad *> &,QList<qreal> &);
public:

        XMLNodeWriter(QList <point *>,QList<realRoad *>);
	bool writeFile(QIODevice *);
        void writeNode(point*);

};

#endif /*XMLNODE_H_*/
