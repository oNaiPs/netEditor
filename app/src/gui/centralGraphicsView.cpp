#include <QGLWidget>
#include <QtWebKit>
#include <QGraphicsProxyWidget>
#include <QMessageBox>
#include <QWebFrame>
#include <QMenu>

#include "centralGraphicsView.h"
#include "graphicsScene.h"

#include "road.h"
#include "lane.h"
#include "segment.h"
#include "induction_loop.h"

centralGraphicsView::centralGraphicsView(QGraphicsScene *scene,QWidget * parent):QGraphicsView(scene, parent)

{
    //  setOptimizationFlags(QGraphicsView::DontClipPainter | QGraphicsView::DontSavePainterState | QGraphicsView::DontAdjustForAntialiasing);
    
    //  setViewportUpdateMode(QGraphicsView::SmartViewportUpdate);
    currentOverlayMode = NO_OVERLAY;
    
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    
    scaleFactor = DEFAULT_SCALE_FACTOR;
    
    segmentProperties = new segmentPropertiesWidget(gScene());
    pointProperties = new pointPropertiesWidget(gScene());
    inductionloopProperties = new inductionloopPropertiesWidget(gScene());
    
    setDragMode(QGraphicsView::NoDrag);
    setResizeAnchor(QGraphicsView::NoAnchor);
    setTransformationAnchor(QGraphicsView::NoAnchor);
    setRenderHints(QPainter::Antialiasing | QPainter::HighQualityAntialiasing | QPainter::TextAntialiasing |QPainter::SmoothPixmapTransform);

    
    segmentProcessRunning = 0;
    manouvreProcessRunning = 0;
    moveItemsProcessRunning = 0;
    
    lastButtonPressed = Qt::NoButton;
    lastKeyModifiers = Qt::NoModifier;
    lastKeyPressed = 0;
    
    generateAxis();
    createGMapsOverlay();
    
    QGraphicsView::centerOn(0,0);

    //Initialize settings
    QSettings settings;
    
    QColor c;
    c.setNamedColor(settings.value("graphicsView/backgroundColour","#9c9c9c").toString());
    setBackgroundBrush(c);

}

centralGraphicsView::~centralGraphicsView()
{
    //save settings
    QSettings settings;
    settings.setValue("graphicsView/backgroundColour",backgroundBrush().color().name());
}

void centralGraphicsView::translate(qreal dx,qreal dy) {
#warning gmaps_viewproxy->translate(-dx, -dy);

    QGraphicsView::translate(dx,dy);
    emit viewportChanged();//for preview widget updating

}

void centralGraphicsView::createGMapsOverlay()
{
    gmaps_view = new QWebView;
    gmaps_view->load(QUrl("qrc:/google-maps.html"));

    //gmaps_view->

    gmaps_viewproxy = scene()->addWidget(gmaps_view, Qt::Widget);
    gmaps_viewproxy->setZValue(-2);
    
    gmaps_viewproxy->setGeometry(QRectF(0, 0, width(), height()));
    if (currentOverlayMode != GMAPS)
        gmaps_view->setVisible(0);
}

void centralGraphicsView::setGMapsMode(bool b)
{
    if (b)
    {
        QMessageBox::warning(this,"Warning","This overlay is somewhat deprecated, as it doesn't have the real coordinate translation from google maps system to netEditor's.\nPlease use an import plugin such as OpenStreetMaps.");
        selectOverlayMode(GMAPS);
    }
    else
        selectOverlayMode(NO_OVERLAY);
}

void centralGraphicsView::selectOverlayMode(OVERLAYMODE mode)
{
    
    if (mode == NO_OVERLAY)
    {
        qDebug() << "Selected mode NO_OVERLAY";
        gmaps_view->setVisible(0);
    }
    if (currentOverlayMode == NO_OVERLAY && mode == GMAPS)
    {
        
        qDebug() << "Selected mode GMAPS";
        gmaps_viewproxy->setGeometry(QRectF(0, 0, width(), height()));
        gmaps_view->setVisible(1);
    }
    
    currentOverlayMode = mode;
}

