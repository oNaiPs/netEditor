#include "layersWidget.h"

layersWidget::layersWidget()
{

    layout = new QVBoxLayout;

    setLayout(layout);

    treeWidget = new QTreeWidget();
    treeWidget->setColumnCount(2);

    QStringList la;
    la << "Object" << "Visible";
    treeWidget->setHeaderLabels(la);
    layout->addWidget(treeWidget);

}

void layersWidget::initializeVariables()
{

    QSettings settings;
    quint32 state;

    //format for DEFAULT_COMPONTENTS:     "MAJOR_COMP(minor,minor,minor,(...),minor) MAJOR_COMP(...)"
    QString def_components=DEFAULT_COMPONENTS;

    qDebug() << "Populating LayersWidget...";

    QStringList components=def_components.split(' ');

    for (int i=0;i<components.size();i++)
    {
        QStringList split1=components[i].split('(');

        //it must respect the format
        if (split1.size()!=2)qDebug() << "ERROR on layersWidget::initializeVariables()";

        if (split1[0]=="Points")
            state=settings.value("layers/POINTS_DRAWING_STATE",DEFAULT_POINT_DRAWING_STATE).toInt();
        else if (split1[0]=="Segments")
            state=settings.value("layers/SEGMENTS_DRAWING_STATE",DEFAULT_SEGMENT_DRAWING_STATE).toInt();
        else
            state=settings.value(QString("layers/")+split1[0]+QString("_DRAWING_STATE"),"").toInt();


        QTreeWidgetItem *tmp_i = new QTreeWidgetItem(treeWidget);
        layersCheckBox *ck=new layersCheckBox(tmp_i);

        tmp_i->setText(0, split1[0]);

        if (GET_BIT(state,OBJECT_TOGGLE))
            ck->setChecked(1);
        else
            ck->setChecked(0);

        treeWidget->insertTopLevelItem(0, tmp_i);
        treeWidget->setItemWidget(tmp_i,1,ck);
        connect (ck,SIGNAL(clicked(QTreeWidgetItem*)),this,SLOT(changeTreeWidgetItemState(QTreeWidgetItem*)));

        QStringList split2=split1[1].split(',');
        split2.last().resize(split2.last().size()-1);


        for (int j=0;j<split2.size();j++)
        {
            qDebug() << "---" << split2[j];

            QTreeWidgetItem *tmp = new QTreeWidgetItem(tmp_i);
            layersCheckBox *ck=new layersCheckBox(tmp_i);
            tmp->setText(0, split2[j]);

            if (GET_BIT(state,OBJECT_TOGGLE+j+1))
                ck->setChecked(1);
            else
                ck->setChecked(0);


            treeWidget->insertTopLevelItem(1, tmp);
            treeWidget->setItemWidget(tmp,1,ck);
            connect (ck,SIGNAL(clicked(QTreeWidgetItem*)),this,SLOT(changeTreeWidgetItemState(QTreeWidgetItem*)));
        }

    }
    treeWidget->expandAll();
    treeWidget->resizeColumnToContents(0);
    treeWidget->resizeColumnToContents(1);

}

layersWidget::~layersWidget()
{
    QSettings settings;

    for (int i=0;i<treeWidget->topLevelItemCount();i++)
    {

        QString object_l=treeWidget->topLevelItem(i)->text(0);
        quint32 state=0;

        if(dynamic_cast<layersCheckBox*>(treeWidget->itemWidget(treeWidget->topLevelItem(i),1))->isChecked())
            SET_BIT(state,OBJECT_TOGGLE);
        else UNSET_BIT(state,OBJECT_TOGGLE);

        for (int j=0;j<treeWidget->topLevelItem(i)->childCount();j++)
            if(dynamic_cast<layersCheckBox*>(treeWidget->itemWidget(treeWidget->topLevelItem(i)->child(j),1))->isChecked())
                SET_BIT(state,OBJECT_TOGGLE+j+1);
            else UNSET_BIT(state,OBJECT_TOGGLE+j+1);

        settings.setValue(QString("layers/")+object_l.toUpper()+QString("_DRAWING_STATE"),state);
    }

}

void layersWidget::changeTreeWidgetItemState(QTreeWidgetItem *i)
{

    qDebug() << "void changeTreeWidgetItemState( QTreeWidgetItem * item)";

    //not very beautifull
    QTreeWidgetItem *item;
    if (i->childCount()==0)
        item=i->parent();
    else
        item=i;
    //

    QString object_l=item->text(0);
    quint32 state=0;



    if (dynamic_cast<layersCheckBox*>(treeWidget->itemWidget(item,1))->isChecked())
        SET_BIT(state,OBJECT_TOGGLE);
    else UNSET_BIT(state,OBJECT_TOGGLE);

    for (int j=0;j<item->childCount();j++)
    {
        if(dynamic_cast<layersCheckBox*>(treeWidget->itemWidget(item->child(j),1))->isChecked())
            SET_BIT(state,OBJECT_TOGGLE+j+1);
        else UNSET_BIT(state,OBJECT_TOGGLE+j+1);
    }
    emit stateChanged(object_l,state);


}
