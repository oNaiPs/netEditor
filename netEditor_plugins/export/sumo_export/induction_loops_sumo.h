#ifndef INDUCTION_LOOPS_SUMO_H
#define INDUCTION_LOOPS_SUMO_H

#include <QtGui>
#include "segment.h"
#include "induction_loop.h"
class induction_loops_sumo:QXmlStreamWriter
{
QList<induction_loop *> induction_loops;



public:

    induction_loops_sumo(QList<segment *> );

    bool writeFile(QIODevice *);
};

#endif // INDUCTION_LOOPS_SUMO_H