void centralGraphicsView::generateAxis()
{
    QGraphicsLineItem *tmp = new QGraphicsLineItem(20, 0, -20, 0);
    QGraphicsLineItem *tmp1 = new QGraphicsLineItem(0, 20, 0, -20);
    scene()->addItem(tmp);
    scene()->addItem(tmp1);
}

void centralGraphicsView::scaleView(qreal sF, QPointF mouse)
{
    //TODO is this right?
    qreal factor = matrix().scale(sF, sF).mapRect(QRectF(0, 0, 1, 1)).width();
    if (factor < 0.07 || factor > 100)
        return;

    //Center the view in the mouse position
    //the factor 0.7 is to smooth the zoom shifting
    /* there is a flaw.
     * zooming should somewhat focus on the mouse pointer when going in
     * but stay with the center of the view when going out
     */
    if (sF >= 1.0)      centerOn(mouse);
    else {};

    //translate(mouse.x(), mouse.y());
    //setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
    scale(sF, sF);

    scaleFactor /= sF;
}

void centralGraphicsView::wheelEvent(QWheelEvent * event)
{
    if (currentOverlayMode == GMAPS)
        return;//TODO, zoom according to gmaps zooms
    
    //TODO USE <QGraphicsItemAnimation> for google maps animation like
    
    //ZOOM
    // TODO:
    scaleView(pow((double) 2, event->delta() / 480.0),  mapToScene(event->pos()));

    QString zoom=QString::number(1 / scaleFactor * 100, 0, 0) + QString('%');
    showOnLabel3(zoom);
    
    emit viewportChanged();
}

inline void centralGraphicsView::startSegmentProcess(QPointF sceneMapped)
{
    showOnLabel2("Segment started");
    
    segmentProcessRunning=1;
    drawingLine=new QGraphicsLineItem(QLineF(sceneMapped,sceneMapped));
    scene()->addItem(drawingLine);
    
    if (gScene()->grepPoint(sceneMapped)==NULL)
        tmpPoint=gScene()->createPoint(sceneMapped);
    else
        tmpPoint=gScene()->grepPoint(sceneMapped);
}

inline bool centralGraphicsView::endSegmentProcess(QPointF sceneMapped)
{
    if (!segmentProcessRunning)
        return 0; //Never going to happen :D
    
    scene()->removeItem(drawingLine);
    
    point *p1=gScene()->grepPoint(sceneMapped);

    if (p1==tmpPoint)
        cancelSegmentProcess();
    
    //cancel if segment length is inferior to defined
    if (QLineF(tmpPoint->pos(),sceneMapped).length() < MINIMUM_ROAD_LENGTH * scaleFactor)
    {
        QString inf("Your road must be larger than ");
        inf += QString::number(MINIMUM_ROAD_LENGTH);
        inf += " meters";
        showOnLabel2(inf);
        cancelSegmentProcess();
        return 0;
    }
    
    segment *tmp;
    
    if (p1==NULL)
        tmp=gScene()->createSegment(tmpPoint,gScene()->createPoint(sceneMapped));
    else
        tmp=gScene()->createSegment(tmpPoint,p1);
    
    if (tmp==NULL) showOnLabel2("<font color=red>Could not create Segment</font>");
    else
    {
        showOnLabel2("<font color=green>Created!</font>");

    }
    
    segmentProcessRunning=0;
    
    return 1;
}

void centralGraphicsView::setMouseCursorOnPress(QMouseEvent *event)
{
    qDebug() << "void centralGraphicsView::setMouseCursorOnPress(QMouseEvent *event)";
    if ((currentEditionMode == PAN) || (event->button() == Qt::MidButton))
        setCursor(Qt::ClosedHandCursor);
}

