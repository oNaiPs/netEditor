#include <QtGui>

#include "graphicsScene.h"
#include "config.h"

graphicsScene::graphicsScene(QObject *parent) :
        QGraphicsScene(parent)
{

    segment_id=0;
    lane_id=0;
    point_id=0;
    count = 0;

    microEditionMode=false;

    QSettings settings;
    segmentDrawingState=settings.value("layers/SEGMENTS_DRAWING_STATE", DEFAULT_SEGMENT_DRAWING_STATE).toInt();
    pointDrawingState=settings.value("layers/POINTS_DRAWING_STATE",DEFAULT_POINT_DRAWING_STATE).toInt();

    microEditionMode=false;
}

void graphicsScene::setLayersWidget(layersWidget *l)
{
    _layers = l;
}

layersWidget *graphicsScene::layers()
{
    return _layers;
}

graphicsScene::~graphicsScene()
{
    foreach (segment *s,_segments)
        deleteSegment(s);
    foreach (point *p,_points)
        deletePoint(p);
}

point *graphicsScene::grepPoint(QPointF p)
{
    foreach (QGraphicsItem *i,items(p))
        if (i->type()==point::Type)
            return qgraphicsitem_cast<point*>(i);

    return NULL;
}

segment *graphicsScene::grepSegment(QPointF p)
{
    foreach (QGraphicsItem *i,items(p))
        if (i->type()==segment::Type)
            return qgraphicsitem_cast<segment*>(i);

    return NULL;
}



induction_loop *graphicsScene::grepInductionLoop(QPointF p)
{
    foreach (QGraphicsItem *i,items(p))
        if (i->type()==induction_loop::Type)
            return qgraphicsitem_cast<induction_loop*>(i);

    return NULL;
}

bool graphicsScene::deletePoint(point* d1)
{

    if (!d1->segments().empty())
        return false;
    else
    {
        qDebug() << "Removing Point ID " << d1->id();
        removeItem(d1);
        _points.remove(d1->id());
        delete d1;
        return true;

    }
}

bool graphicsScene::deleteSegment(segment *s)
{
    if (s==NULL || (s->finalPoint()==NULL) || (s->startPoint()==NULL))
        return false;

    foreach (lane *l,s->forwardLanes())
        foreach (manouvre *m,l->manouvres())
            delManouvre(m);

    foreach (lane *l,s->backwardLanes())
        foreach (manouvre *m,l->manouvres())
            delManouvre(m);

    s->finalPoint()->removeSegment(s);
    s->startPoint()->removeSegment(s);

    s->setForwardLanesNumber(0);
    s->setBackwardLanesNumber(0);

    removeItem(s);
    _segments.remove(s->id());
    delete s;
    return true;

}


void graphicsScene::startMicroEditionMode()
{
    qDebug() << "graphicsScene::startMicroEditionMode()";


    QList<segment *> allsegments=segments();

    //TODO
    for (int i=0;i<allsegments.size();i++)
    {
        for (int j=0;j<allsegments[i]->forwardLanes(NULL).size();j++)
        {
            for (int k=0;k<allsegments[i]->forwardLanes(NULL)[j]->originManouvres.size();k++)
                allsegments[i]->forwardLanes(NULL)[j]->originManouvres[k]->setVisible(1);
        }
        for (int j=0;j<allsegments[i]->backwardLanes(NULL).size();j++)
        {
            for (int k=0;k<allsegments[i]->backwardLanes(NULL)[j]->originManouvres.size();k++)
                allsegments[i]->backwardLanes(NULL)[j]->originManouvres[k]->setVisible(1);
        }
    }

    microEditionMode=true;

    update();
}

void graphicsScene::stopMicroEditionMode()
{
    qDebug() << "graphicsScene::stopMicroEditionMode()";


    QList<segment *> allsegments=segments();

    //TODO
    for (int i=0;i<allsegments.size();i++)
    {
        for (int j=0;j<allsegments[i]->forwardLanes(NULL).size();j++)
        {
            for (int k=0;k<allsegments[i]->forwardLanes(NULL)[j]->originManouvres.size();k++)
                allsegments[i]->forwardLanes(NULL)[j]->originManouvres[k]->setVisible(0);
        }
        for (int j=0;j<allsegments[i]->backwardLanes(NULL).size();j++)
        {
            for (int k=0;k<allsegments[i]->backwardLanes(NULL)[j]->originManouvres.size();k++)
                allsegments[i]->backwardLanes(NULL)[j]->originManouvres[k]->setVisible(0);
        }
    }



    microEditionMode=false;

    update();
}


void graphicsScene::layersChanged(QString a,quint32 state)
{
    qDebug() << "void graphicsScene::layersChanged(QTreeWidgetItem *item, int)";

    
    
    if (a=="Segments")
    {
      // QList<segment *> allsegments=segments();

        segmentDrawingState=state;
//        for (int i=0;i<allsegments.size();i++)
  //          allsegments[i]->update();
        emit segmentLayerChanged(state);
    }
    else if (a=="Points")
    {
        //  QList<point *> allpoints=points();

        pointDrawingState=state;
        //for (int i=0;i<allpoints.size();i++)
          //  allpoints[i]->update();

        emit pointLayerChanged(state);
    }
    //update();
}


