#ifndef _GRAPHICSSCENE_
#define _GRAPHICSSCENE_

#include <QtGui>
#include <QGraphicsScene>

#include "layersWidget.h"
#include "road.h"
#include "segment.h"
#include "point.h"
#include "manouvre.h"
#include "induction_loop.h"

class road;
class lane;
class manouvre;
class induction_loop;

class graphicsScene: public QGraphicsScene
{
    Q_OBJECT

    int count;
    int segment_id;
    int lane_id;
    int point_id;//tmp
    //	QList<arrowItem *> arrowList;

    layersWidget *_layers;
    int lastPointID;

    bool microEditionMode;
    bool _connectedToDB;
    quint32 segmentDrawingState;
    quint32 pointDrawingState;

    QHash<QString,segment *> _segments;
    QHash<QString,road *> _roads;
    QHash<QString,lane*> _lanes;
    QHash<QString,point *> _points;
public:

    graphicsScene(QObject *parent = 0);
    ~graphicsScene(void);

    QList<segment*> segments();
    QList<road *> roads();
    QList<lane *> lanes();
    QList<point *> points();

    segment *segmentFromID(QString);
    road *roadFromID(QString);
    lane *laneFromID(QString);
    point *pointFromID(QString);

    //road *createAndAddRoad(QPointF);
    bool addManouvre(lane *,lane*);
    bool addManouvre(manouvre *);
    bool delManouvre(lane *,lane*);
    bool delManouvre(manouvre*);
    bool hasManouvre(lane*,lane*);

    point* createPoint(QPointF);
    void addRoad(road *r);
    void addPoint(point *p);
    segment *createSegment(point *start_p,point *final_p,QString _id="");
    void addSegment(segment *s);

    //	void removeArrowItem(arrowItem *);
    bool deletePoint(point*);
    bool deleteSegment(segment *);
    void removeRoad(road *);
    //void addRoad(road *);
    void setLayersWidget(layersWidget *l);

    layersWidget* layers();

    segment *grepSegment(QPointF);
    point *grepPoint(QPointF);
    induction_loop *grepInductionLoop(QPointF p);

    void startMicroEditionMode();
    void stopMicroEditionMode();

public slots:
    void layersChanged(QString,quint32);

signals:
    void segmentLayerChanged(quint32);
    void pointLayerChanged(quint32);
};

#endif /*_GRAPHICSSCENE_*/