void centralGraphicsView::setMouseCursorOnRelease(QMouseEvent *)
{
    qDebug() << "void centralGraphicsView::setMouseCursorOnRelease(QMouseEvent *)";
    setCursor(QCursor(QPixmap(":/images/mousecursor.png")));

    if (currentEditionMode == PAN)
        setCursor(Qt::OpenHandCursor);
    else if (currentEditionMode==MICROEDITION)
        setCursor(Qt::ArrowCursor);
}


void centralGraphicsView::mousePressEvent(QMouseEvent *event)
{
    QGraphicsView::mousePressEvent(event);

    qDebug() << "void centralGraphicsView::mousePressEvent(QMouseEvent *event)";
    
    point *selectedPoint;
    segment *selectedSegment;
    induction_loop *selectedInductionLoop;
    
    QPointF sceneMapped = mapToScene(event->pos());
    
    if (event->button() == Qt::LeftButton)
    {
        switch (currentEditionMode)
        {
        case LINE:
            if (!segmentProcessRunning)
                startSegmentProcess(sceneMapped);
            else
            {
                if (lastKeyPressed == Qt::Key_Control)
                {
                    qDebug() << " X value" << gridPos.x();
                    sceneMapped = gridPos;
                }
                endSegmentProcess(sceneMapped);
            }
            break;
        case POLYLINE:
            if (!segmentProcessRunning)
                startSegmentProcess(sceneMapped);
            else
            {
                endSegmentProcess(sceneMapped);
                startSegmentProcess(sceneMapped);
            }
            break;
            
        case SELECT:
        {
            selectedPoint=gScene()->grepPoint(sceneMapped);
            selectedSegment=gScene()->grepSegment(sceneMapped);

            if (selectedSegment || selectedPoint)
                moveItemsProcessRunning=true;

            showItemFilterWidget();
        }
            break;
        case RECTSELECT:
            break;

        case MICROEDITION:
            selectedSegment=gScene()->grepSegment(sceneMapped);

            if (selectedSegment==NULL)
            {
                if (manouvreProcessRunning)
                {
                    manouvreProcessRunning=0;
                    scene()->removeItem(manouvrePolygon);
                    delete manouvrePolygon;
                }
                break;
            }

            selectedLane=selectedSegment->laneOnPos(sceneMapped);
            if (selectedLane==NULL)
            {
                qDebug() << "NULL lane?";
                return;
            }

            if (!manouvreProcessRunning)
            {

                manouvrePolygon=scene()->addPolygon(selectedSegment->lanePolygon(selectedLane),QPen(Qt::black),QBrush(Qt::yellow));


                showOnLabel2(QString("1st selected lane id ")+QString::number(selectedLane->id()));

                previousSelectedLane=selectedLane;
                manouvreProcessRunning=1;
            }
            else
            {
                manouvreProcessRunning=0;
                scene()->removeItem(manouvrePolygon);
                delete manouvrePolygon;

                if (selectedLane->parentSegment()==previousSelectedLane->parentSegment())
                {
                    showOnLabel2("Deselected lane");
                    break;
                }


                point *point1=selectedLane->parentSegment()->commonPoint(previousSelectedLane->parentSegment());

                if (point1!=NULL)
                {
                    //it has a node in common,lets see if the lanes have the same direction
                    //         if ((selectedLane->isForwardLane(point1) and (not previousSelectedLane->isForwardLane(point1))) or
                    //           ((not selectedLane->isForwardLane(point1)) and previousSelectedLane->isForwardLane(point1)))
                    {
                        //lets see if there is already a manouvre
                        if (gScene()->hasManouvre(selectedLane,previousSelectedLane))
                        {
                            if (gScene()->delManouvre(selectedLane,previousSelectedLane))
                                showOnLabel2("Manouvre removed");
                            else
                                showOnLabel2("Couldn't remove manouvre");
                        }
                        else
                        {
                            qDebug() << "adding manouvre";


                            if ( gScene()->addManouvre(selectedLane,previousSelectedLane))
                                showOnLabel2("Manouvre added");
                            else
                                showOnLabel2("Couldn't add manouvre");
                        }
                    }
                }
                else
                {
                    showOnLabel2("No Point in common");
                }
            }
            break;

        case INDUCTION_LOOP:
            selectedInductionLoop=gScene()->grepInductionLoop(sceneMapped);
            if (selectedInductionLoop)
            {
                qDebug() << "Encontrado Induction Loop";
                // ja existe
                QList<induction_loop*> l;
                l.push_back(selectedInductionLoop);
                inductionloopProperties->setInductionLoop(l);
                dock->setWidget(inductionloopProperties);
            }
            else
            {

                selectedSegment=gScene()->grepSegment(sceneMapped);

                if (selectedSegment==NULL)
                {
                    qDebug() << "NULL segment?";
                    showOnLabel1("Couldn't add induction loop, segment not found");
                    return;
                }
                selectedLane=selectedSegment->laneOnPos(sceneMapped);

                if (selectedLane==NULL)
                {
                    qDebug() << "NULL lane? this should not happen as we have already a segment:\n" << selectedSegment->info();
                    return;
                }


                gScene()->addItem(new induction_loop(selectedLane,sceneMapped));

                qDebug() << "Added induction loop to lane index=" << selectedSegment->lanes().indexOf(selectedLane) << " segmentid=" << selectedSegment->id();
            }
            break;
        default:
            break;
        }

    }
    else if (event->button() == Qt::RightButton)
    {
        if (segmentProcessRunning)
            cancelSegmentProcess();
        else
            createAndShowContextMenu(event->globalPos());
    }

    setMouseCursorOnPress(event);

    lastPos = event->pos();
    lastPosOnPress = event->pos();
    lastButtonPressed = event->button();


}

