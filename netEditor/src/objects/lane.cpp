/*
 * lane_ng.cpp
 *
 *  Created on: Nov 30, 2008
 *      Author: root
 */


#include "lane.h"
#include "graphicsScene.h"
#include "common.h"

long int lane::globalID=0;

lane::lane(segment *s)
{
    
    setID(++globalID);
    
    qDebug() << "new line id" << id();
    _segment=s;
    
}

lane::~lane()
{
    qDebug() << "lane::~lane()";
    while(manouvres().size())
        delManouvre(manouvres()[0]);
    //emit prepareForDelection(this);
}

void lane::setSpeedLimit(int a) {
    speed_limit = a;
}

void lane::setCarAllowed(int a) {
    if ((a < 1) || (a > 7))
    {
        qDebug() << "FATAL on lane::setCarAllowed(int a)";
    }
    car_allowed = a;
}


void lane::addManouvre(manouvre *m)
{
    
    if (m->origin()==this)
    {
        if (!originManouvres.contains(m))
            originManouvres.push_back(m);
    }
    else if (m->destiny()==this)
    {
        if (!destinManouvres.contains(m))
            destinManouvres.push_back(m);
    }

}

void lane::delManouvre(manouvre* m)
{
    if (originManouvres.contains(m))
        originManouvres.removeOne(m);
    if (destinManouvres.contains(m))
        destinManouvres.removeOne(m);
}



bool lane::isForwardLane(point *p)
{
    return parentSegment()->forwardLanes(p).contains(this);
}

QList<lane *> lane::getBackwardLaneConnections()
{
    qDebug() << "QList<lane *> lane::getBackwardLaneConnections()";

    QList<lane *>list;
    for (int i=0;i<originManouvres.size();i++)
    {
        qDebug() << "from" << id() << "to" << originManouvres[i]->destiny()->id();
        list.push_back(originManouvres[i]->destiny());
    }
    
    return list;
}

void lane::addInductionLoop(induction_loop *i)
{
    if (!_induction_loops.contains(i))
        _induction_loops.push_back(i);
}

void lane::delInductionLoop(induction_loop *i)
{
    _induction_loops.removeOne(i);
}


QList<lane *> lane::getForwardLaneConnections()
{
    qDebug() << "QList<lane *> lane::getForwardLaneConnections()";
    QList<lane *>list;
    for (int i=0;i<destinManouvres.size();i++)
    {
        qDebug() << "from" << destinManouvres[i]->origin()->id() << "to" << id();
        list.push_back(destinManouvres[i]->origin());
    }
    
    return list;
}



QList<manouvre*> lane::manouvres()
{
    return originManouvres+destinManouvres;
}


void insertionSort(QList <manouvre *> &m, QList<qreal> &angles)
{
    qDebug() << "void insertionSort(QList <manouvre *> &m, QList<qreal> &angles)";
    int i;
    int j;
    qreal aux;
    manouvre *aux1;
    int k = angles.size() - 1 ;
    
    for(i = 0; i < angles.size(); i++)
    {
        for(j = 0; j < k; j++)
        {
            if(angles[j] > angles[j+1])
            {
                aux = angles[j];
                angles[j] = angles[j+1];
                angles[j+1]=aux;
                aux1 = m[j];
                m[j] = m[j+1];
                m[j+1]=aux1;
            }
        }
        k--;
    }
}




void lane::sortManouvres(QList<manouvre *> &m)
{
    QList<qreal> angles;
    foreach (manouvre *m1,m)
    {
        if (m1->origin()==this)
            angles.push_back(m1->destiny()->parentSegment()->referenceLine().angle(m1->origin()->parentSegment()->referenceLine()));
        else
            angles.push_back(m1->origin()->parentSegment()->referenceLine().angle(m1->destiny()->parentSegment()->referenceLine()));
    }
    insertionSort(m,angles);
    
    for (int i=0;i<angles.size();i++)
        qDebug() << "Angles sorted:" << angles[i];
    
}


void lane::updateManouvres()
{//TODO optimize
    if (manouvres().size()==0)
        return;
    
    
    sortManouvres(originManouvres);
    
    //we'll have to make a list of oriented manouvres
    
    
    //for origin
    QPointF norm(perp_norm(this->parentSegment()->referenceLine()));
    QPointF pos(norm*parentSegment()->lanesWidth()/2.0);
    norm*=parentSegment()->lanesWidth();
    norm/=originManouvres.size()+1;
    
    for (int i=0;i<originManouvres.size();i++)
    {
        pos-=norm;
        originManouvres[i]->setDelta(pos,this);
        originManouvres[i]->preprocessLook();
    }
    
    //for destin
    norm =QPointF(perp_norm(this->parentSegment()->referenceLine()));
    pos=QPointF(norm*parentSegment()->lanesWidth()/2.0);
    norm*=parentSegment()->lanesWidth();
    norm/=destinManouvres.size()+1;
    
    for (int i=0;i<destinManouvres.size();i++)
    {
        pos-=norm;
        destinManouvres[i]->setDelta(pos,this);
        destinManouvres[i]->preprocessLook();
    }
}


