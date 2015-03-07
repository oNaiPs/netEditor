#ifndef INDUCTION_LOOP_H
#define INDUCTION_LOOP_H

#include <QtGui>
#include "lane.h"
#include "object.h"
#include "config.h"

class lane;

/// This is an experimental object to insert on the lanes (WARNING)
class induction_loop:public object
{
    lane *parentLane;
    float _pointPosFromLane;//parameterized point from lane

int _frequency;
    QColor currentColor;

    static long int globalID;
public:
    induction_loop(lane *,QPointF pos);
    ~induction_loop();
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *,QWidget *);
    QRectF boundingRect() const;
    QPainterPath shape () const;
    enum { Type = QGraphicsItem::UserType+INDUCTION_LOOP_TYPE};
    void setSelected(bool s);
    void preprocessLook();
    int type () const;

    int frequency(){return _frequency;}
    void setFrequency(int f){_frequency=f;}

    float pointPosFromLane(){return _pointPosFromLane;}
    lane *getParentLane(){return parentLane;}
};


#endif // INDUCTION_LOOP_H