void centralGraphicsView::emptyDock()
{
    if (dock->widget()!=NULL)
        dock->setWidget(NULL);

}

void centralGraphicsView::unSelectPoints()
{
    qDebug() << "void centralGraphicsView::unSelectItems()";
    foreach (point *i,selectedPoints)
        i->setSelected(0);

    selectedPoints.clear();
}

void centralGraphicsView::unSelectSegments()
{
    qDebug() << "void centralGraphicsView::unSelectItems()";
    foreach (segment *i,selectedSegments)
        i->setSelected(0);

    selectedSegments.clear();

    emptyDock();
}

void centralGraphicsView::unSelectItems()
{
    unSelectPoints();
    unSelectSegments();
}

void centralGraphicsView::deletePoints(QList<point*> p)
{
    //this function provides on view pointerhandling
    unSelectPoints();//prevent crash
    int size=p.size();

    for (int i=0;i<p.size();i++)
        gScene()->deletePoint(p[i])?size--:0;

    if (dock->widget()==pointProperties)
        emptyDock();

    if (size)
        showOnLabel2("Couldn't delete all Point(s)");
    else
        showOnLabel2("Deleted Point(s)");
}




void centralGraphicsView::deleteSegments(QList<segment*> s)
{

    unSelectSegments();

    for (int i=0;i<s.size();i++)
        gScene()->deleteSegment(s[i]);

    if (dock->widget()==segmentProperties)
        emptyDock();

    showOnLabel2("Deleted Segment(s)");

}

void centralGraphicsView::filterSelection(QString s)
{
    if (s=="point")
        unSelectSegments();
    else if (s=="segment")
        unSelectPoints();

    showItemFilterWidget();
}


