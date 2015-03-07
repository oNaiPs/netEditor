#ifndef NODES_READER_H
#define NODES_READER_H

#include <QtGui>
#include "point.h"


/// This class manaages the parsing from a .nod.xml file to a list of netEditor points'
class nodes_reader:public QXmlStreamReader
{

    QHash<QString,point *> pointList;
public:
    nodes_reader(QFile *file);
    QHash<QString,point *> getNodes();

};

#endif // NODES_READER_H
