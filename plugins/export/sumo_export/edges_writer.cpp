#include "edges_writer.h"

edges_writer::edges_writer(QList<segment *> s)
{
    qDebug() << "edges_writer::edges_writer(QList<segment *> s)";
    segments=s;
    setAutoFormatting(true);

}

bool edges_writer::writeFile(QIODevice *device)
{
    setDevice(device);

    writeStartDocument();
    writeStartElement("edges");
    qDebug() << "writing " << segments.size() << "edges";
    for (int i=0;i<segments.size();i++)
        writeEdge(segments[i]);
    writeEndElement();
    writeEndDocument();
    return 1;
}

void edges_writer::writeEdge(segment *s)
{

    if (s->forwardLanes(NULL).size()>0)
    {
        //forward lanes
        writeStartElement("edge");
        writeAttribute("id",(s->id())+QString("_f"));

#ifdef SUMO_012
        writeAttribute("fromnode",s->finalPoint()->id()+QString("_n"));
        writeAttribute("tonode",s->startPoint()->id()+QString("_n"));
#else
        writeAttribute("from",s->finalPoint()->id()+QString("_n"));
        writeAttribute("to",s->startPoint()->id()+QString("_n"));
#endif

        writeAttribute("priority","1");


#ifdef SUMO_012
        writeAttribute("nolanes",QString::number(s->forwardLanes(NULL).size()));
#else
        writeAttribute("numLanes",QString::number(s->forwardLanes(NULL).size()));
#endif
        writeAttribute("speed","11.111");
        if (s->polyline().size())
        {
            QString str;
            for (int i=0;i<s->polyline().size();i++)
            {
                str.push_back(QString::number(s->polyline()[i].x()));
                str.push_back(",");
                str.push_back(QString::number(-s->polyline()[i].y()));

                if (i+1!=s->polyline().size())//prevent exception from sumo
                    str.push_back(" ");
            }
            writeAttribute("shape",str);
        }

        //writeAttribute("spread_type","center");it should be here, but there is a bug on sumo
        writeEndElement();
    }
    if (s->backwardLanes(NULL).size()>0)
    {
        //backward lanes
        writeStartElement("edge");
        writeAttribute("id",(s->id())+QString("_b"));
#ifdef SUMO_012
        writeAttribute("fromnode",(s->startPoint()->id())+QString("_n"));
        writeAttribute("tonode",(s->finalPoint()->id())+QString("_n"));
#else
        writeAttribute("from",(s->startPoint()->id())+QString("_n"));
        writeAttribute("to",(s->finalPoint()->id())+QString("_n"));
#endif
        writeAttribute("priority","1");
#ifdef SUMO_012
        writeAttribute("nolanes",QString::number(s->backwardLanes(NULL).size()));
#else
                writeAttribute("numLanes",QString::number(s->backwardLanes(NULL).size()));
#endif
        writeAttribute("speed","11.111");
        if (s->polyline().size())
        {
            QString str;
            for (int i=s->polyline().size()-1;i>=0;i--)
            {
                str.push_back(QString::number(s->polyline()[i].x()));
                str.push_back(",");
                str.push_back(QString::number(-s->polyline()[i].y()));

                if (i!=0)//prevent exception from sumo
                    str.push_back(" ");
            }
            writeAttribute("shape",str);
        }

        //writeAttribute("spread_type","center");it should be here, but there is a bug on sumo
        writeEndElement();
    }


}
