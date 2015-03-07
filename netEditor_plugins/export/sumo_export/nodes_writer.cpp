#include "nodes_writer.h"
#include "config.h"

nodes_writer::nodes_writer(QList<point*> p)
{

    points=p;
    setAutoFormatting(true);

}

bool nodes_writer::writeFile(QIODevice *device) {
    setDevice(device);

    writeStartDocument();
    writeStartElement("nodes");
    foreach (point *n,points)
        writeNode(n);

    writeEndElement();
    writeEndDocument();

    return 1;
}


void nodes_writer::writeNode(point *n) {
    writeStartElement("node");
    {

        writeAttribute("id",n->id()+QString("_n"));
        writeAttribute("x", QString::number(n->pos().x()/DEFAULT_SCALE_FACTOR));
        writeAttribute("y", QString::number((-1)*n->pos().y()/DEFAULT_SCALE_FACTOR));

        if (n->pointType()==priority)
            writeAttribute("type", "priority");
        else if (n->pointType()==traffic_light)
            writeAttribute("type", "traffic_light");
        else if (n->pointType()==right_before_left)
            writeAttribute("type", "right_before_left");
        else writeAttribute("type", "priority");
    }
    writeEndElement();//Node

}
