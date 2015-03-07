#include "road.h"

road::road(QString id) :
        _id(id)
{
}

QString road::info()
{
    QString s("Road ID:");
    s.append((id()));
    s.append(" Name:");
    s.append(_name);
    return s;
}

QString road::name()
{
    return _name;
}

point *road::finalPoint()
{
    if (_segments.size() == 0)
        return NULL;
    return _segments.last()->finalPoint();
}

point *road::startPoint()
{
    if (_segments.size() == 0)
        return NULL;
    return _segments.first()->startPoint();
}



void road::updateShape()
{
    QPainterPath tmp(_segments[0]->path());
    for (int i=1;i<_segments.size();i++)
        tmp.addPath(_segments[i]->path());
    _shape=tmp;
}



bool road::addSegment(segment *i)
{
    if (i == NULL || i->startPoint()==NULL || i->finalPoint()==NULL)
        return 0;

    if (_segments.size() == 0)
    {
        _segments.push_back(i);
    }
    else
    {
        if (i->startPoint() == _segments.last()->finalPoint())
            _segments.push_back(i);
        else if (i->finalPoint() == _segments.first()->startPoint())
            _segments.prepend(i);
        else
        {
            qDebug() << "return 0 on bool road::addSegment(segment *i)";
            return 0;
        }
    }


    if (i->parentRoad()!=this)
        i->setParentRoad(this);

    qDebug() << "Successfully added segment id" << i->id() << "to road id" << this->id() << "segments number" << this->segments().size();
    return 1;
}


float road::length()
{
    float soma=0;

    foreach (segment *s,_segments)
        soma+=s->lenght();

    return soma;

}

QList<segment *> & road::segments()
{
    return _segments;
}

void road::setID(QString i)
{
    _id=i;
}
QString road::id()
{

    return _id;
}
