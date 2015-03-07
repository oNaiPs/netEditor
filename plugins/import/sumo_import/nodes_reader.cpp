#include "nodes_reader.h"

nodes_reader::nodes_reader(QFile *file):QXmlStreamReader(file)
{


    while (!atEnd())
    {
        readNext();

        if (name()=="node" && tokenType()==QXmlStreamReader::StartElement)
        {
            point *p=new point();
            foreach (QXmlStreamAttribute a,attributes())
            {
                if (a.name()=="id")
                    p->setID(a.value().toString());
                else if (a.name()=="x")
                    p->setX(a.value().toString().toFloat());
                else if (a.name()=="y")
                    p->setY(-a.value().toString().toFloat());
                else if (a.name()=="type") //TODO find out a better way to enum this
                {
                    if (a.value()=="priority")
                        p->setPointType(priority);
                    else if (a.value()=="traffic_light")
                        p->setPointType(traffic_light);
                    else if (a.value()=="right_before_left")
                        p->setPointType(right_before_left);
                }
            }
            pointList.insert(p->id(),p);
            qDebug() << "Inserted point: " << p->info();
        }
    }

    if (hasError())
    {
        QMessageBox::warning(NULL,"Sumo Import Plugin - Error",QString("An error ocurred while reading file " + QFileInfo(*file).fileName() +
                                                                       QString("\nThe error was:\n\n") + errorString()) + QString("\nOn line ")
                             + QString::number(lineNumber()) + QString(" column ") + QString::number(columnNumber()));
    }


}

QHash<QString,point *> nodes_reader::getNodes()
{
return pointList;
}
