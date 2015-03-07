/*
 * segment_ng.cpp
 *
 *  Created on: Nov 30, 2008
 *      Author: root
 */

#include "common.h"
#include "config.h"
#include "segment.h"
#include <QtGui>


segment::segment(point *s,point* f,quint32 dstate)
{
    //    tipo = "segment";
    qDebug() << "created segment";
    final_point = s;
    start_point = f;
    _road=NULL;
    lanes_width=DEFAULT_LANE_WIDTH;
    highlight=false;
    setDrawingState(dstate);

    connect(start_point,SIGNAL(positionChanged()),this,SLOT(updatePosition()));
    connect(final_point,SIGNAL(positionChanged()),this,SLOT(updatePosition()));

    s->addSegment(this);
    f->addSegment(this);

    setPens();
    setZValue(-2);

    //setCacheMode(QGraphicsItem::DeviceCoordinateCache);
    //setFlags(QGraphicsItem::ItemIsSelectable|QGraphicsItem::ItemIsMovable|QGraphicsItem::ItemSendsGeometryChanges);
    setFlags(QGraphicsItem::ItemIsSelectable|QGraphicsItem::ItemSendsGeometryChanges);

}

int segment::type() const
{
    return QGraphicsItem::UserType+SEGMENT_TYPE;
}


void segment::setLanesWidth(float f)
{
    lanes_width = f;
}

float segment::lanesWidth()
{
    return lanes_width;
}

point *segment::finalPoint()
{
    Q_ASSERT_X(final_point != NULL,"final point", "ERRO should not happen!");
    return final_point;
}
point *segment::startPoint()
{
    Q_ASSERT_X(start_point != NULL,"start point", "ERRO should not happen!");
    return start_point;
}

point *segment::otherPoint(point *p)
{
    if (p==finalPoint())
        return startPoint();
    else if (p==startPoint())
        return finalPoint();
    else
        return NULL;

}

bool segment::setPolyline(QVector<QPointF> v)
{
    if (v.size()<1)
        return 0;

    _polyline = v;
    
    Q_ASSERT_X((start_point!=NULL) && (final_point!=NULL),"setPolyline","points should not be null!");

    preprocessLook();
    update();
    return 1;
}

const QPainterPath &segment::path()
{
    return _path;
}


QLineF segment::referenceLine()
{
    return QLineF(startPoint()->pos(),finalPoint()->pos());
}

inline void segment::drawVectorOfPoints(QPainter *painter,QVector<QPointF> p)
{
    for (int i=0;i<p.size()-1;i++)
        painter->drawLine(p[i],p[i+1]);
}



void segment::paint(QPainter *painter, const QStyleOptionGraphicsItem *,QWidget *)
{
    //painter->drawLine(start_point->pos(),final_point->pos());

    if (GET_BIT(drawingState(),OBJECT_TOGGLE))
    {
        
        if (GET_BIT(drawingState(),SEGMENT_REFERENCE))
        {
            for (int i=0;i<_holeSegment.size()-1;i++)
                painter->drawLine(_holeSegment[i],_holeSegment[i+1]);
        }
        
        if (GET_BIT(drawingState(),SEGMENT_STREET) || highlight)
        {
            if (isSelected())
                painter->setBrush(Qt::darkGray);
            else if (highlight)
            {
                QColor c(highlightColor);
                painter->setBrush(c);
                painter->setPen(highlightColor);
            }
            else
            {
                QColor c(Qt::black);
                c.setAlpha(100);
                painter->setBrush(c);
                painter->setPen(Qt::black);
            }
            
            painter->drawPolygon(_pathPolygon);
        }
        if (GET_BIT(drawingState(),SEGMENT_ARROWS))
        {
            painter->setPen(Qt::white);
            for (int j=0;j<lanes().size();j++)
                for (int i=0;i<lanePos(lanes()[j]).size()-1;i++)
                    drawDirectionArrow(QLineF(lanePos(lanes()[j])[i+1],lanePos(lanes()[j])[i]),painter,_backwardLanes.contains(lanes()[j]));
        }
        if (GET_BIT(drawingState(),SEGMENT_SHAPE))
        {
            QPen pe(Qt::white);
            pe.setWidth(1);
            pe.setStyle(Qt::DashLine);
            painter->setPen(pe);
            for (int i=0;i<drawingLines.size();i++)
                painter->drawLine(drawingLines[i]);
        }
        if (GET_BIT(drawingState(),SEGMENT_ID))
        {
            painter->drawText(_path.boundingRect(),id());
        }
    }

}

QRectF segment::boundingRect() const
{
    return _path.boundingRect();
}

