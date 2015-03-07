#ifndef SEGMENT_NG_H_
#define SEGMENT_NG_H_

#include <QtGui>

#include "point.h"
#include "lane.h"
#include "road.h"
#include "graphicsScene.h"

class common;
class road;
class lane;
class point;

/// Object of type segment, inherits from QGraphicsItem to be able to be drawn
class segment:public object
{
    Q_OBJECT

    /// parent road id
    road *_road;

    /// Backward Lanes (from end to start node)
    QList<lane*> _backwardLanes,
    /// Forward Lanes (from start to end node)
    _forwardLanes;

    /// Width of child lanes
    float lanes_width;

    /// Polyline describing the shape of the segment
    QVector<QPointF> _polyline;

    /// Same as polyline, but containing the start and end nodes
    QVector<QPointF> _holeSegment;

    /// Object of the start node
    point *start_point,
    /// Object of the end node
    *final_point;


    // Painting

    /// Pens of the various layers to be drawn. Refer to QPen
    QList<QPen> _pens;

    /// Object containing the contour of the segment
    QPainterPath _path;
    QPolygonF _pathPolygon;

    /// Method to draw the direction arrows of the lanes
    void drawDirectionArrow(QLineF,QPainter *,bool);

    /// Method called when the object is changes its state
    void preprocessShape();

    /// Highlight Flag, true is the segment is to be highlighted
    bool highlight;
    QColor highlightColor;

    QVector<QLineF> drawingLines;
    //substituir por defines       [], reference, street, arrows, shape, id
    /// Adds b number of Forward lanes
    void addForwardLanes(int b);
    /// Adds b number of backward lanes
    void addBackwardLanes(int b);

    /// Removes b number of forward lanes
    void removeForwardLanes(int b);
    /// Removes b number of backward lanes
    void removeBackwardLanes(int b);

    /// Method to draw a QVector<QPointF> objects p to the painter
    inline void drawVectorOfPoints(QPainter *painter,QVector<QPointF> p );


    /// Vector containing the position of the various lanes (for drawing)
    QVector< QVector<QPointF> > _lanePos;

    /// Preprocesses the lanes positions
    void calculateLanesPos();

    /// Preprocesses a single lane position
    QVector<QPointF> calculateLanePos(lane *);

public:
    /**
      * Default constructor
      * \param start Start point
      * \param end End point
      * \param dstate Drawing state (provides layering)
      */
    segment(point *start,point * end,quint32 dstate=DEFAULT_SEGMENT_DRAWING_STATE);

    QVariant itemChange(GraphicsItemChange change, const QVariant &value);

    /// Setter - sets parent road (not used)
    void setParentRoad(road *);

    /// Getter - returns parent road
    road *parentRoad();

    //  QPoint pos(){return QPoint(0,0);}
    /// \return QLineF with start and end nodes as p1,p2
    QLineF referenceLine();

    /// Getter - gets start node
    point *startPoint();

    /// Getter - gets end node
    point *finalPoint();

    /// \returns The complementary point of p1
    point *otherPoint(point *p1);

    /// Setter - Sets the inner points of the segment
    bool setPolyline(QVector<QPointF>);

    /// Getter - Gets the inner points of the segment
    QVector<QPointF> polyline(){return _polyline;}
    
    
    /// \returns The lenght of the hole segment
    float lenght();

    /// Setter - Sets the lanes width
    void setLanesWidth(float);

    /// Getter - Gets the lanes width
    float lanesWidth();

    /// Getter - Gets the scene position of the lane l1
    QVector<QPointF> lanePos(lane *l1);

    /// Getter - Gets all the lanes contained by the current segment
    QList<lane *> lanes();

    /// Getter - Gets the start and end nodes
    QList<point *>points();

    /**
      * Setter - Sets the forward lanes number
      * If num > currentFWLanes inserts new lanes
      * else removes the latest created
      */
    void setForwardLanesNumber(int num);
    /**
      * Setter - Sets the backward lanes number
      * If num > currentBWLanes inserts new lanes
      * else removes the latest created
      */
    void setBackwardLanesNumber(int num);

    /** Getter - returns the forward lanes
      * \param p refers to the start point,
      * if p=startpoint returns FWlanes
      * if p=endpoint return BWlanes
      */
    QList<lane *> forwardLanes(point *p=NULL);

    /** Getter - returns the backward lanes
      * \param p refers to the start point,
      * if p=startpoint returns BWlanes
      * if p=endpoint return FWlanes
      */
    QList<lane *> backwardLanes(point *p=NULL);

    /// Render function - contains all the layering and drawing routines
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
               QWidget *widget);

    /// Contains the bounding rectangle of the segment
    QRectF boundingRect() const;

    /// Contains a preprocessed contour of the segment's shape
    const QPainterPath &path();

    /// Routine for preprocessing, called when the segment changes its shape
    void preprocessLook();

    /// Called by preprocessLook(), updates the manouvres within the inner lanes
    void updateManouvres();

    /// QGraphicsItem Object type, to recognize it on the scene
    enum { Type = QGraphicsItem::UserType+SEGMENT_TYPE };

    /// QGraphicsItem Object type, to recognize it on the scene
    int type() const;

    /// \returns The common point of this segment and other, returns NULL if no common point
    point *commonPoint(segment *other);

    /// Setter - Highlight Flag, true is the segment is to be highlighted
    void setHighlighted(bool,QColor c=Qt::red);

    /// Setter - Sets pens
    void setPens();

    /// Called by preprocessLook(), \returns the shape of the lane l1 on the segment
    QPolygonF lanePolygon(lane * l1);

    /// \returns the lane from the segment of p1, NULL if not found
    lane *laneOnPos(QPointF p1);

    /// Returns a contour of the segment's shape
    QPainterPath shape() const;

    /// Some info on a string
    QString info();

public slots:
    void updatePosition();
    void layerChanged(quint32 state);
};

#endif /* SEGMENT_NG_H_ */
