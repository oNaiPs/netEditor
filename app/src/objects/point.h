#ifndef POINT_H_
#define POINT_H_

#include <QtGui>
#include <QString>
#include "config.h"
#include "object.h"
#include "segment.h"

class segment;

/// Enumerates the intersection type of the point
enum point_type {priority=0,traffic_light,right_before_left};


/// This class describes an intersection: a junction where two or more segments meet.
class point:public object
{
    Q_OBJECT


    static quint32 globalId;
    /// The point type
    point_type _point_type;

    /// The cars flow in cars per hour, for simulation purposes
    qint32 _flow;
    /// The cars percentage,
    qint8 _carsPercent;

    /// If the point is a source point, this array containing an array list of possible ways to the various exits on the network
    QList<QList <point *> > possibleWays;

    /// Last processed possible ways
    QList<QList <point *> > oldPossibleWays;

    /// Array containing all the segments in the intersection
    QList<segment *> _segments;

    /// The point shape , for drawing purposes
    QPainterPath _shape;


    /// Brush used for painting
    QBrush brush;
    QRectF _boundingRect;

public:
    /** The point constructor
      * \param drawingState The initial drawing state of the object
      **/
    point(quint32 drawingState=DEFAULT_POINT_DRAWING_STATE);


    /// Render function - contains all the layering and drawing routines
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *,QWidget *);

    //! Contains the bounding rectangle of the point
    QRectF boundingRect() const;

    /// Returns a contour of the segment's shape
    QPainterPath shape () const;

    /// QGraphicsItem Object type, to recognize it on the scene
    enum { Type = QGraphicsItem::UserType+POINT_TYPE };

    int type () const;

    /// Routine for preprocessing, called when the segment changes its shape
    void preprocessLook();

    QVariant itemChange(GraphicsItemChange change, const QVariant &value);

    /// Estimates the point diameter
    qreal diameter();

    /**
      * Returns true if the point is a source (it contains forward segments only)
      * \n For simulation purposes, a source point is a vehicle generator
      **/
    bool isSource();

    /**
      * Returns true if the point is a drain (it contains backward segments only)
      * \n For simulation purposes, a drain point is a vehicle
      **/
    bool isDrain();

    /// Returns true if the point is source or drain
    bool isSourceOrDrain();

    /// some point info on a string
    QString info();


    /// Getter - returns the list of segments
    QList<segment *> & segments();

    /// Returns true if seg contains this point
    bool containsSegment(segment *seg);

    /// Adds the segment to the intersection
    void addSegment(segment *seg);

    /// Removes the segment from this intersection
    void removeSegment(segment *seg);

    /// Returns the segment containing this point and p1, or NULL if none exists
    segment *getCommonSegment(point *p1);

    /// If containing two segments, returns the other segment than seg
    segment *getTheOtherSegment(segment *seg);

    /// Getter - returns flow
    int flow(){return _flow;}

    /// Getter - returns cars percentage
    int carsPercent(){return _carsPercent;}

    /// Setter - sets flow
    void setFlow(int a){_flow=a;}

    /// Setter - sets cars percentage
    void setCarsPercent(int b){_carsPercent=b;}


    /// Setter - sets the point type
    void setPointType(point_type a){_point_type=a;}

    /// Getter - returns the point tyoe
    point_type pointType(){return _point_type;}

    /**
      * Array containing the probability of a vehicle follow the direction of a possibleWay
      * \n The size of this array is the same of possibleWays, and its index refers to the possibleWay index
      * \returns The percentage of vehicles going on the way defined (range 0-1)
      */
    QList<float> possibleWaysDistribution;

    /// This routine checks if the present calculated possible ways is the same of the previous.
    bool possibleWaysChanged(){return oldPossibleWays!=possibleWays;}

    /// Recursive routine called by processPossibleWays()
    void getPossibleWaysAssistant(point *currentPoint, point *anteriorPoint,QList<point *> list);

    /// Processes the possible ways from the source node to all the drain nodes (only used if the point is source only)
    void processPossibleWays();

    /// Getter - returns processed possible ways
    QList< QList< point *> > getCalculatedPossibleWays(){return possibleWays;}

signals:
    void positionChanged();
public slots:
    void layerChanged(quint32 state);
};
#endif /* POINT_H_ */
