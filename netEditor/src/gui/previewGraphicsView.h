#ifndef PREVIEWGRAPHICSVIEW_H_
#define PREVIEWGRAPHICSVIEW_H_

#include <QtGui>
#include "road.h"
#include "centralGraphicsView.h"

/// This inherited class from QGraphicsView allows to display on the application a little preview of the whole scene, and perform some actions related to mouse events, such as paning, etc...
class previewGraphicsView: public QGraphicsView
{

    Q_OBJECT

    Qt::MouseButton lastButtonPressed;
    QPoint lastPos;
    bool panProcess;

    QGraphicsRectItem *visibleRegion;
    centralGraphicsView *centralGV;
public:
    previewGraphicsView(QGraphicsScene *, centralGraphicsView *,
                        QWidget * parent = 0);

protected:

    void mousePressEvent(QMouseEvent *);
    void mouseMoveEvent(QMouseEvent *);
    void mouseReleaseEvent(QMouseEvent *);

    void paintEvent(QPaintEvent *event);

public slots:

    void previewChanged();

};

#endif /*PREVIEWGRAPHICSVIEW_H_*/
