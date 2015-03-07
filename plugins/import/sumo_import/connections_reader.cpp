#include "connections_reader.h"
#include "point.h"
connections_reader::connections_reader(QFile *file,QHash<QString,segment*> segmentList):QXmlStreamReader(file)
{


    while (!atEnd())
    {
        readNext();

        segment *orig=NULL,*dest=NULL;
        point *origPoint,*destPoint;
        QString origID,destID;
        lane *l1,*l2;
        int origLane=0,destLane=0;

        if (name()=="connection" && tokenType()==QXmlStreamReader::StartElement)
        {
            foreach (QXmlStreamAttribute a,attributes())
            {
                qDebug() << a.value().toString();
                if (a.name()=="from")
                {
                    orig=segmentList.value(a.value().toString(),NULL);
                    origID=a.value().toString();
                }
                else if (a.name()=="to")
                {
                    destID=a.value().toString();
                    dest=segmentList.value(a.value().toString(),NULL);
                }
                else if (a.name()=="lane")
                {
                    QStringList lanes=a.value().toString().split(":");
                    if (lanes.size()!=2)
                        qDebug() << "BAD aa";
                    else
                    {
                        origLane=lanes[0].toInt();
                        destLane=lanes[1].toInt();
                    }
                }
            }

            if (orig==NULL || dest==NULL)
            {
                qDebug() << "ANOTHER E";
                continue;
            }

            if (orig->id()==origID)
                origPoint=orig->startPoint();
            else
                origPoint=orig->finalPoint();

            if (dest->id()==destID)
                destPoint=dest->startPoint();
            else
                destPoint=dest->finalPoint();

            int l1Index=orig->forwardLanes(origPoint).size()-1-origLane;
            if (l1Index<orig->forwardLanes(origPoint).size())
                l1=orig->forwardLanes(origPoint)[l1Index];

            if (destLane <= dest->forwardLanes(destPoint).size()-1)
            {

                int l2Index=dest->forwardLanes(destPoint).size()-1-destLane;
                l2=dest->forwardLanes(destPoint)[l2Index];
                qDebug() << "aki" << l2Index << dest->forwardLanes(destPoint).size();
            }
            else
            {
                qDebug() << "ali";
                int l2Index=destLane - (dest->forwardLanes(destPoint).size()-1);
                if (l2Index<dest->backwardLanes(destPoint).size())
                    l2=dest->backwardLanes(destPoint)[l2Index];
                else
                    qDebug() << "ERROR aqui";
            }


            if (l1!=NULL and l2!=NULL)
            {
                l1s.push_back(l1);
                l2s.push_back(l2);
                qDebug() << "Inserted manouvre: ";// << m->info();
            }
            else
                qDebug() << "Couldnt import manouvre";


        }
    }

    if (hasError())
    {
        QMessageBox::warning(NULL,"Sumo Import Plugin - Error",QString("An error ocurred while reading file " + QFileInfo(*file).fileName() +
                                                                       QString("\nThe error was:\n\n") + errorString()) + QString("\nOn line ")
                             + QString::number(lineNumber()) + QString(" column ") + QString::number(columnNumber()));
    }


}

QList<lane *> connections_reader::getL1s()
{
    return l1s;
}

QList<lane *> connections_reader::getL2s()
{
    return l2s;
}
