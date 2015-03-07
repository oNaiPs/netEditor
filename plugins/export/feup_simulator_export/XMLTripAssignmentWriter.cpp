#include "XMLTripAssignmentWriter.h"

XMLTripAssignmentWriter::XMLTripAssignmentWriter(QList <point *> nl)
{
    nodeList=nl;
    this->setAutoFormatting(1);
}

bool XMLTripAssignmentWriter::writeFile(QIODevice *d)
{


    setDevice(d);
    writeStartDocument();

    writeStartElement("TripAssignment");
    {
        foreach(point *n, nodeList)
        {
            if (n->isSource())
            {
                writeStartElement("Start");
                {
                    writeAttribute("node", (n->id()));
                    writeAttribute("tripId", (n->id()));

                 //  if (n->possibleWaysChanged())
                        n->processPossibleWays();
                    QList<QList<point *> > pw=n->getCalculatedPossibleWays();
                    for (int i=0; i<pw.size(); i++)
                    {
                        writeStartElement("Path");
                        {

                            writeAttribute("percentage",QString::number(n->possibleWaysDistribution[i]));
                            for (int j=1; j<pw[i].size(); j++)
                            {
                                if (pw[i][j]->segments().size()!=2)
                                {
                                    writeTextElement("Node",(pw[i][j]->id()));
                                }
                            }
                        }
                        writeEndElement();
                    }
                }
                writeEndElement();
            }

        }
    }
    writeEndElement();
    return 1;
}
