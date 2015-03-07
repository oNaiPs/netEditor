#ifndef DEMAND_WRITER_H
#define DEMAND_WRITER_H

#include "point.h"
#include <QtGui>

class demand_writer:QXmlStreamWriter
{
    QList<point *> points;

public:
    demand_writer(QList<point *>);

    bool writeFile(QIODevice *);
    void writeRoute(point *,int);
    void writevtypeDist(point *,int);

};


#endif // DEMAND_WRITER_H
