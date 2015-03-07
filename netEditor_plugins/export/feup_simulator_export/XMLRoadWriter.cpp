#include "XMLRoadWriter.h"
#include "lane.h"
#include "realRoad.h"

XMLRoadWriter::XMLRoadWriter(QList<realRoad *> a) {

    roads=a;

    setAutoFormatting(true);
}

bool XMLRoadWriter::writeFile(QIODevice *device) {
    setDevice(device);

    writeStartDocument();
    writeStartElement("Roads");
    for (int i=0; i<roads.size(); i++) {
        currentRealRoad=roads[i];
        writeStartElement("Road");
        {
            qDebug() << "AKIIII" << currentRealRoad->id();
            writeAttribute("id", (currentRealRoad->id()));
            writeAttribute("node1", (currentRealRoad->getFirstNode()->id()));
            writeAttribute("node2", (currentRealRoad->getSecondNode()->id()));

            for (int id=0; id<currentRealRoad->getNodeList().size()-1; id++)
                writeRoadSegment(currentRealRoad->getNodeList()[id], currentRealRoad->getNodeList()[id+1], id);

            writeStartElement("NecessaryLaneToReachNode");
            {

                writeStartElement("NodeLaneToLaneConnection");
                {

                    //first road segment
                    segment *rd=currentRealRoad->getRoadAdjacentToFirstNode();
                    foreach (lane *l1,rd->backwardLanes(currentRealRoad->getFirstNode()))
                    {
                        qDebug() << "l1=" << l1->id() << "road:" << l1->parentSegment()->id();

                        foreach(lane *l2,l1->getForwardLaneConnections())
                        {
                            qDebug() << "Forward -> Connection from " << l1->id() << " to "<<l2->id();
                            writeStartElement("Connection");
                            writeAttribute("from",QString::number(l1->id()));
                            writeAttribute("to",QString::number(l2->id()));

                            point *tmpNode=NULL;
                            foreach (realRoad *rr,roads)
                            {
                                if (rr->contains(currentRealRoad->getFirstNode()) && rr->contains(l2->parentSegment()->otherPoint(currentRealRoad->getFirstNode())) && (rr!=currentRealRoad))
                                {
                                    tmpNode=rr->getTheOtherNode(currentRealRoad->getFirstNode());
                                    break;
                                }
                            }

                            if (tmpNode==NULL)qDebug() << "GRANDE ERRO";
                            writeAttribute("node",(tmpNode->id()));

                            qDebug() << "Forward -> Connection from " << l1->id() << " to "<<l2->id() << " node: " << tmpNode->id();

                            writeEndElement();
                        }
                    }
                    //last road segment
                    rd=currentRealRoad->getRoadAdjacentToSecondNode();

                    foreach (lane *l1,rd->backwardLanes(currentRealRoad->getSecondNode()))
                    {
                        qDebug() << "l1=" << l1->id() << "road:" << l1->parentSegment()->id();
                        foreach(lane *l2,l1->getForwardLaneConnections())
                        {
                            writeStartElement("Connection");
                            writeAttribute("from",QString::number(l1->id()));
                            writeAttribute("to",QString::number(l2->id()));

                            point *tmpNode=NULL;
                            foreach (realRoad *rr,roads)
                            {
                                if (rr->contains(currentRealRoad->getSecondNode()) && rr->contains(l2->parentSegment()->otherPoint(currentRealRoad->getSecondNode())))
                                {	tmpNode=rr->getTheOtherNode(currentRealRoad->getSecondNode());break;}
                            }
                            qDebug() << "Backward -> Connection from " << l1->id() << " to "<<l2->id() << " node: " << tmpNode->id();

                            writeAttribute("node",(tmpNode->id()));
                            writeEndElement();

                        }
                    }

                }
                writeEndElement();

            }
            writeEndElement();//NecessaryLaneToReachNode
            writeEndElement();//Road
        }

    }
    writeEndDocument();
    return 1;
}

