#ifndef CONNECTIONS_READER_H
#define CONNECTIONS_READER_H


#include <QtGui>
#include "manouvre.h"

class connections_reader:public QXmlStreamReader
{
      QList<lane *> l1s,l2s;
public:
    connections_reader(QFile *file,QHash<QString,segment*> segmentList);
    QList<lane *> getL1s();
    QList<lane *> getL2s();

};
#endif // connections_reader
