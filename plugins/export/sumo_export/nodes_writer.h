#ifndef NODES_WRITER_H
#define NODES_WRITER_H

#include "point.h"
#include <QtGui>

class nodes_writer:public QXmlStreamWriter
{
    QList<point*> points;
public:
    nodes_writer(QList<point*> );
    bool writeFile(QIODevice *);
    void writeNode(point*);
};

#endif // NODES_WRITER_H
