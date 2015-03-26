#include "induction_loop.h"
#include "common.h"
#include <math.h>


long int induction_loop::globalID=0;

induction_loop::induction_loop(lane *l,QPointF pos)
{
    setID(QString::number(++globalID));
    parentLane=l;
    setPos(pos);
    preprocessLook();
    parentLane->addInductionLoop(this);
    _frequency=1;
}

induction_loop::~induction_loop()
{
    parentLane->delInductionLoop(this);

}


void induction_loop::preprocessLook()
{
    QVector<QPointF > v=parentLane->parentSegment()->lanePos(parentLane),vec;

    for (int i=0;i<v.size()-1;i++)
    {
        vec.push_back(DistancePointLine(pos(),QLineF(v[i],v[i+1])));
    }


    qreal min=(pos()-vec[0]).manhattanLength();
    QPointF selectedPoint;

    for (int i=0;i<vec.size();i++)
    {

        qreal dif=(pos()-vec[i]).manhattanLength();

        if (dif<=min)
        {
            min=vec[i].manhattanLength();
            selectedPoint=vec[i];
        }
        qDebug() << min << vec[i].manhattanLength() << dif;
    }
    qDebug() << "aki" << selectedPoint;

    setPos(selectedPoint);

    QLineF reference(v.first(),v.last());
    float mag=(float)QLineF(reference.p1(),pos()).length()/reference.length();

    //    setPos(reference.pointAt(mag));
    _pointPosFromLane=mag;
}

void induction_loop::paint(QPainter *painter, const QStyleOptionGraphicsItem *,QWidget *)

{
    painter->setBrush(Qt::yellow);

    float a= parentLane->parentSegment()->lanesWidth()/2.0;

    painter->drawEllipse(-a,-a,a*2,a*2);
    painter->drawText(QRectF(-a,-a,a*2,a*2),"i");
}

QRectF induction_loop::boundingRect() const
{

    return shape().boundingRect();
}


QPainterPath induction_loop::shape () const
{
    QPainterPath p(QPointF(-5,-5));
    p.lineTo(-5,5);
    p.lineTo(5,5);
    p.lineTo(5,-5);
    p.closeSubpath();
    return p;
}

void induction_loop::setSelected(bool b)
{
    if (b)
        currentColor=Qt::red;
    else
        currentColor=Qt::yellow;

    update();
}

int induction_loop::type () const
{
    return QGraphicsItem::UserType+INDUCTION_LOOP_TYPE;
}