QPainterPath segment::shape() const
{
    return _path;
}

float segment::lenght()
{



    return 0;//TODO
}

QString segment::info()
{
    QString tmp("Segment id:");
    tmp.push_back(id());
    tmp.push_back(" startPoint:");
    tmp.push_back(QString::number(start_point->x()));
    tmp.push_back(" y:");
    tmp.push_back(QString::number(start_point->y()));
    tmp.push_back(" finalPoint:");
    tmp.push_back(QString::number(final_point->x()));
    tmp.push_back(" y:");
    tmp.push_back(QString::number(final_point->y()));
    tmp.push_back(" lanes_width:");
    tmp.push_back(QString::number(lanes_width));
    
    return tmp;
}




QVariant segment::itemChange(GraphicsItemChange change, const QVariant &value)
{

    if (change==QGraphicsItem::ItemPositionChange)
    {
        qDebug()<< "KAII" << value;
        QPointF d=value.toPointF()-pos();


      start_point->moveBy(d.x(),d.y());
      final_point->moveBy(d.x(),d.y());

    }
    return QGraphicsItem::itemChange(change,value);
}

road *segment::parentRoad()
{
    return _road;
}

void segment::setParentRoad(road *r)
{
    _road = r;
    r->addSegment(this);
}

void segment::setPens()
{
    _pens.push_back(QPen(Qt::blue));
    //QPen p(Qt::DotLine);
    QPen p;
    //p.setBrush(QBrush(Qt::blue, Qt::Dense4Pattern));
    p.setColor(Qt::blue);
    _pens.push_back(p);
    _pens.push_back(QPen(Qt::red));
}



QList<lane *> segment::lanes()
{
    return _backwardLanes+_forwardLanes;
}

QList<point *> segment::points()
{
    QList<point*> _points;
    _points.push_back(start_point);
    _points.push_back(final_point);
    
    return _points;
}

void segment::preprocessShape()
{
    qDebug() << "inline void segment::preprocessShape()";

    if (_holeSegment.size()<2)
    {
        qDebug() << "FATAL";
        return;
    }
    QPainterPath p(_holeSegment[0]);
    for (int i=1;i<_holeSegment.size();i++)
        p.lineTo(_holeSegment[i]);

    QPainterPathStroker st;
    st.setCapStyle(Qt::RoundCap);
    st.setJoinStyle(Qt::RoundJoin);
    st.setWidth(lanesWidth()*lanes().size());

    _path=(st.createStroke(p)).simplified();
    _pathPolygon=_path.toFillPolygon();
}

void segment::preprocessLook()
{
    qDebug() << "void segment::preprocessLook() id:" << this->id();

    // updating the hole segment
    _holeSegment.clear();
    _holeSegment.push_back(final_point->pos());
    _holeSegment+=_polyline;
    _holeSegment.push_back(start_point->pos());

    calculateLanesPos();


    if (startPoint()==NULL || finalPoint()==NULL){
        qDebug() << "returning from preprocesslook(), this condition should never happen";
        return;
    }

    drawingLines.clear();

    if (lanes().size()==0)
    {
        preprocessShape();
        return;
    }
    prepareGeometryChange();

    for (int i=0;i<_holeSegment.size()-1;i++)
    {
        QPointF perp,_perp_norm;

        _perp_norm=perp_norm(QLineF(_holeSegment[i],_holeSegment[i+1]));
        _perp_norm*=lanes_width;

        perp=_perp_norm*((lanes().size()/2.0));

        for (int j=0;j<lanes().size()-1;j++)
        {
            perp-=_perp_norm;
            drawingLines.push_back(QLineF(_holeSegment[i]+perp,_holeSegment[i+1]+perp));
        }
    }

    preprocessShape();
    update();
}



void segment::addForwardLanes(int n)
{

    for (int i=0;i<n;i++)
    {
        _forwardLanes.push_back(new lane(this));
    }

}

void segment::addBackwardLanes(int n)
{
    for (int i=0;i<n;i++)
    {
        _backwardLanes.push_front(new lane(this));
    }
}

QList<lane *> segment::forwardLanes(point *p)
{
    if (p==final_point)
        return _backwardLanes;
    else
        return _forwardLanes;
}
QList<lane *> segment::backwardLanes(point *p)
{
    if (p==final_point)
        return _forwardLanes;
    else
        return _backwardLanes;
}

