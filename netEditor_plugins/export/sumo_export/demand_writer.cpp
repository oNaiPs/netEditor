#include "demand_writer.h"

demand_writer::demand_writer(QList<point *> s)
{
    qDebug() << "demand_writer::demand_writer(QList<point *> s)";
    points=s;
    setAutoFormatting(true);

    qDebug() << points.size();
    for (int i=0;i<points.size();i++)
    {
        if (not points[i]->isSource())
        {
            points.removeAt(i);
            i--;
        }
    }

    qDebug() << "Found" << points.size() << "source points";
}

bool demand_writer::writeFile(QIODevice *device)
{
    setDevice(device);

    writeStartDocument();
    writeStartElement("routes");


    qDebug() << "writing " << points.size() << "sources";
    for (int i=0;i<points.size();i++)
    {
        writevtypeDist(points[i],i);
        writeRoute(points[i],i);
    }

//####### FOR AUTONOMOUS
writeStartElement("route");
writeAttribute("id","autonomous_route");
//find edges
QList< QList< point *> > possibleWays;
QString edgeList;

for (int i=0;i<points.size();i++)
{
points[i]->processPossibleWays();
possibleWays=points[i]->getCalculatedPossibleWays();
if (!possibleWays.empty())
    break;
}

for (int k=1;k<possibleWays[0].size();k++)
{
    segment *comSeg=possibleWays[0][k-1]->getCommonSegment(possibleWays[0][k]);
    edgeList.push_back(" ");
    edgeList.push_back((comSeg->id()));
    edgeList.push_back("_");
    edgeList.push_back(comSeg->startPoint()==possibleWays[0][k-1]?"b":"f");

}
writeAttribute("edges",edgeList);
writeAttribute("probability","100");
writeEndElement();
writeStartElement("vtype");
writeAttribute("id","autonomous_type");
writeEndElement();
writeStartElement("vehicle");
writeAttribute("id","autonomous");
writeAttribute("type","autonomous_type");
writeAttribute("route","autonomous_route");
writeAttribute("depart","0");
writeAttribute("color","1,0,1");
writeEndElement();
//######

    writeEndElement();
    writeEndDocument();
    return 1;
}

void demand_writer::writeRoute(point *p,int iddist)
{
    p->processPossibleWays();//fill all empty demands
    QList< QList< point *> > possibleWays=p->getCalculatedPossibleWays();

    if (possibleWays.size()==0)
        return;

    static int idroute=0;
    writeStartElement("routeDistribution");
    writeAttribute("id",QString("routedist")+QString::number(iddist));

    for (int j=0;j<possibleWays.size();j++)
    {
        writeStartElement("route");
        writeAttribute("id",QString("route")+QString::number(++idroute));
        //find edges
        QString edgeList;
        for (int k=1;k<possibleWays[j].size();k++)
        {
            segment *comSeg=possibleWays[j][k-1]->getCommonSegment(possibleWays[j][k]);
            edgeList.push_back(" ");
            edgeList.push_back((comSeg->id()));
            edgeList.push_back("_");
            edgeList.push_back(comSeg->startPoint()==possibleWays[j][k-1]?"b":"f");

        }
        writeAttribute("edges",edgeList);

        writeAttribute("probability",QString::number(p->possibleWaysDistribution[j]));
        writeEndElement();
    }
    writeEndElement();


    writeStartElement("flow");
    writeAttribute("id",QString::number(iddist));
    writeAttribute("type","typedist" + QString::number(iddist));
    writeAttribute("route",QString("routedist")+QString::number(iddist));
    writeAttribute("begin","0");
    writeAttribute("vehsPerHour",QString::number(p->flow()));
    writeAttribute("end","10000");
    writeEndElement();
}


void demand_writer::writevtypeDist(point *p,int iddist)
{
    writeStartElement("vtypeDistribution");
    writeAttribute("id","typedist"+QString::number(iddist));
    writeStartElement("vtype");
    writeAttribute("id","type1"+QString::number(iddist));
    writeAttribute("accel","0.8");
    writeAttribute("lenght","7.5");
    writeAttribute("maxspeed","34");
    writeAttribute("probability",QString::number(p->carsPercent()/100.0));
    writeAttribute("color","0,1,0");
    writeAttribute("guiShape","passenger");
    writeEndElement();

    writeStartElement("vtype");
    writeAttribute("id","type2"+QString::number(iddist));
    writeAttribute("accel","1.8");
    writeAttribute("lenght","18.75");
    writeAttribute("maxspeed","25");
    writeAttribute("probability",QString::number((100-p->carsPercent())/100.0));
    writeAttribute("vclass","transport");
    writeAttribute("guiWidth","2.6");
    writeAttribute("guiShape","bus");
    writeAttribute("color","0.5,0,0");

    writeEndElement();

    writeEndElement();

}