void centralGraphicsView::showItemFilterWidget()
{

    selectedPoints.clear();
    selectedSegments.clear();
    QList<QGraphicsItem*> items=scene()->selectedItems();

    if (items.size()==0)// || items.size()>1000) // too much items to handle
    {
        dock->setWidget(NULL);
        return;
    }

    for (int i=0;i<items.size();i++)
    {
        //qgraphicsitem_cast will verify item types
        point *p=qgraphicsitem_cast<point*>(items[i]);
        segment *s=qgraphicsitem_cast<segment*>(items[i]);

        if (p)
            selectedPoints.append(p);
        else if (s)
            selectedSegments.append(s);
    }

    if (selectedPoints.size() == 0)
    {
        segmentProperties->setSegments(selectedSegments);
        dock->setWidget(segmentProperties);
    }
    else if (selectedSegments.size()==0)
    {
        pointProperties->setPoints(selectedPoints);
        dock->setWidget(pointProperties);
    }
    else
    {

        QVBoxLayout *l = new QVBoxLayout;
        QWidget *w = new QWidget;
        QLabel *l1 = new QLabel("Filter types:");
        l->addStretch(200);

        l->addWidget(l1, Qt::AlignCenter);

        // segments
        QString str(QString::number(selectedSegments.size()));
        str.push_back(" segments");
        filterPushButton *b = new filterPushButton(str, "segment");
        connect(b, SIGNAL(clicked(QString)),this, SLOT(filterSelection(QString)));
        l->addWidget(b, Qt::AlignTop);

        //points
        str.clear();
        str=QString::number(selectedPoints.size());
        str.push_back(" points");
        filterPushButton *a = new filterPushButton(str, "point");
        connect(a, SIGNAL(clicked(QString)),this, SLOT(filterSelection(QString)));
        l->addWidget(a, Qt::AlignTop);

        l->addStretch(200);
        w->setLayout(l);

        dock->setWidget(w);

    }
}

void centralGraphicsView::mouseMoveEvent(QMouseEvent *event)
{
    //TODO mover updates para dentro dos objectos
    QGraphicsView::mouseMoveEvent(event);

    if ((lastButtonPressed == Qt::MidButton) || (currentEditionMode == PAN
                                                 && lastButtonPressed == Qt::LeftButton))
    {
        float x_var = mapToScene(event->pos()).x() - mapToScene(lastPos).x();
        float y_var = mapToScene(event->pos()).y() - mapToScene(lastPos).y();

        translate(x_var, y_var);

        setInteractive(false);
    }

    //######POINTER POSITION
    QPointF *tmp = new QPointF(mapToScene(event->pos()));
    tmp->setX(tmp->x()*X_TRANSFORM);
    tmp->setY(tmp->y()*Y_TRANSFORM);
    static QString mess;

    mess = QString::number((int) tmp->x());
    mess += ",";
    mess += QString::number((int) tmp->y());

    if (segmentProcessRunning)
    {
        mess += "     lenght=";
        mess += QString::number((int) drawingLine->line().length()/ DEFAULT_SCALE_FACTOR);
        mess += " m";
    }
    showOnLabel1(mess);
    //#############

    //CTRL key Control
    mousepos = mapToScene(event->pos());
    if (lastKeyPressed == Qt::Key_Control)
    {
        if (floor((abs(lastPosOnPress.x()-mousepos.x()) % GRIDSCALE) == 0) || (floor(abs(lastPosOnPress.y()-mousepos.y()) % GRIDSCALE) == 0))
            {
                gridPos.setX(mousepos.x());
                gridPos.setY(mousepos.y());
                normalModeMoveEvent(event);
            }
    }
    else normalModeMoveEvent(event);

    lastPos = event->pos();

}

inline void centralGraphicsView::normalModeMoveEvent(QMouseEvent *event)
{
    QPointF sceneMapped = mapToScene(event->pos());

    ////    QPointF delta(mapToScene(event->pos()).x()
    //              - mapToScene(lastPos).x(), mapToScene(event->pos()).y()
    //            - mapToScene(lastPos).y());

    QPointF delta(sceneMapped - mapToScene(lastPos));

    switch (currentEditionMode)
    {
    case POLYLINE:
    case LINE:
    {
        if (segmentProcessRunning)
            drawingLine->setLine(QLineF(drawingLine->line().p1(),sceneMapped));
        break;
    }
    case PAN:
    {
        break;
    }
    case SELECT:
        if (lastButtonPressed == Qt::LeftButton && lastKeyModifiers!=Qt::CTRL && moveItemsProcessRunning)
        //if (lastButtonPressed == Qt::LeftButton && moveItemsProcessRunning)
        {

            qDebug() << "selectedSegments.size() " << selectedSegments.size();

            //create a hash to prevent moving more than once, TODO MOVE to press event
            QHash<QString,point*> pointsToMove;
            for (int i=0;i<selectedSegments.size();i++)
            {
                point *start_p=selectedSegments[i]->startPoint();
                point *final_p=selectedSegments[i]->finalPoint();
                pointsToMove.insert(start_p->id(),start_p);
                pointsToMove.insert(final_p->id(),final_p);
            }

            for (int i=0;i<pointsToMove.values().size();i++)
                pointsToMove.values()[i]->moveBy(delta.x(),delta.y());

        }
        break;
    case RECTSELECT:
        //Qt Rubber drag works automatically
        break;
    default:
        break;
    }

}

