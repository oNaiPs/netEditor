#include <QtGui>
#include <QLineEdit>

#include "segmentPropertiesWidget.h"

segmentPropertiesWidget::segmentPropertiesWidget(graphicsScene *s)
{
    scene=s;

    QVBoxLayout *vlayout1 = new QVBoxLayout;
    QHBoxLayout *hlayout1 = new QHBoxLayout;
    QHBoxLayout *hlayout2 = new QHBoxLayout;
    QHBoxLayout *hlayout3 = new QHBoxLayout;
    QHBoxLayout *hlayout4 = new QHBoxLayout;

    //###############
    nameEdit = new QLineEdit;
    nameEdit->setEnabled(0);

    
    QLabel *nameLabel = new QLabel("Road Name");
    hlayout4->addWidget(nameLabel);
    hlayout4->addWidget(nameEdit);

    vlayout1->addLayout(hlayout4);
    //################

    laneWidthEdit = new QDoubleSpinBox;
    laneWidthEdit->setSuffix("m");
    laneWidthEdit->setMinimum(MINIMUM_LANE_WIDTH);
    QLabel *widthLabel = new QLabel("Lane Width");
    hlayout1->addWidget(widthLabel);
    hlayout1->addWidget(laneWidthEdit);

    vlayout1->addLayout(hlayout1);

    //###################
    forwardLanes = new QSpinBox;
    forwardLanes->setMinimum(0);
    QLabel *forwardLabel = new QLabel("Forward Lanes");
    hlayout2->addWidget(forwardLabel);
    hlayout2->addWidget(forwardLanes);

    vlayout1->addLayout(hlayout2);
    //##################
    backwardLanes = new QSpinBox;

    QLabel *backwardLabel = new QLabel("Backward Lanes");
    hlayout3->addWidget(backwardLabel);
    hlayout3->addWidget(backwardLanes);

    vlayout1->addLayout(hlayout3);

    //###############
    lenght = new QLabel;
    vlayout1->addWidget(lenght);



    QPushButton *clearm=new QPushButton("Clear Manouvres");
    connect (clearm,SIGNAL(clicked()),this,SLOT(clearManouvres()));
    vlayout1->addWidget(clearm);


    vlayout1->setAlignment(Qt::AlignTop);
    this->setLayout(vlayout1);


    _autoSetManouvres=new QPushButton("AutoSet Manouvres");
    vlayout1->addWidget(_autoSetManouvres);

    connect(_autoSetManouvres,SIGNAL(clicked()),this,SLOT(autoSetManouvres()));

    //#############

    connect(laneWidthEdit, SIGNAL(valueChanged(double)),this, SIGNAL(changed()));
    connect(forwardLanes, SIGNAL(valueChanged(int)),this, SIGNAL(changed()));
    connect(backwardLanes, SIGNAL(valueChanged(int)),this, SIGNAL(changed()));
    connect(nameEdit, SIGNAL(editingFinished()),this, SIGNAL(changed()));

    nameEdit->setText("Not impl");
}

void segmentPropertiesWidget::setSegments(QList<segment*> &l)
{
    currentSegments=l;

    disconnect(SIGNAL(changed()));

    forwardLanes->setMinimum(-1);
    backwardLanes->setMinimum(-1);
    laneWidthEdit->setMinimum(0);

    forwardLanes->setSpecialValueText("Prev");
    backwardLanes->setSpecialValueText("Prev");
    laneWidthEdit->setSpecialValueText("Prev");

    if (l.size()==0)
        return;
    if (l.size()==1)
    {
        setForwardLanes(l[0]->forwardLanes(NULL).size());
        setBackwardLanes(l[0]->backwardLanes(NULL).size());
        laneWidthEdit->setValue(l[0]->lanesWidth());



        lenght->setText(QString::number(QLineF(l[0]->startPoint()->pos(),l[0]->finalPoint()->pos()).length()) + QString(" meters"));
    }
    if (l.size()<1000)
    {
        int fl=l[0]->forwardLanes(NULL).size();
        int bl=l[0]->backwardLanes(NULL).size();
        int lw=l[0]->lanesWidth();
        bool fl_ok=1,bl_ok=1,lw_ok=1;

        for (int i=0;i<l.size();i++)
        {
            if (l[i]->forwardLanes(NULL).size()!=fl)
                fl_ok&=false;
            if (l[i]->backwardLanes(NULL).size()!=bl)
                bl_ok&=false;
            if (lw!=l[i]->lanesWidth())
                lw_ok&=false;
        }
        if (fl_ok)
            forwardLanes->setValue(fl);
        else
            forwardLanes->setValue(-1);

        if (bl_ok)
            backwardLanes->setValue(bl);
        else
            backwardLanes->setValue(-1);

        if (lw_ok)
            laneWidthEdit->setValue(lw);
        else
            laneWidthEdit->setValue(-1);

    }
    else
    {
        forwardLanes->setValue(-1);
        backwardLanes->setValue(-1);
        laneWidthEdit->setValue(-1);
    }

    connect(this, SIGNAL(changed()), this, SLOT(segmentPropertiesChanged()));
}

