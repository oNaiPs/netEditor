#include "point.h"
#include "config.h"
#include <stdlib.h>

quint32 point::globalId=0;


point::point(quint32 dstate)
{
    _point_type=priority;
    setZValue(-1);

    setDrawingState(dstate);
    setID(QString("def_p_")+QString::number(globalId++));
    //setCacheMode(QGraphicsItem::DeviceCoordinateCache);

    setCarsPercent(DEFAULT_VEHS_PERCENT);
    setFlow(DEFAULT_VEHS_PER_HOUR);

    preprocessLook();
    setFlags(QGraphicsItem::ItemIsSelectable|QGraphicsItem::ItemIsMovable|QGraphicsItem::ItemSendsGeometryChanges);

}

QString point::info()
{
    QString tmp("Point id:");
    tmp.push_back(id());
    tmp.push_back(" x:");
    tmp.push_back(QString::number(x()));
    tmp.push_back(" y:");
    tmp.push_back(QString::number(y()));
    return tmp;
}


void point::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    if (GET_BIT(drawingState(),OBJECT_TOGGLE))
    {

        if (GET_BIT(drawingState(),POINT_CIRCLE))
        {
            if (isSelected())
                painter->setBrush(Qt::darkGray);
            else
                painter->setBrush(brush);

            int elipseRadius=_boundingRect.right()*0.9;
            painter->drawEllipse(QPointF(0,0),elipseRadius,elipseRadius);
            painter->setBrush(Qt::white);
        }
        if (GET_BIT(drawingState(),POINT_ID))
        {
            painter->setFont(QFont("sans",2));
            painter->drawText(_boundingRect,id(),QTextOption(Qt::AlignCenter));
        }

    }
}

QVariant point::itemChange(GraphicsItemChange change, const QVariant &value)
{

    if (change==QGraphicsItem::ItemPositionChange)
    {
        qDebug()<< "point::akii id=" << id() << " position" << pos();
        //QPointF d=value.toPointF()-pos();
        emit positionChanged();

    }
    return QGraphicsItem::itemChange(change,value);
}

QRectF point::boundingRect() const
{
    return _boundingRect;
}



QPainterPath point::shape () const
{
    return _shape;
}



int point::type () const
{
    return QGraphicsItem::UserType+POINT_TYPE;
}


void point::addSegment(segment *s)
{
    if (! _segments.contains(s))
        _segments.push_back(s);

    preprocessLook();
}

void point::removeSegment(segment *s)
{
    if (_segments.contains(s))
        _segments.removeOne(s);
    preprocessLook();
}

bool point::containsSegment(segment *s)
{
    return _segments.contains(s);
}

QList<segment *> & point::segments()
{
    return _segments;
}

void point::preprocessLook()
{
    qDebug() << "void point::preprocessLook() point_id=" << this->id();

    prepareGeometryChange();

    bool s1=0,s2=0;

    qreal d=diameter();

    QPainterPath t;
    t.addEllipse(-d/2.0,-d/2.0,d,d);
    _shape=t;
    _boundingRect=_shape.controlPointRect();


    if (_segments.size()==1)
    {
        if (_segments[0]->forwardLanes(this).size()>0)
            s1=true;
        if (_segments[0]->backwardLanes(this).size()>0)
            s2=true;

        if (s1 && s2)
            brush=QBrush(Qt::yellow);
        else if (s2)
            brush=QBrush(Qt::green);
        else if (s1)
            brush=QBrush(Qt::red);
        else if (_point_type==traffic_light)
            brush=QBrush(Qt::blue);
        else
            brush=QBrush(Qt::lightGray);

    }
    else
        brush=QBrush(Qt::lightGray);

    update();
}

qreal point::diameter()
{
    qreal diameter=0;

    for (int i=0;i<segments().size();i++)
        if (_segments[i]->lanes().size()*_segments[i]->lanesWidth()>diameter)
            diameter=_segments[i]->lanes().size()*_segments[i]->lanesWidth();

    return diameter;
}

bool point::isSourceOrDrain()
{
    return isSource() || isDrain();
}

bool point::isSource()
{
    bool s1=false,s2=false;
    if (_segments.size()==1)
    {
        if (_segments[0]->forwardLanes(this).size()>0)
            s1=true;
        if (_segments[0]->backwardLanes(this).size()>0)
            s2=true;
    }
    return (s2);
    //return (s2==true and s1==false);
}

bool point::isDrain()
{
    bool s1=false,s2=false;
    if (_segments.size()==1)
    {
        if (_segments[0]->forwardLanes(this).size()>0)
            s1=true;
        if (_segments[0]->backwardLanes(this).size()>0)
            s2=true;
    }
    //return (s2==false and s1==true);
    return (s1);
}

segment *point::getCommonSegment(point *p)
{
    for (int i=0;i<_segments.size();i++)
    {
        if ((_segments[i]->startPoint()==p) || (_segments[i]->finalPoint()==p))
            return _segments[i];
    }

    return NULL;
}

segment *point::getTheOtherSegment(segment *s)
{
    if (_segments.size() != 2)
    {
        qDebug() << "adjacentRoads.size() !=2 !!! returning NULL";
        return NULL;
    }

    if (_segments[0] == s)
        return _segments[1];
    else
        return _segments[0];
}


void point::getPossibleWaysAssistant(point *currentPoint, point *anteriorPoint,QList<point *> list)
{

    list.push_back(currentPoint);

    if (currentPoint->isDrain())
        possibleWays.push_back(list);

    foreach (segment *r,currentPoint->segments())
    {
        if (currentPoint->getCommonSegment(anteriorPoint) != r)
        {
            if (! list.contains(r->otherPoint(currentPoint)))
            {

                bool ok = 0;
                foreach (lane *l1,currentPoint->getCommonSegment(anteriorPoint)->backwardLanes(anteriorPoint))
                    foreach (lane *l2,r->backwardLanes(currentPoint))
                        foreach (manouvre *m,l1->manouvres())
                            if (l2 == m->destiny())
                            {
                                ok = 1;
                                break;
                            }

                if (ok)
                {
                    point *tmp = anteriorPoint;
                    point *tmp1 = currentPoint;

                    anteriorPoint = currentPoint;
                    currentPoint = r->otherPoint(currentPoint);

                    getPossibleWaysAssistant(currentPoint, anteriorPoint,
                                             list);

                    anteriorPoint = tmp;
                    currentPoint = tmp1;

                }
            }
        }
    }
}


void point::processPossibleWays()
{
    qDebug() << "void pointPropertiesWidget::findPossibleWays()";
    //using depth first search to find all ways

    QList<point *> list;
    oldPossibleWays = possibleWays;
    possibleWays.clear();

    list.push_back(this);

    qDebug() << "Computing possible ways";

    //TODO  make compatible to source node with more than 1 adjacent road
    getPossibleWaysAssistant(segments()[0]->otherPoint(this), this, list);

    qDebug() << "Done";

    qDebug() << "Ways Found: " << possibleWays.size();

    if (possibleWaysChanged())
    {
        possibleWaysDistribution.clear();
        for (int i = 0; i < possibleWays.size(); i++)
        {
            qDebug() << "tmpPossibleWaysChanged()=true!";

            possibleWaysDistribution.push_back(100.0 / possibleWays.size());
        }
    }

}


void point::layerChanged(quint32 state)
{
    setDrawingState(state);
    update();
}