void segment::setForwardLanesNumber(int n)
{
    if (_forwardLanes.size()>n)
        removeForwardLanes(_forwardLanes.size()-n);
    else
        addForwardLanes(n-_forwardLanes.size());

    foreach (point* p,points())
        p->preprocessLook();

    calculateLanesPos();
}
void segment::setBackwardLanesNumber(int n)
{
    if (_backwardLanes.size()>n)
        removeBackwardLanes(_backwardLanes.size()-n);
    else
        addBackwardLanes(n-_backwardLanes.size());

    foreach (point* p,points())
        p->preprocessLook();

    calculateLanesPos();
}

inline void segment::drawDirectionArrow(QLineF pos,QPainter *painter,bool direction)
{
    QPointF p((pos.p2()-pos.p1()));
    if (direction)
    {
        painter->drawLine(QLineF(pos.p1()+p/3, pos.p2()-p/3));
        painter->drawLine(QLineF(pos.p2()-p/3, pos.p2()-p/3+perp_norm(pos)*lanes_width*DEFAULT_SCALE_FACTOR/3 - p/10));
        painter->drawLine(QLineF(pos.p2()-p/3, pos.p2()-p/3-perp_norm(pos)*lanes_width*DEFAULT_SCALE_FACTOR/3 - p/10));
    }
    else
    {
        painter->drawLine(QLineF(pos.p1()+p/3.0, pos.p2()-p/3.0));
        painter->drawLine(QLineF(pos.p1()+p/3.0, pos.p2()-p*1.5/3.0+perp_norm(pos)*lanes_width*DEFAULT_SCALE_FACTOR/3.0 - p/10.0));
        painter->drawLine(QLineF(pos.p1()+p/3.0, pos.p2()-p*1.5/3.0-perp_norm(pos)*lanes_width*DEFAULT_SCALE_FACTOR/3.0 - p/10.0));
    }
}

void segment::removeBackwardLanes(int n)
{
    for (int i=0;i<n;i++)
    {
        lane *tmp=_backwardLanes.last();
        _backwardLanes.removeLast();
        delete tmp;
    }
}

void segment::removeForwardLanes(int n)
{
    for (int i=0;i<n;i++)
    {
        lane *tmp=_forwardLanes.first();
        _forwardLanes.removeFirst();
        delete tmp;
    }

}

void segment::calculateLanesPos()
{
    _lanePos.clear();
    for (int i=0;i<lanes().size();i++)
        _lanePos.push_back(calculateLanePos(lanes()[i]));
}

QVector<QPointF> segment::lanePos(lane *l)
{    
    int idx=lanes().indexOf(l);
    if (idx==-1)
        return QVector<QPointF> ();
    return _lanePos.at(idx);
}

inline QVector<QPointF> segment::calculateLanePos(lane *l)
{
    qDebug() << "QVector<QPointF> segment::calculateLanePos_aux(lane *l)" << id();
    QVector<QPointF> p;

    for (int i=0;i<_holeSegment.size()-1;i++)
    {
        QPointF perp,_perp_norm;

        _perp_norm=perp_norm(QLineF(_holeSegment[i],_holeSegment[i+1]));
        _perp_norm*=lanes_width;

        perp=_perp_norm*((lanes().size()/2.0)-0.5);


        foreach (lane *l1,lanes())
        {
            if (l!=l1)
                perp-=_perp_norm;
            else
                break;
        }

        p.push_back(_holeSegment[i]+perp);
        p.push_back(_holeSegment[i+1]+perp);
    }

    return p;
}


lane *segment::laneOnPos(QPointF p)
{
    for (int i=0;i<lanes().size();i++)
    {
        if (lanePolygon(lanes()[i]).containsPoint(p,Qt::WindingFill))
            return lanes()[i];
    }

    return NULL;
}


QPolygonF segment::lanePolygon(lane *la)
{
    QVector<QPointF> p=lanePos(la);

    if (p.size()==0)
        return QPolygonF();

    QPainterPath path(p[0]);
    for (int i=1;i<p.size();i++)
        path.lineTo(p[i]);

    //path=path.simplified();

    QPainterPathStroker st;
    st.setWidth(lanesWidth());

    return (st.createStroke(path)).toFillPolygon();
}

point *segment::commonPoint(segment *s)
{
    for (int i=0;i<2;i++)
        for (int j=0;j<2;j++)
            if (s->points()[i]==points()[j])
                return points()[j];
    return NULL;
}

void segment::updateManouvres()
{

    for (int i=0;i<lanes().size();i++)
        lanes()[i]->updateManouvres();
}

void segment::setHighlighted(bool a, QColor c)
{
    highlight=a;
    highlightColor=c;
    update();
}

void segment::updatePosition(){
    qDebug() << "AKIII segment id=" << id();
    preprocessLook();
    update();
}


void segment::layerChanged(quint32 state)
{
    setDrawingState(state);
    update();
}
