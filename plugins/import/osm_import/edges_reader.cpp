#include "edges_reader.h"

edges_reader::edges_reader(QFile *file,QHash<QString,point *> pointList):QXmlStreamReader(file)
{
    while (!atEnd())
    {
        readNext();

        if (name()=="edge" && tokenType()==QXmlStreamReader::StartElement)
        {


            QString firstNode,secondNode,id;
            int priority=1,nolanes=1;
            float speed=11.111;
            QVector<QPointF> polyline;

            foreach (QXmlStreamAttribute a,attributes())
            {
                if (a.name()=="id")
                    id=a.value().toString();
                else if (a.name()=="fromnode")
                    firstNode=a.value().toString();
                else if (a.name()=="tonode")
                    secondNode=a.value().toString();
                else if (a.name()=="priority")//not implemented yet
                    priority=a.value().toString().toInt();
                else if (a.name()=="nolanes")
                    nolanes=a.value().toString().toInt();
                else if (a.name()=="speed")
                    speed=a.value().toString().toFloat();
		else if (a.name()=="shape")
		{
                    QStringList points=a.value().toString().split(" ");
                    foreach (QString s,points)
                    {
                        QStringList xy=s.split(",");
                        if (xy.size()==2)
                            polyline.push_back(QPointF(xy[0].toInt(),xy[1].toInt()));
                        else
                            qDebug() << "edges_reader::edges_reader BAD POINT";
                    }
		}
            }

            //SUMO has two segments, one with forward lanes, and other with backwardlanes... lets join it...or not
            segment *seg=pointList.value(firstNode)->getCommonSegment(pointList.value(secondNode));
            if (seg==NULL)
            {
                segment *s=new segment(pointList.value(firstNode),pointList.value(secondNode));
                s->setID(id);
                s->setForwardLanesNumber(nolanes);
                segmentList.insert(s->id(),s);
            }
            else
            {
                seg->setBackwardLanesNumber(nolanes);
                segmentList.insert(id,seg);//insert the same segment with another key
            }
        }
    }

    if (hasError())
    {
        QMessageBox::warning(NULL,"Sumo Import Plugin - Error",QString("An error ocurred while reading file " + QFileInfo(*file).fileName() +
                                                                       QString("\nThe error was:\n\n") + errorString()) + QString("\nOn line ")
                             + QString::number(lineNumber()) + QString(" column ") + QString::number(columnNumber()));
    }


}

QHash<QString,segment *> edges_reader::getSegments()
{
    return segmentList;
}
