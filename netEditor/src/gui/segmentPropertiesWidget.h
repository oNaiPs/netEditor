#ifndef SEGMENTPROPERTIESWIDGET_H_
#define SEGMENTPROPERTIESWIDGET_H_

#include <QWidget>
#include <QtGui>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QLabel>
#include <QPushButton>

#include "road.h"
#include "segment.h"
#include "graphicsScene.h"
#include "config.h"

class  centralGraphicsView;
/// This widget will be on a dock and pretends to display editable information about the segments selected on the scene, supporting the edition of a single or multiple objects.
class segmentPropertiesWidget:public QWidget
{
    Q_OBJECT

    /// Pointer to the scene, to have access to the objects in it
    graphicsScene *scene;

    QLineEdit *nameEdit;
    QSpinBox *forwardLanes;
    QSpinBox *backwardLanes;
    QDoubleSpinBox *laneWidthEdit;
    QLabel *lenght;
    QPushButton *_autoSetManouvres;

    /// Array of the segments currently managed by the properties widget
    QList<segment*> currentSegments;

public:
    /// Constructor, sets up the graphical interface and keeps the scene on a pointer
    segmentPropertiesWidget(graphicsScene *s);

    /// Sets the segments to be shown
    void setSegments(QList<segment*> &);

    //void setRoadName(QString s){nameEdit->setText(s);}
    //QString getRoadName(){return nameEdit->text();}

    void setLaneWidth(qreal a){laneWidthEdit->setValue(a);}
    qreal getLaneWidth(){return laneWidthEdit->value();}

    void setBackwardLanes(int a){backwardLanes->setValue(a);}
    int getBackwardLanes(){return backwardLanes->value();}

    void setForwardLanes(int a){forwardLanes->setValue(a);}
    int getForwardLanes(){return forwardLanes->value();}

    QSpinBox *getBackwardLanesBox(){return backwardLanes;}
    QSpinBox *getForwardLanesBox(){return forwardLanes;}

    //	void handleMultipleObjects(QList<road *>);

    //void roadPropertiesChanged(QList<graphicsItemObject*>);

signals:
    /// Signal emitted when some parameter on the widget is changed,to perform a real update on the affected segments
    void changed();

public slots:
    /// This routine automatically sets manouvres to the current segments, obeing some manouvre rules (in the future)
    void autoSetManouvres();

    /// Slot called by signal changed(), refer to it
    void segmentPropertiesChanged();

    /// This routine clears all manouvres on the current segments.
    void clearManouvres();
};

#endif /*ROADPROPERTIESWIDGET_H_*/
