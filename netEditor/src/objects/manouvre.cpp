#include "manouvre.h"
#include "common.h"

manouvre::manouvre(lane *lane1,lane *lane2)
{
    //dispose lanes
    point *p=lane1->parentSegment()->commonPoint(lane2->parentSegment());
    
    if (p==NULL)qDebug() << "FATAL on manouvre::manouvre(lane *l1,lane *l2)";
    
    if (lane1->parentSegment()->forwardLanes(p).contains(lane1))
    {
        _origin=lane1;
        _destiny=lane2;
    }
    else
    {
        _origin=lane2;
        _destiny=lane1;
    }

    delta1=delta2=QPointF(0,0);

    qsrand(QDateTime::currentDateTime().toTime_t());

    pen.setColor(QColor(qrand() % 230, qrand() % 230, qrand() % 230));
    pen.setWidth(1.5);

    lane1->addManouvre(this);
    lane2->addManouvre(this);
    lane1->updateManouvres();
    lane2->updateManouvres();

    preprocessLook();
}

manouvre::~manouvre()
{
    prepareGeometryChange();
    origin()->delManouvre(this);
    destiny()->delManouvre(this);
    origin()->updateManouvres();
    destiny()->updateManouvres();
}

void manouvre::paint(QPainter *painter, const QStyleOptionGraphicsItem *,QWidget *)
{
    painter->setPen(pen);
    painter->drawLine(l1);
    painter->drawLine(l2);
    painter->drawEllipse(p1,p1r,p1r);
    painter->drawEllipse(p2,p2r,p2r);
    painter->setBrush(Qt::yellow);
    painter->drawEllipse(p1,p1r,p1r);
    painter->drawEllipse(p2,p2r,p2r);
}

QRectF manouvre::boundingRect() const
{

    return _boundingRect;

}

void manouvre::setDelta(QPointF p,lane *l)
{
    if (l==_origin)
    {
        delta1=p;
        qDebug() << "set delta1=" << p;
    }
    else
    {
        delta2=p;
        qDebug() << "set delta2=" << p;
    }

    //assuming preprocessLook call after this
}



QString manouvre::key()
{
    //to be used
    return QString::number(_origin->id())+QString::number(_destiny->id());
}



QPointF norm(QLineF l)
{
    l.translate(-l.p1());
    
    return l.p2()/l.length();
}


void manouvre::preprocessLook()
{
    prepareGeometryChange();
    //find common node
    point *p=_origin->parentSegment()->commonPoint(_destiny->parentSegment());
    QVector<QPointF> vec1;

    vec1=_origin->parentSegment()->lanePos(_origin);
    if (vec1.size()<2)
        return;


    if (_origin->isForwardLane())
    {
        l1=QLineF(vec1[vec1.size()-2],vec1[vec1.size()-1]);

        l1.setP1(l1.pointAt(0.9)-norm(l1)*p->diameter()/2.0+delta1);
        p1=l1.p1();
        l1.setP2(l1.p2()+delta1);
    }
    else
    {
        l1=QLineF(vec1[0],vec1[1]);

        l1.setP1(l1.pointAt(0.1)+norm(l1)*p->diameter()/2.0+delta1);
        p1=l1.p1();
        l1.setP2(l1.p2()+delta1);
    }

    vec1=_destiny->parentSegment()->lanePos(_destiny);
    if (vec1.size()<2)
        return;


    if (! _destiny->isForwardLane())
    {
        l2=QLineF(vec1[vec1.size()-2],vec1[vec1.size()-1]);

        l2.setP2(l2.pointAt(0.9)-norm(l2)*p->diameter()/2.0+delta2);
        p2=l2.p2();
        l2.setP1(l2.p1()+delta2);
    }
    else
    {
        l2=QLineF(vec1[0],vec1[1]);

        l2.setP2(l2.pointAt(0.1)+norm(l2)*p->diameter()/2.0+delta2);
        p2=l2.p2();
        l2.setP1(l2.p1()+delta2);
    }

    p1r=((float)_origin->parentSegment()->lanesWidth()/3.5F);

    if (_origin->originManouvres.size())
        p1r/=_origin->originManouvres.size();

    p2r=((float)_destiny->parentSegment()->lanesWidth()/3.5F);
    if (_destiny->destinManouvres.size())
        p2r/=(float)_destiny->destinManouvres.size();

    //lets extend the lines so the two will merge together
    QPointF i;
    l1.intersect(l2,&i);

    l2.setP1(i);
    l1.setP2(i);


    if (QLineF(p1,p2).length()*2<(l1.length()+l2.length()))
    {
        i=l1.p1();
        l2.setP1(i);
        l1.setP2(i);
    }

    preprocessPath();
}

void manouvre::preprocessPath()
{
    QPainterPath p(l1.p1());
    p.lineTo(l1.p2());
    p.lineTo(l2.p2());

    QPainterPathStroker st;
    st.setWidth(p1r>p2r?p1r:p2r);

    _shape=st.createStroke(p);
    _boundingRect=_shape.controlPointRect().adjusted(-2.5,-2.5,2.5,2.5);

}

lane *manouvre::otherLane(lane *l)
{
    if (l==_origin)
        return _destiny;
    else if (l==_destiny)
        return _origin;
    else return NULL;
}
