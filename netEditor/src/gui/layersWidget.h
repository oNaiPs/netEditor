#ifndef LAYERS_H_
#define LAYERS_H_

#include <QWidget>
#include <QtGui>
#include <QTreeWidget>
#include <QVBoxLayout>
#include <QCheckBox>

#include "config.h"

class layersCheckBox;

class layersWidget: public QWidget
{
    Q_OBJECT

public:
    layersWidget();
    ~layersWidget();

    void initializeVariables();

    QTreeWidget *treeWidget;
    QVBoxLayout *layout;


public slots:

    void changeTreeWidgetItemState(QTreeWidgetItem *);


signals:
    void stateChanged(QString,quint32);

};

class layersCheckBox:public QCheckBox
{
    Q_OBJECT
    QTreeWidgetItem *item;
public:
    layersCheckBox(QTreeWidgetItem *i)
    {
        item=i;
        connect (this,SIGNAL(clicked()),this,SLOT(clica()));
    }

public slots:
    void clica(){emit clicked(item);}
signals:
    void clicked(QTreeWidgetItem *i);

};

#endif /*LAYERS_H_*/
