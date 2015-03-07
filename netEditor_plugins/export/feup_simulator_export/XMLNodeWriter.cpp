#include "XMLNodeWriter.h"
#include <math.h>
#include "realRoad.h"

#define PI 3.141592654

XMLNodeWriter::XMLNodeWriter(QList<point*> a, QList<realRoad *> b) {
    roadList=b;
    nodes=a;

    setAutoFormatting(true);
}

bool XMLNodeWriter::writeFile(QIODevice *device) {
    setDevice(device);

    writeStartDocument();
    writeStartElement("Nodes");
    foreach (point *n,nodes)
        writeNode(n);
    writeEndDocument();
    return 1;
}

void XMLNodeWriter::writeNode(point *n) {

    if ((n->segments().size()<3 ) and (not (n->isSourceOrDrain())))
        return;

    writeStartElement("Node");
    {
        writeAttribute("id", (n->id()));
        writeAttribute("posx", QString::number(-1*n->pos().x()/DEFAULT_SCALE_FACTOR));//glwidgets coordinates are different from graphicsview!
        writeAttribute("posy", QString::number(n->pos().y()/DEFAULT_SCALE_FACTOR));
        writeAttribute("size", QString::number(10));//int(n->getRadius())*2));//TODO retirar int
        writeAttribute("isExit", QString::number(n->isDrain()));
        writeAttribute("isStart", QString::number(n->isSource()));
        //#############RoadsInCounterClokwise#
        writeStartElement("RoadInCounterClokwise");
        {

            QList<qreal> angles;
            QList<realRoad *> tmpRoads;

            foreach (realRoad *r,roadList) {
                if (n==r->getFirstNode()) {
                    angles.push_back(getRoadAngleToAxis_connectedToNode(r->getRoadAdjacentToFirstNode(), n));
                    tmpRoads.push_back(r);
                }
                else if (n==r->getSecondNode()) {
                    angles.push_back(getRoadAngleToAxis_connectedToNode(
                            r->getRoadAdjacentToSecondNode(), n));
                    tmpRoads.push_back(r);
                }
            }

            insertionSort(tmpRoads, angles);



            foreach (realRoad *r,tmpRoads)
                writeTextElement("Road",(r->id()));

        }
        writeEndElement();//RoadInCounterClokwise

        //############EntryRoadConnections############
        writeStartElement("EntryRoadConnections");
        {
            foreach (realRoad *rr,roadList)
            {
                if (rr->getFirstNode()==n && rr->getRoadAdjacentToFirstNode()->forwardLanes(n).size()>0)
                {
                    writeStartElement("Entry");
                    writeAttribute("fromNode",(rr->getSecondNode()->id()));
                    writeAttribute("road",rr->id());
                    writeEndElement();
                }
                if (rr->getSecondNode()==n && rr->getRoadAdjacentToSecondNode()->forwardLanes(n).size()>0)
                {
                    writeStartElement("Entry");
                    writeAttribute("fromNode",(rr->getFirstNode()->id()));
                    writeAttribute("road",(rr->id()));
                    writeEndElement();
                }
            }
        }
        writeEndElement();//EntryRoadConnections
        //############EntryRoadConnections############
        writeStartElement("ExitRoadConnections");
        {
            foreach (realRoad *rr,roadList)
            {
                if (rr->getFirstNode()==n && rr->getRoadAdjacentToFirstNode()->backwardLanes(n).size()>0)
                {
                    writeStartElement("Exit");
                    writeAttribute("toNode",(rr->getSecondNode()->id()));
                    writeAttribute("road",(rr->id()));
                    writeEndElement();
                }
                if (rr->getSecondNode()==n && rr->getRoadAdjacentToSecondNode()->backwardLanes(n).size()>0)
                {
                    writeStartElement("Exit");
                    writeAttribute("toNode",(rr->getFirstNode()->id()));
                    writeAttribute("road",(rr->id()));
                    writeEndElement();
                }
            }
        }
        writeEndElement();//ExitRoadConnections

        //###############TrafficFlow#################
        writeStartElement("Startup");
        {
            if (n->isSource()) {
                writeTextElement("Flow", QString::number(n->flow()));//n->getFlow()));TODO
                writeTextElement("CarVSTruck",QString::number(n->carsPercent() )     );//n->getCarsPercent()));
                writeTextElement("StartupTripVec",(n->id()));
            }
        }
        writeEndElement();//Startup
    }
    writeEndElement();//Node

    qDebug() << "Node ID=" << n->id() << " parsed.";

}

qreal XMLNodeWriter::getRoadAngleToAxis_connectedToNode(segment *r, point *tmpNode) {
    float angle=0;
    float declive;

    declive=(r->referenceLine().y1()-r->referenceLine().y2())/(r->referenceLine().x1()-r->referenceLine().x2());
    if (declive<0)
        angle+=PI;

    if (tmpNode==r->startPoint()) {
        if (r->referenceLine().p2().y() < r->referenceLine().p1().y())
            angle+=PI;
    } else {
        if (r->referenceLine().p1().y() < r->referenceLine().p2().y())
            angle+=PI;
    }
    angle+=atan(declive);

    return angle;
}

void XMLNodeWriter::insertionSort(QList <realRoad *> & rl,
                                  QList<qreal> &angles) {
    for (int i = 1; i < angles.size(); i++) {
        qreal x = angles[i];
        realRoad *id=rl[i];
        int j;
        for (j = i; j > 0 && x > angles[j-1]; j--) {
            angles[j] = angles[j-1];
            rl[j]=rl[j-1];
        }
        angles[j] = x;
        rl[j]=id;
    }
}

