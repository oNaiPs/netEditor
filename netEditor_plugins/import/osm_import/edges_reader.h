#ifndef EDGES_READER_H
#define EDGES_READER_H

#include <QtGui>
#include "segment.h"

class edges_reader:public QXmlStreamReader
{
     QHash<QString,segment *> segmentList;
public:
    edges_reader(QFile *file,QHash<QString,point *>);
    QHash<QString,segment *> getSegments();

};

#endif // EDGES_READER_H
