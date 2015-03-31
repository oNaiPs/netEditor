#ifndef GRAPHICSVIEW_H_
#define GRAPHICSVIEW_H_

#include <QtGui>
#include <QtWebKit>
#include <QGraphicsView>
#include <QDockWidget>
#include <QWebView>

#include "segmentPropertiesWidget.h"
#include "pointPropertiesWidget.h"
#include "inductionlooppropertieswidget.h"
#include "graphicsScene.h"
#include "config.h"
#include "modifiedWidgets/filterpushbutton.h"
#include "road.h"

enum OVERLAYMODE {NO_OVERLAY,GMAPS};

class graphicsScene;
class pointPropertiesWidget;
class segmentPropertiesWidget;
class inductionloopPropertiesWidget;
class filterPushButton;

class centralGraphicsView: public QGraphicsView
{
    Q_OBJECT

    Qt::MouseButton lastButtonPressed; //temporary

    double scaleFactor;
    QPoint lastPos, lastPosOnPress;
    QPointF mousepos, gridPos;
    int lastKeyPressed; //Qt::Key
    Qt::KeyboardModifiers lastKeyModifiers;
    int count;


    QList<point *> selectedPoints;
    QList<point *> previousSelectedPoints;

    QList<segment *> selectedSegments;
    QList<segment *> previousSelectedSegments;

    EDITIONMODE currentEditionMode;
    OVERLAYMODE currentOverlayMode;

    void scaleView(qreal, QPoint mousePos);

    QDockWidget *dock;



    graphicsScene *gScene(){ return dynamic_cast<graphicsScene *>(scene());}

    //Copy & Paste
    // QList<road *> copyContent;
    //	void copyItems(QList<road *>);
    //	void cutItems(QList<road *>);
    //	void pasteItems();

    //################NORMAL MODE###########
    bool segmentProcessRunning;

    //	void itemSelection(graphicsItemObject *, bool); //bool -> 1 for multiple selecting
    void showItemFilterWidget();

    void deletePoints(QList<point*>);
    void deleteSegments(QList<segment*>);
    void deleteTrafficItems(QList<QGraphicsView*> items);

    void unSelectSegments();
    void unSelectPoints();
    void unSelectItems();

    void setMouseCursorOnPress(QMouseEvent *);
    void setMouseCursorOnRelease(QMouseEvent *);

    //	void moveItems(QList<graphicsItemObject*>,QPointF);

    segmentPropertiesWidget *segmentProperties;
    pointPropertiesWidget *pointProperties;
    inductionloopPropertiesWidget *inductionloopProperties;
    //################MICROEDITION MODE###########

    bool manouvreProcessRunning;
    QGraphicsPolygonItem *manouvrePolygon;
    //manouvreItem *manouvreProcess;
    lane *selectedLane,*previousSelectedLane;

    //#############TRIP ASSIGNEMENT // must move to a custom class
    //	QList< road *> tripAssignmentpdRoads;
    void emptyDock();

    //###########google maps
    QGraphicsProxyWidget *gmaps_viewproxy;
    QWebView *gmaps_view;
    void createGMapsOverlay();
    //############selections mode
    void createAndShowContextMenu(QPoint);
    QGraphicsLineItem *drawingLine;
    point *tmpPoint;
    //############## moving items

    bool moveItemsProcessRunning;

    inline void processAffectedSegments(QList<point*> &);

    //---- Test Block -----

    //---- Press Ctrl function
    void discretiseActions(void);

public:
    centralGraphicsView(QGraphicsScene *,QWidget * parent = 0);
    ~centralGraphicsView();
    QRectF visibleRegion();

    void translate(qreal dx,qreal dy);

    qreal getScaleFactor(){return scaleFactor;}

    void setDock(QDockWidget *d){dock=d;}
    void generateAxis();
    inline void startSegmentProcess(QPointF);
    inline bool endSegmentProcess(QPointF);

    void cancelSegmentProcess();
    void normalModeMoveEvent(QMouseEvent *);

    void setCurrentEditionMode(EDITIONMODE);
protected:
    void wheelEvent ( QWheelEvent *);
    void keyPressEvent ( QKeyEvent * );
    void keyReleaseEvent( QKeyEvent *);
    void mousePressEvent(QMouseEvent *);
    void mouseMoveEvent ( QMouseEvent *);
    void mouseReleaseEvent (QMouseEvent *);
    void resizeEvent ( QResizeEvent *);
    void mouseDoubleClickEvent ( QMouseEvent * ){}

public slots:
    void startPanMode();
    void startSelectMode();
    void startRectSelectMode();
    void startLineMode();
    void startMicroEditionMode();
    void startPolylineMode();
    void startInductionLoopMode();

    void selectOverlayMode(OVERLAYMODE);
    void setGMapsMode(bool);

    void receivedCommand(QString);
    void filterSelection(QString);

    void selectAll();

signals:
    void viewportChanged();
    void focusReceived();

    void showOnLabel1(QString);
    void showOnLabel2(QString);
    void showOnLabel3(QString);
};

#endif /*GRAPHICSVIEW_H_*/