void segmentPropertiesWidget::segmentPropertiesChanged()
{
    disconnect(SIGNAL(changed()));

    //nao esquecer nao deixar ficar 0 lanes
    if (currentSegments.size()==0)return;

    forwardLanes->setMinimum(0);
    backwardLanes->setMinimum(0);
    laneWidthEdit->setMinimum(MINIMUM_LANE_WIDTH);

    forwardLanes->setSpecialValueText("");
    backwardLanes->setSpecialValueText("");
    laneWidthEdit->setSpecialValueText("");

    foreach (segment *s,currentSegments)
    {
        if (!forwardLanes->value() && !backwardLanes->value())
            forwardLanes->setValue(1);

        if (forwardLanes->value()!=-1)
            s->setForwardLanesNumber(forwardLanes->text().toInt());

        if (backwardLanes->value()!=-1)
            s->setBackwardLanesNumber(backwardLanes->text().toInt());

        if (laneWidthEdit->value()!=0)
            s->setLanesWidth(laneWidthEdit->value());

        s->preprocessLook();
        s->updateManouvres();

    }
    connect(this, SIGNAL(changed()), this, SLOT(segmentPropertiesChanged()));

}

void segmentPropertiesWidget::autoSetManouvres()
{
    QList<point *> _currentPoints;
    for (int i=0;i<currentSegments.size();i++)
    {
        if(! _currentPoints.contains(currentSegments[i]->startPoint()))
            _currentPoints.push_back(currentSegments[i]->startPoint());
        if (! _currentPoints.contains(currentSegments[i]->finalPoint()))
            _currentPoints.push_back(currentSegments[i]->finalPoint());
    }

    for (int j=0;j<_currentPoints.size();j++)
    {
        if (_currentPoints[j]->segments().size()<2)
            continue;
        for (int i=0;i<_currentPoints[j]->segments().size();i++)
        {
            for (int k=0;k<_currentPoints[j]->segments().size();k++)
            {
                if(_currentPoints[j]->segments()[i]==_currentPoints[j]->segments()[k])
                    continue;
                segment *s1=_currentPoints[j]->segments()[i];
                segment *s2=_currentPoints[j]->segments()[k];
                int ok=0;
                if (s1->forwardLanes(_currentPoints[j]).size()==s2->backwardLanes(_currentPoints[j]).size())
                    ok++;
                if (s1->backwardLanes(_currentPoints[j]).size()==s2->forwardLanes(_currentPoints[j]).size())
                    ok++;
                if (ok==2)
                {
                    for (int l=0;l<s1->forwardLanes(_currentPoints[j]).size();l++)
                    {
                        scene->addManouvre(s1->forwardLanes(_currentPoints[j])[l],s2->backwardLanes(_currentPoints[j])[l]);
                    }
                    for (int l=0;l<s1->backwardLanes(_currentPoints[j]).size();l++)
                    {
                        scene->addManouvre(s1->backwardLanes(_currentPoints[j])[l],s2->forwardLanes(_currentPoints[j])[l]);
                    }
                }
            }
        }
    }
}


void segmentPropertiesWidget::clearManouvres()
{

    QVector< manouvre * >m;

    for (int j=0;j<currentSegments.size();j++)
    {
        for (int k=0;k<currentSegments[j]->lanes().size();k++)
        {
            for (int l=0;l<currentSegments[j]->lanes()[k]->originManouvres.size();l++)
                if (! m.contains(currentSegments[j]->lanes()[k]->originManouvres[l]))
                    m.push_back(currentSegments[j]->lanes()[k]->originManouvres[l]);
              for (int l=0;l<currentSegments[j]->lanes()[k]->destinManouvres.size();l++)
                if (! m.contains(currentSegments[j]->lanes()[k]->destinManouvres[l]))
                    m.push_back(currentSegments[j]->lanes()[k]->destinManouvres[l]);
        }
    }

    for (int i=0;i<m.size();i++)
                scene->delManouvre(m[i]);
}
