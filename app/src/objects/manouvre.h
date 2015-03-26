#ifndef MANOUVRE_H
#define MANOUVRE_H

#include "lane.h"
#include "object.h"

class lane;

/// The class manouvre comprehends a possible movement of a vehicle when changing from a segment to another across an intersection (point) \n\n It appears on the scene when microedition mode is on \n Node: the manouvre can represent a possible way to change segment, or a forbidden way
class manouvre: public object
{
    /// lane of origin, the source of the vehicle
    lane *_origin;
    /// lane of destin, the destination of the vehicle
    lane *_destiny;

    // for drawing purposes
    QLineF l1;
    QLineF l2;
    QPointF p1,p2,delta1,delta2;
    qreal p1r,p2r;

    // for painting purposes
    QPen pen;
    QPainterPath _shape;
    QRectF _boundingRect;
public:
    /// manouvre constructor takes two lanes as input, they can be unordered
    manouvre(lane *lane1,lane *lane2);
    ~manouvre();

    /// Routine called to preprocess the appearence of the manouvre
    void preprocessLook();
    /// Routine called to preprocess the appearence of the manouvre
    void preprocessPath();

    /// Unique key, to identify the manouvre
    QString key();

    /// Getter - returns the origin lane
    lane *origin(){return _origin;}
    /// Getter - returns the destin lane
    lane *destiny(){return _destiny;}

    /**
      * For drawing purposes, to follow the lanes vector
      * \n\n (it will be deprecated when using with lanePos())
      * \param p delta
      * \param l The origin or destin lane to apply the delta
      */
    void setDelta(QPointF p,lane *l);

    /// Render function
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,QWidget *widget);
    /// Returns the preprocessed bounding rectangle
    QRectF boundingRect() const;

    /// returns the other lane than thisLane
    lane *otherLane(lane *thisLane);

};

#endif // MANOUVRE_H
