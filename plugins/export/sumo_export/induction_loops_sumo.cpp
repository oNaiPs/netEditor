#include "induction_loops_sumo.h"

induction_loops_sumo::induction_loops_sumo(QList<segment *> s)
{
    setAutoFormatting(true);

    for (int i=0;i<s.size();i++)
        for (int j=0;j<s[i]->lanes().size();j++)
            induction_loops.append(s[i]->lanes()[j]->inductionLoops());


}

bool induction_loops_sumo::writeFile(QIODevice *device)
{
    setDevice(device);

    writeStartDocument();
    writeStartElement("e1-detector");


    for (int i=0;i<induction_loops.size();i++)
    {
        writeAttribute("id",QString::number(i));
        writeAttribute("freq","1");
        writeAttribute("file","editor.out");

        float a=induction_loops[i]->pointPosFromLane();
        QVector<QPointF> b=induction_loops[i]->getParentLane()->parentSegment()->lanePos(induction_loops[i]->getParentLane());

        writeAttribute("pos",QString::number(a*QLineF(b.first(),b.last()).length()));

        //writeAttribute("lane",);
    }

    writeEndElement();
    writeEndDocument();
    return 1;
}
