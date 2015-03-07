#include <QtGui>
#include "common.h"

template <class T>  void swapPointers(T* a,T* b)
{
    T* c=a;
    a=b;
    b=c;
    delete c;
}


QPointF perp_norm(QLineF ref)
{
    if (ref.length()==0)
        return QPointF(0,0);

    QLineF normal=ref.normalVector();
    normal.translate(-normal.p1());

    return normal.unitVector().p2();
}

QPointF DistancePointLine(QPointF p, QLineF l)
{
    float LineMag;
    float U;

    LineMag = l.length();

    U = ( ( ( p.x() - l.p1().x() ) * ( l.p2().x() - l.p1().x() ) ) +
          ( ( p.y() - l.p1().y() ) * ( l.p2().y() - l.p1().y() ) )) /
        ( LineMag * LineMag );

    if( U < 0.0f || U > 1.0f )
        return QPointF(0,0);   // closest point does not fall within the line segment

    float xx = l.p1().x() + U * ( l.p2().x() - l.p1().x() );
    float yy = l.p1().y() + U * ( l.p2().y() - l.p1().y() );

    return  QPointF(xx,yy);
}