point* graphicsScene::createPoint(QPointF p)
{
    qDebug() << "point* graphicsScene::createPoint(QPointF p)";
    point *tmp=new point(pointDrawingState);

    tmp->setPos(p);
    tmp->setID(QString::number(point_id++)+QString("p"));

    _points.insert(tmp->id(),tmp);

    addItem(tmp);

    connect(this,SIGNAL(pointLayerChanged(quint32)),tmp,SLOT(layerChanged(quint32)));

    return tmp;
}

void graphicsScene::addRoad(road *r)
{
    qDebug() << "Add road id:"<<r->id() << "\tname:" << r->name();
    _roads.insert(r->id(),r);

}


void graphicsScene::addPoint(point *p)
{
    qDebug() << "bool graphicsScene::addPoint(point *p)";
    qDebug() << p->info();

    connect(this,SIGNAL(pointLayerChanged(quint32)),p,SLOT(layerChanged(quint32)));

    _points.insert(p->id(),p);
    p->setDrawingState(pointDrawingState);
    addItem(p);
}


void graphicsScene::addSegment(segment *s)
{
    qDebug() << "aki";
    s->setDrawingState(segmentDrawingState);
    s->preprocessLook();
    _segments.insert(s->id(),s);
    addItem(s);

    connect(this,SIGNAL(segmentLayerChanged(quint32)),s,SLOT(layerChanged(quint32)));


}

segment* graphicsScene::createSegment(point *start_p,point *final_p,QString id)
{
    qDebug() << "segment* graphicsScene::createSegment(point *start_p,point *final_p,int _id)";
    //let's see if does't exist one with the same points

    foreach (segment *s,start_p->segments())
        if ((s->startPoint()==start_p && s->finalPoint()==final_p) || (s->finalPoint()==start_p && s->startPoint()==final_p))
            return NULL;


    segment *tmp=new segment(start_p,final_p,segmentDrawingState);

    if (id.isEmpty())
        tmp->setID(QString::number(++segment_id)+QString("s"));
    else
        tmp->setID(id);
    tmp->setLanesWidth(DEFAULT_LANE_WIDTH);

    tmp->setForwardLanesNumber(1);
    tmp->preprocessLook();

    _segments.insert(tmp->id(),tmp);

    addItem(tmp);


    connect(this,SIGNAL(segmentLayerChanged(quint32)),tmp,SLOT(layerChanged(quint32)));

    return tmp;
}

bool graphicsScene::addManouvre(lane *l1,lane*l2)
{
    qDebug() << "void graphicsScene::addManouvre(lane *l1,lane*l2)";


    if (hasManouvre(l1,l2))
    {
        qDebug() << "Already exists";
        return false;
    }


    point *point1=l1->parentSegment()->commonPoint(l2->parentSegment());

    //it has a node in common,lets see if the lanes have the same direction
    if (! ((l1->isForwardLane(point1) && (! l2->isForwardLane(point1))) ||
             ((! l1->isForwardLane(point1)) && l2->isForwardLane(point1))))
        return false;

    manouvre *tmp=new manouvre(l1,l2);

    //normally wont occur
    if (microEditionMode==0)
        tmp->setVisible(0);


    addItem(tmp);
    qDebug() << "Added manouvre";

    return true;
}

bool graphicsScene::delManouvre(manouvre *m)
{
    qDebug() << "void graphicsScene::delManouvre(manouvre *m)";

    removeItem(m);

    delete m;
    return true;
}

bool graphicsScene::delManouvre(lane *l1,lane *l2)
{
    qDebug() << "void graphicsScene::delManouvre(lane *l1,lane*l2)";

    //invasive way
    manouvre *tmp=NULL;

    for (int i=0;i<l1->originManouvres.size();i++)
    {
        if (l1->originManouvres[i]->destiny()==l2)
            tmp=l1->originManouvres[i];
    }
    for (int i=0;i<l1->destinManouvres.size();i++)
    {
        if (l1->destinManouvres[i]->origin()==l2)
            tmp=l1->destinManouvres[i];
    }

    if (tmp==NULL)
        return false;

    removeItem(tmp);

    delete tmp;
    return true;
}

bool graphicsScene::hasManouvre(lane *l1,lane *l2) // THIS IS VERY TIME CONSUMING
{

    manouvre *tmp=NULL;

    for (int i=0;i<l1->originManouvres.size();i++)
    {
        if (l1->originManouvres[i]->destiny()==l2)
            tmp=l1->originManouvres[i];
    }
    for (int i=0;i<l1->destinManouvres.size();i++)
    {
        if (l1->destinManouvres[i]->origin()==l2)
            tmp=l1->destinManouvres[i];
    }

    if (tmp)
        return true;
    else
        return false;
}




QList<segment*> graphicsScene::segments(){
    return _segments.values();
}

QList<road *> graphicsScene::roads()
{
    return _roads.values();
}

QList<lane *> graphicsScene::lanes()
{
    return _lanes.values();
}

QList<point *> graphicsScene::points()
{
    return _points.values();
}

segment *graphicsScene::segmentFromID(QString s)
{
    return _segments.value(s,NULL);
}

road *graphicsScene::roadFromID(QString s)
{
    return _roads.value(s,NULL);
}

lane *graphicsScene::laneFromID(QString s)
{
    return _lanes.value(s,NULL);
}

point *graphicsScene::pointFromID(QString s)
{
    return _points.value(s,NULL);
}
