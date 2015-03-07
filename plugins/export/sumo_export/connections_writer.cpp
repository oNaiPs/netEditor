#include "connections_writer.h"



connections_writer::connections_writer(QList<segment *> s)
{
    qDebug() << "connections_writer::connections_writer(QList<segment *> s)";
    segments=s;
    setAutoFormatting(true);
}

bool connections_writer::writeFile(QIODevice *device)
{
    setDevice(device);

    writeStartDocument();
    writeStartElement("connections");

    for (int i=0;i<segments.size();i++)
        writeConnection(segments[i]);
    writeEndElement();
    writeEndDocument();

    return 1;
}

void connections_writer::writeConnection(segment *s)
{
    for (int i=0;i<s->forwardLanes(NULL).size();i++)
    {
        for (int j=0;j<s->forwardLanes(NULL)[i]->originManouvres.size();j++)
        {
            writeStartElement("connection");
            writeAttribute("from",s->id()+QString("_f"));
            lane *l2=s->forwardLanes(NULL)[i]->originManouvres[j]->destiny();
            writeAttribute("to",(l2->parentSegment()->id())+QString(l2->isForwardLane(NULL)?"_f":"_b"));


#ifdef SUMO_012
            writeAttribute("lane",
                           QString::number(s->forwardLanes(NULL).size()-s->forwardLanes(NULL).indexOf(s->forwardLanes(NULL)[i])-1)
                           +":"+(
                               l2->isForwardLane(NULL)?
                                   QString::number(l2->parentSegment()->forwardLanes(NULL).size()-l2->parentSegment()->forwardLanes(NULL).indexOf(l2)-1)
                                 :
                                   QString::number(l2->parentSegment()->backwardLanes(NULL).indexOf(l2))));
#else
            writeAttribute("fromLane",QString::number(s->forwardLanes(NULL).size()-s->forwardLanes(NULL).indexOf(s->forwardLanes(NULL)[i])-1));
            writeAttribute("toLane",(
                               l2->isForwardLane(NULL)?
                                   QString::number(l2->parentSegment()->forwardLanes(NULL).size()-l2->parentSegment()->forwardLanes(NULL).indexOf(l2)-1)
                                 :
                                   QString::number(l2->parentSegment()->backwardLanes(NULL).indexOf(l2))));
#endif

            writeAttribute("spread_type","center");
            writeEndElement();
        }
    }
    for (int i=0;i<s->backwardLanes(NULL).size();i++)
    {
        for (int j=0;j<s->backwardLanes(NULL)[i]->originManouvres.size();j++)
        {
            writeStartElement("connection");
            writeAttribute("from",(s->id())+QString("_b"));
            lane *l2=s->backwardLanes(NULL)[i]->originManouvres[j]->destiny();
            writeAttribute("to",(l2->parentSegment()->id())+QString(l2->isForwardLane(NULL)?"_f":"_b"));
#ifdef SUMO_012
            writeAttribute("lane",
                           QString::number(s->backwardLanes(NULL).indexOf(s->backwardLanes(NULL)[i]))
                           +":"+(
                               l2->isForwardLane(NULL)?
                                   QString::number(l2->parentSegment()->forwardLanes(NULL).size()-l2->parentSegment()->forwardLanes(NULL).indexOf(l2)-1)
                                 :
                                   QString::number(l2->parentSegment()->backwardLanes(NULL).indexOf(l2))));
#else
            writeAttribute("fromLane",QString::number(s->backwardLanes(NULL).indexOf(s->backwardLanes(NULL)[i])));
            writeAttribute("toLane",(
                               l2->isForwardLane(NULL)?
                                   QString::number(l2->parentSegment()->forwardLanes(NULL).size()-l2->parentSegment()->forwardLanes(NULL).indexOf(l2)-1)
                                 :
                                   QString::number(l2->parentSegment()->backwardLanes(NULL).indexOf(l2))));
#endif
            writeAttribute("spread_type","center");
            writeEndElement();
        }
    }
}
