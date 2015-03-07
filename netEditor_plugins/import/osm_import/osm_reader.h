#ifndef OSM_READER_H
#define OSM_READER_H

#include <QXmlStreamReader>
#include <QtGui>
#include "point.h"
#include "segment.h"
#include "osm_import.h"


class osm_reader : public QXmlStreamReader
{
    QHash<QString,point *> pointList;
    QPointF toMercator(float,float);
    QPointF toMercator (QPointF p);

    QRectF bbox;


public:
    osm_reader(QIODevice *,QRectF,osm_import *osm);

    QHash<QString,point *> getPoints();
    QHash<QString,segment *> getSegments();


};

#endif // OSM_READER_H
