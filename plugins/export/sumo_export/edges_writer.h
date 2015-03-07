#ifndef EDGES_WRITER_H
#define EDGES_WRITER_H

#include "segment.h"

class edges_writer:QXmlStreamWriter
{
    QList<segment *>segments;
public:
    edges_writer(QList<segment *>);

    bool writeFile(QIODevice *);
    void writeEdge(segment *);

};

#endif // EDGES_WRITER_H