void centralGraphicsView::mouseReleaseEvent(QMouseEvent *event)
{
    QGraphicsView::mouseReleaseEvent(event);

    qDebug() << "graphicsView::mouseReleaseEvent";

    static QString tmpString;

    if (event->button() == Qt::LeftButton)
    {
        switch (currentEditionMode)
        {
        case LINE:
            break;
        case SELECT:
            showOnLabel2("Ready");

            if (moveItemsProcessRunning)//TODO optimize CRUCIAL
            {
                for (int i=0;i<selectedSegments.size();i++)
                {
                    selectedSegments[i]->updateManouvres();
                }


                for (int i=0;i<selectedPoints.size();i++)
                {
                    for (int j=0;j<selectedPoints[i]->segments().size();j++)
                    {
                        selectedPoints[i]->segments()[j]->updateManouvres();
                    }
                }

            }
            moveItemsProcessRunning=false;

            break;
        case RECTSELECT:
            showItemFilterWidget();

            tmpString = QString("Selected ");
            tmpString.push_back(QString::number(selectedPoints.size()));
            tmpString.push_back(" points and ");
            tmpString.push_back(QString::number(selectedSegments.size()));
            tmpString.push_back(" segments");


            showOnLabel2(tmpString);


            break;
        default:
            break;
        }
    }
    else if ((event->button() == Qt::MidButton) || (currentEditionMode==PAN))
    {
        float x_vara = event->pos().x() - lastPosOnPress.x();
        float y_vara = event->pos().y() - lastPosOnPress.y();

        if (lastKeyModifiers != Qt::SHIFT)
        {
            QString tmp("map.panBy( new GSize(");
            tmp.push_back(QString::number(x_vara));
            tmp.push_back(",");
            tmp.push_back(QString::number(y_vara));
            tmp.push_back("))");
            gmaps_view->page()->mainFrame()->evaluateJavaScript(tmp);
        }

        setInteractive(true);
    }


    lastButtonPressed = Qt::NoButton;
    setMouseCursorOnRelease(event);

}

void centralGraphicsView::keyPressEvent(QKeyEvent * event)
{

    qDebug() << "void centralGraphicsView::keyPressEvent(QKeyEvent * event)";

    lastKeyModifiers = event->modifiers();


    if (event->key() == Qt::Key_Escape)
    {
        if (segmentProcessRunning)
            cancelSegmentProcess();

        setCurrentEditionMode(SELECT);
        unSelectItems();
    }
    else if (event->key() == Qt::Key_Space)
    {
        emit focusReceived();
    }
    else if (event->key() == Qt::Key_Delete)
    {
        deleteSegments(selectedSegments);
        deletePoints(selectedPoints);
        emptyDock();

    }
    //else if (event->key() == Qt::Key_Control)
    //{
    //    lastKeyPressed = Qt::Key_Control;
    //}
    else if (event->key() == Qt::Key_C && event->modifiers() == Qt::CTRL)
    {
        //	copyItems(selectedItems);
    }
    else if (event->key() == Qt::Key_X && event->modifiers() == Qt::CTRL)
    {
        //cutItems(selectedItems);
    }
    else if (event->key() == Qt::Key_V && event->modifiers() == Qt::CTRL)
    {
        //	pasteItems();
    }
    else if (event->key() == Qt::Key_A && event->modifiers() == Qt::CTRL)
    {

    }

    lastKeyPressed = event->key();
    //if (lastButtonPressed == Qt::Key_Control) qDebug() << "CTRL Pressed";
    qDebug() << "lastKeyPressed" << lastKeyPressed;

    QGraphicsView::keyPressEvent(event);
}

