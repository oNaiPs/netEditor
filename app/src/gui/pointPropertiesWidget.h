#ifndef POINTPROPERTIESWIDGET_H
#define POINTPROPERTIESWIDGET_H

#include <QWidget>
#include <QtGui>
#include <QComboBox>
#include <QSpinBox>
#include <QLabel>

#include "graphicsScene.h"
#include "config.h"
#include "tablewidget.h"

class centralGraphicsView;

/// This widget will be on a dock and is intended to display editable information about the points selected on the scene, supporting the edition of a single or multiple objects.
class pointPropertiesWidget:public QWidget
{
    Q_OBJECT

    /// pointer to the scene, to access its objects
    graphicsScene *scene;

    QSpinBox *flow,*percentage;
    QLabel *pos,*segments_,*_type_label;
    QVBoxLayout *v;
    QList<point*> _currentPoints;
    QList<segment*> selectedWaySegments;
    QComboBox *_type;

    tableWidget *table;


public:

    pointPropertiesWidget(graphicsScene *);

    /// Sets the points to be managed by the widget
    void setPoints(QList<point*> &);

protected:

signals:
    void changed();

public slots:
    void typeChanged(int);
    void flowChanged(int);
    void percentageChanged(int);

    /// Routine used to show the possible ways on the table
    void findPossibleWays();
    void highlightWay();
    void unHighlightWay();
};
#endif // POINTPROPERTIESWIDGET_H
