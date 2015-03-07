#ifndef ROAD_NG_H_
#define ROAD_NG_H_

#include <iostream>
#include <QtGui>

#include "segment.h"
#include "point.h"

class point;
class segment;

/// Object of type road, contains consequent segments, not fully used for now
class road
{
    /// road id
    QString _id;

    /// Ordered segment list
    QList<segment *> _segments;//childs

    /// road name
    QString _name;

    /// road shape, preprocessed
    QPainterPath _shape;
public:
    /**
      * The only constructor
      * \param n Road name
      **/
    road(QString id);

    /// Getter - Returns the final point from the last segment
    point *finalPoint();

    /// Getter - Returns the start point from the first segment
    point *startPoint();

    /// Preprocess shape
    void updateShape();

    /// Add segment to the road, and orders it if its needed
    bool addSegment(segment *);

    /// Getter - returns the list of segments from the road
    QList<segment *> &segments();

    /// Getter - returns the lenght of the whole road
    float length();

    /// Getter - returns the road name
    QString name();

    /// Setter - set road name
    void setName(QString n){_name=n;}

    /// Some info on a string
    QString info();

    /// Setter - sets id
    void setID(QString);

    /// Getter - returns road id
    QString id();

};


#endif /* ROAD_NG_H_ */
