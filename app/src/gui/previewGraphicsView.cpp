#include "previewGraphicsView.h"
#include <QGLWidget>

previewGraphicsView::previewGraphicsView(QGraphicsScene *scene,centralGraphicsView *v, QWidget *parent) :
        QGraphicsView(scene, parent)
{

    //	this->setOptimizationFlags(QGraphicsView::DontClipPainter|QGraphicsView::DontSavePainterState|QGraphicsView::DontAdjustForAntialiasing);
    //	this->setViewport(new QGLWidget(QGLFormat(QGL::SampleBuffers)));//FOR OPENGL VIEW

    panProcess = 0;
    centralGV = v;
    visibleRegion = new QGraphicsRectItem;
    visibleRegion->setBrush(QColor(200, 200, 210, 200));
    //previewChanged();
    this->viewport()->setMouseTracking(0);

    fitInView(QRectF(-100000, -100000, 200000, 200000), Qt::KeepAspectRatio);
}

void previewGraphicsView::previewChanged()
{
    fitInView(sceneRect(), Qt::KeepAspectRatio);
}

void previewGraphicsView::paintEvent(QPaintEvent *event)
{
    QGraphicsView::paintEvent(event);
    //TODO tirar este comment

    // Set up the painter
    QPainter painter(viewport());
    painter.setRenderHint(QPainter::Antialiasing, renderHints()
                          & QPainter::Antialiasing);
    painter.setRenderHint(QPainter::SmoothPixmapTransform, renderHints()
                          & QPainter::SmoothPixmapTransform);
    painter.setTransform(viewportTransform());

    QStyleOptionGraphicsItem option;
    option.state = QStyle::State_None;

    visibleRegion->setRect(centralGV->visibleRegion());
    QGraphicsItem **itemArray = new QGraphicsItem *[1];
    itemArray[0] = visibleRegion;

    drawItems(&painter, 1, itemArray, &option);
}

void previewGraphicsView::mousePressEvent(QMouseEvent *event)
{

    if (centralGV->visibleRegion().contains(mapToScene(event->pos())))
        panProcess = 1;

    lastPos = event->pos();
    lastButtonPressed = event->button();
}

void previewGraphicsView::mouseMoveEvent(QMouseEvent *event)
{
    QPointF panPos = lastPos - event->pos();


    if (panProcess && lastButtonPressed == Qt::LeftButton)
    {
        float x_var =  mapToScene(lastPos).x()- mapToScene(event->pos()).x();
        float y_var = mapToScene(lastPos).y()- mapToScene(event->pos()).y();

        centralGV->translate(x_var, y_var);
    }


    viewport()->update();

    lastPos = event->pos();

}
void previewGraphicsView::mouseReleaseEvent(QMouseEvent *event)
{

    panProcess = 0;
    lastPos = event->pos();

}