void centralGraphicsView::keyReleaseEvent(QKeyEvent *)
{

    qDebug() << "void centralGraphicsView::keyReleaseEvent(QKeyEvent *)";

    lastKeyPressed = 0;
    lastKeyModifiers = Qt::NoModifier;

}

inline void centralGraphicsView::cancelSegmentProcess()
{
    qDebug() << "centralGraphicsView::cancelRoadProcess()";

    if (!segmentProcessRunning)
    {
        qDebug() << "There wasn't a segmentProcess running!";
        return;
    }


    //	if (tmpNode->getAdjacentRoads().size() == 0)
    //	gScene()->removeNode(tmpNode);

    scene()->removeItem(drawingLine);
    segmentProcessRunning = 0;
    qDebug() << "roadProcess canceled";
    showOnLabel2("Segment creation canceled");
    //gScene()->update();
}

void centralGraphicsView::setCurrentEditionMode(EDITIONMODE mod)
{

    EDITIONMODE previousEditionMode = currentEditionMode;
    currentEditionMode = mod;

    if (segmentProcessRunning)
        if ((previousEditionMode != POLYLINE) || (previousEditionMode != LINE))
            cancelSegmentProcess();

    switch (previousEditionMode)
    {
    case SELECT:
        //dock->clearFocus();
        emptyDock();//watchout for segfaults
        unSelectPoints();
        unSelectSegments();
        break;
    case MICROEDITION:
        gScene()->stopMicroEditionMode();
        if (manouvreProcessRunning)
        {
            scene()->removeItem(manouvrePolygon);
            manouvreProcessRunning=0;
        }
        break;
    case RECTSELECT:
        break;
    default:
        break;
    }
    setDragMode(QGraphicsView::NoDrag);

}

void centralGraphicsView::startSelectMode()
{
    qDebug() << "graphicsView::startSelectMode()";
    setCurrentEditionMode(SELECT);
    setInteractive(true);
    setDragMode(QGraphicsView::NoDrag);
    setCursor(Qt::ArrowCursor);

    qDebug() << "changed";
}

void centralGraphicsView::startRectSelectMode()
{

    setCurrentEditionMode(RECTSELECT);
    setInteractive(true);
    setDragMode(QGraphicsView::RubberBandDrag);
    qDebug() << "graphicsView::startSelectMode()";
}

void centralGraphicsView::startLineMode()
{
    //setCursor(Qt::PointingHandCursor);
    setCursor(QCursor(QPixmap(":/images/mousecursor.png")));
    setInteractive(false);
    setCurrentEditionMode(LINE);
    qDebug() << "graphicsView::startDrawMode()";
}

void centralGraphicsView::startMicroEditionMode()
{
    setCursor(Qt::ArrowCursor);

    unSelectItems();
    setInteractive(false);
    //make manouvres visible
    gScene()->startMicroEditionMode();
    setDragMode(QGraphicsView::NoDrag);

    setCurrentEditionMode(MICROEDITION);
    qDebug() << "graphicsView::startMEDMode()";
}

void centralGraphicsView::startInductionLoopMode()
{
    setCursor(QCursor(QPixmap(":/images/mousecursor.png")));
    //setCursor(Qt::PointingHandCursor);
    setInteractive(false);
    setCurrentEditionMode(INDUCTION_LOOP);

    qDebug() << "entralGraphicsView::startInductionLoopMode()";
}

