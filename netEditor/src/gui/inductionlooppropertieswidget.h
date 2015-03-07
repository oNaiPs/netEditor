#ifndef INDUCTIONLOOPPROPERTIESWIDGET_H
#define INDUCTIONLOOPPROPERTIESWIDGET_H

#include <QtGui>
#include <QLabel>
#include <QSpinBox>

#include "graphicsScene.h"

class inductionloopPropertiesWidget : public QWidget
{
    Q_OBJECT

    /// pointer to the scene, to access its objects
    graphicsScene *scene;

    QLabel *idLabel;
    QLabel *laneLabel;
    QSpinBox *freqSpinBox;

 QList<induction_loop*> currentInductionLoops;
public:
    explicit inductionloopPropertiesWidget(graphicsScene *s,QWidget *parent = 0);


    void setInductionLoop(QList<induction_loop*>);
signals:

public slots:
void freqSpinBoxChanged(int i);
};

#endif // INDUCTIONLOOPPROPERTIESWIDGET_H

