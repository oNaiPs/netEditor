#include "cfg_writer.h"

bool cfg_writer::writeFile(QIODevice *device)
{
    qDebug() << "cfg_writer(); writefile";
    setDevice(device);

    writeStartDocument();
    writeStartElement("configuration");
    writeStartElement("files");
    writeTextElement("net-file","editor.net.xml");
    writeTextElement("route-files","editor.rou.xml");
    writeEndElement();
    writeStartElement("simulation");
    writeTextElement("begin","0");
    writeTextElement("end","10000");
    writeEndElement();
    if (_settings->traciEnabled())
    {
                writeTextElement("remote-port",_settings->traciPort());
    }
    writeEndElement();
    writeEndDocument();

    return 1;


}

