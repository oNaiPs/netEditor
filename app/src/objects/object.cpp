#include "object.h"

object::object()
{
}


void object::setID(QString i)
{
    Q_ASSERT_X(i!="","Point set id", "An ID must have somethin!");
    _id=i;
}


QString object::id()
{
    return _id;
}

quint32 object::drawingState()
{
    return _drawingState;
}

void object::setDrawingState(quint32 d)
{
    _drawingState=d;
}

