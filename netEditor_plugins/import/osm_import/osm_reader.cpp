#include "osm_reader.h"
#include "point.h"
#include "segment.h"
#include <GeographicLib/LocalCartesian.hpp>

osm_reader::osm_reader(QIODevice *io,QRectF bb,osm_import *osm):QXmlStreamReader(io),bbox(bb)
{

    qDebug() << center;
    bool alreadyRead=false;
    QPointF center=bb.center();
    QString roadName;


    GeographicLib::LocalCartesian coord_convert(center.x(),center.y());
    qDebug() << "Center Point:" << center;


    QHash<point*,bool> usedBySegment;

    while (!atEnd())
    {

        if (!alreadyRead)
            readNext();
        else
            alreadyRead=false;

        if (name()=="node" && tokenType()==QXmlStreamReader::StartElement)
        {
            QString id,lat,lon;
            foreach (QXmlStreamAttribute a,attributes())
            {
                if (a.name()=="id")
                    id=a.value().toString();
                else if (a.name()=="lat")
                    lat=a.value().toString();
                else if (a.name()=="lon")
                    lon=a.value().toString();
            }

            /*if (!bb.contains(QPointF(lat.toFloat(),lon.toFloat())))
                continue;*/

            point *p=new point();
            double x,y,z;
            coord_convert.Forward(lat.toFloat(),lon.toFloat(),0,x,y,z);
            p->setPos(QPointF(x,-y));
            p->setID(id);
            qDebug() << "creating point id=" << id << lat.toDouble() << lon.toDouble() << QPointF(x,y);
            pointList.insert(p->id(),p);
        }
        else if (name()=="way" && tokenType()==QXmlStreamReader::StartElement)
        {
            bool oneway=false;
            int noLanes=1;
            QString id;
            foreach (QXmlStreamAttribute a,attributes())
            {
                if (a.name()=="id")
                    id=a.value().toString();
            }

            QList<QString> pointIdList;
            bool highway=false;
            do
            {

                if (!atEnd())
                    readNext();
                //                qDebug() << name();

                if (name()=="nd"&& tokenType()==QXmlStreamReader::StartElement)
                {
                    foreach (QXmlStreamAttribute a,attributes())
                    {
                        if (a.name()=="ref")
                            pointIdList.push_back(a.value().toString());
                    }
                }
                else if (name()=="tag"&& tokenType()==QXmlStreamReader::StartElement)
                {
                    QString k;
                    QString v;

                    foreach (QXmlStreamAttribute a,attributes())
                    {
                        if (a.name()=="k")
                            k=a.value().toString();
                        else if (a.name()=="v")
                            v=a.value().toString();
                    }


                    if (k=="oneway")
                        oneway=true;
                    else if (k=="highway")
                    {
                        if (v=="motorway")
                            noLanes=2;


                        highway=true;
                    }
                    else if (k=="lanes")
                        noLanes=2;
                    else if (k=="name")
                        roadName=v;

                }
                //qDebug() << "token=" << tokenType();

            }
            while (((name()=="nd" || name()=="tag")&& tokenType()==QXmlStreamReader::StartElement)||
                   tokenType()==QXmlStreamReader::EndElement||
                   tokenType()==QXmlStreamReader::Characters);

            alreadyRead=true;

            if (!highway)
                pointIdList.clear();


            road *_r=new road(id);
            _r->setName(roadName);

            for (int i=0;i<pointIdList.size()-1;i++)
            {

                point *p1,*p2;
                p1=pointList.value(pointIdList[i],NULL);
                p2=pointList.value(pointIdList[i+1],NULL);

                if (p1==NULL || p2==NULL)
                {
                    qDebug() << "EEEERRRRRRO";
                    continue;
                }

                //lets just add points that have segments
                if (!usedBySegment.value(p1))
                    osm->addPoint(p1);
                if (!usedBySegment.value(p2))
                    osm->addPoint(p2);

                usedBySegment.insert(p1,true);
                usedBySegment.insert(p2,true);

                segment *s=osm->createSegment(p1,p2,id+QString::number(i));
                if (!s)
                {
                    qDebug() << "Couldnt create segment, with p1=" << p1->id() << "p2=" << p2->id();
                    continue;
                }

                if (!oneway)
                {
                    s->setForwardLanesNumber(noLanes);
                    s->setBackwardLanesNumber(noLanes);
                }
                else
                    s->setForwardLanesNumber(noLanes);

                _r->addSegment(s);
            }
            osm->addRoad(_r);
        }
        //        else qDebug() << "NOT " << name();
    }

    if (hasError())
    {
        QMessageBox::warning(NULL,"Sumo Import Plugin - Error",QString("An error ocurred while reading OSM file "+
                                                                       QString("\nThe error was:\n\n") + errorString()) + QString("\nOn line ")
                             + QString::number(lineNumber()) + QString(" column ") + QString::number(columnNumber()));
    }

}


