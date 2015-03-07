#ifndef CFG_WRITER_H
#define CFG_WRITER_H


#include <QtGui>
#include "sumo_export.h"

class cfg_writer:QXmlStreamWriter
{
sumo_settings *_settings;
public:
    cfg_writer(sumo_settings *s){
    _settings=s;
        setAutoFormatting(true);
}

    bool writeFile(QIODevice *);

};


#endif // CFG_WRITER_H