void centralGraphicsView::startPolylineMode()
{
    setCursor(QCursor(QPixmap(":/images/mousecursor.png")));
    //setCursor(Qt::PointingHandCursor);
    setInteractive(false);
    setCurrentEditionMode(POLYLINE);

    qDebug() << "graphicsView::startPolylineMode()";
}

void centralGraphicsView::startPanMode()
{
    setCurrentEditionMode(PAN);
    setCursor(Qt::OpenHandCursor);
    setInteractive(false);
    qDebug() << "graphicsView::startPanMode()";
}

QRectF centralGraphicsView::visibleRegion()
{
    return QRectF(mapToScene(0, 0).x(), mapToScene(0, 0).y(), width()
                  * scaleFactor, height() * scaleFactor);
}


void centralGraphicsView::receivedCommand(QString s)
{

    QStringList list = s.split(" ", QString::SkipEmptyParts);
    if (list.size() < 1)
        return;

    if (!segmentProcessRunning)
        if (list[0] == "refresh" && currentOverlayMode == GMAPS)
        {
            showOnLabel2("Reloaded page");

            gmaps_view->reload();
            return;
        }

    if (list[0] == "goto")
    {
        QString tmp("showAddress(\"");
        for (int i = 1; i < list.size(); i++)
            tmp += " " + list[i];
        tmp += "\")";
        gmaps_view->page()->mainFrame()->evaluateJavaScript(tmp);
        qDebug() << tmp;
        return;
    }
    else if (list[0] == "changemode" && list.size() == 2)
    {
        if (list[1] == "RECTSELECT")
            setCurrentEditionMode(RECTSELECT);

    }
    else if (list.size() == 2)//falta ver se e alpha
    {
        QPointF pos;

        pos.setX(tmpPoint->pos().x() + list[0].toFloat() * DEFAULT_SCALE_FACTOR);
        pos.setY(tmpPoint->pos().y() - list[1].toFloat() * DEFAULT_SCALE_FACTOR);

        if (currentEditionMode == LINE)
            endSegmentProcess(pos);
        else if (currentEditionMode == POLYLINE)
        {
            endSegmentProcess(pos);
            startSegmentProcess(pos);
        }
    }
    viewport()->update();
}

void centralGraphicsView::resizeEvent(QResizeEvent * event)
{
    qDebug() << "void centralGraphicsView::resizeEvent(QResizeEvent * event)";

    QGraphicsView::resizeEvent(event);
    if (gmaps_viewproxy != NULL)
        gmaps_viewproxy->setGeometry(QRectF(0, 0, width(), height()));

}

void centralGraphicsView::createAndShowContextMenu(QPoint p)
{
    QMenu *m = new QMenu(this);

    QAction *del = new QAction(QIcon(":/images/rubber.png"), "Delete items",this);
    m->addAction(del);
    connect(del, SIGNAL(triggered()), this, SLOT(deleteSelected()));

    //if (selectedItems.empty())
    del->setEnabled(0);

    m->popup(p);
}

void centralGraphicsView::selectAll()
{
    qDebug() << "void centralGraphicsView::selectAll()";

    if (segmentProcessRunning || manouvreProcessRunning || moveItemsProcessRunning || currentEditionMode==MICROEDITION)
        return;

    selectedSegments.clear();
    selectedPoints.clear();

    selectedSegments=gScene()->segments();
    selectedPoints=gScene()->points();

    for (int i=0;i<selectedSegments.size();i++)
        selectedSegments[i]->setSelected(1);

    for (int i=0;i<selectedPoints.size();i++)
        selectedPoints[i]->setSelected(1);

    showItemFilterWidget();

}

void centralGraphicsView::discretiseActions()
{
    qDebug() << "CRTL pressed" << currentEditionMode;

    switch (currentEditionMode)
    {
    case LINE:
        if (segmentProcessRunning)
        {
            //QPoint p = centralGraphicsView.mapFromGlobal(QCursor::pos());
            qDebug() << "Mouse X position";
        }
        break;
    default:
        break;
    }
}
