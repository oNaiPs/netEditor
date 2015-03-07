#ifndef CONNECTIONS_WRITER_H
#define CONNECTIONS_WRITER_H

#include "lane.h"
#include "segment.h"

class connections_writer:QXmlStreamWriter
{
    QList<segment *>segments;
public:
    connections_writer(QList<segment *>);

    bool writeFile(QIODevice *);
    void writeConnection(segment *);


};

#endif // CONNECTIONS_WRITER_H