void XMLRoadWriter::writeRoadSegment(point *n1, point *n2, int id) {

    segment *currentRoad=n1->getCommonSegment(n2);
    writeStartElement("RoadSegment");
    {
        writeAttribute("id", QString::number(id));
        writeStartElement("AdjacentNodes");
        {
            if (n1->segments().size()>2 || n1->isSourceOrDrain())
                writeTextElement("Connected2Node", (n1->id()));
            if (n2->segments().size()>2 || n2->isSourceOrDrain())
                writeTextElement("Connected2Node", (n2->id()));
        }
        writeEndElement();//AdjacentNodes

        writeTextElement("InitialPosition", QString::number(-1*n1->pos().x()/DEFAULT_SCALE_FACTOR) + " " + QString::number(n1->pos().y()/DEFAULT_SCALE_FACTOR));// (-1)glwidgets coordinates are different from graphicsview!
        writeTextElement("FinalPosition", QString::number(-1*n2->pos().x()/DEFAULT_SCALE_FACTOR) + " " + QString::number(n2->pos().y()/DEFAULT_SCALE_FACTOR));
        writeStartElement("LaneStruct");
        {
            writeTextElement("LaneWidth", QString::number(currentRoad->lanesWidth()));
            writeTextElement("Node1ToNode2NumLanes",
                             QString::number(currentRoad->backwardLanes(n1).size()));
            writeTextElement("Node2ToNode1NumLanes",
                             QString::number(currentRoad->forwardLanes(n1).size()));


            QString tmp;
            if (not currentRoad->finalPoint()->isSourceOrDrain())
                foreach (lane *l,currentRoad->forwardLanes(currentRoad->startPoint()))
                    if (l->getForwardLaneConnections().size()==0)tmp += QString::number(l->id()) + " ";

            if (not currentRoad->finalPoint()->isSourceOrDrain())
                foreach (lane *l,currentRoad->backwardLanes(currentRoad->startPoint()))
                    if (l->getForwardLaneConnections().size()==0)tmp += QString::number(l->id()) + " ";


            writeTextElement("DeadEndLanes", tmp);


            writeStartElement("InterSegmentLaneConnections");
            {
                writeStartElement("ForwardLaneConnections");
                {
                    foreach (lane *sourceLane,currentRoad->forwardLanes(n1))
                    {
                        foreach(lane *destLane,sourceLane->getBackwardLaneConnections())
                        {
                            if (not currentRealRoad->contains(destLane->parentSegment()->otherPoint(n1)))break;
                            writeStartElement("Connection");
                            writeAttribute("from",QString::number(sourceLane->id()));
                            writeAttribute("to",QString::number(destLane->id()));
                            writeEndElement();
                        }
                    }
                    foreach (lane *sourceLane,currentRoad->backwardLanes(n1))
                    {
                        foreach(lane *destLane,sourceLane->getBackwardLaneConnections())
                        {
                            if (not currentRealRoad->contains(destLane->parentSegment()->otherPoint(n2)))break;
                            writeStartElement("Connection");
                            writeAttribute("from",QString::number(sourceLane->id()));
                            writeAttribute("to",QString::number(destLane->id()));
                            writeEndElement();

                        }
                    }

                }
                writeEndElement();

                writeStartElement("BackwardLaneConnections");
                {
                    foreach (lane *sourceLane,currentRoad->forwardLanes(n1))
                    {
                        foreach(lane *destLane,sourceLane->getForwardLaneConnections())
                        {
                            if (not currentRealRoad->contains(destLane->parentSegment()->otherPoint(n2)))break;
                            writeStartElement("Connection");
                            writeAttribute("from",QString::number(sourceLane->id()));
                            writeAttribute("to",QString::number(destLane->id()));
                            writeEndElement();
                        }
                    }
                    foreach (lane *sourceLane,currentRoad->backwardLanes(n1))
                    {
                        foreach(lane *destLane,sourceLane->getForwardLaneConnections())
                        {
                            if (not currentRealRoad->contains(destLane->parentSegment()->otherPoint(n1)))break;
                            writeStartElement("Connection");
                            writeAttribute("from",QString::number(sourceLane->id()));
                            writeAttribute("to",QString::number(destLane->id()));
                            writeEndElement();
                        }
                    }
                }
                writeEndElement();//BackwardLaneConnections
            }
            writeEndElement();//InterSegmentLaneConnections

        }
        writeEndElement();//LaneStruct

        if (id>0)
            writeTextElement("Node1ToNode2BackwardSegment", QString::number(id-1));
        else
            writeTextElement("Node1ToNode2BackwardSegment", "");

        if (id<currentRealRoad->getNodeList().size()-2)
            writeTextElement("Node1ToNode2ForwardSegment", QString::number(id+1));
        else
            writeTextElement("Node1ToNode2ForwardSegment", "");
    }

    writeEndElement();//RoadSegment
}

