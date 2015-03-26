#include "inductionlooppropertieswidget.h"

inductionloopPropertiesWidget::inductionloopPropertiesWidget(graphicsScene *s,QWidget *parent) :
        QWidget(parent)
{
    scene=s;
    QVBoxLayout *v=new QVBoxLayout;
    v->setAlignment(Qt::AlignTop);

    idLabel=new QLabel;
    laneLabel=new QLabel;

    //    QHBoxLayout *h3=new QHBoxLayout;
    //    h3->addWidget(new QLabel("ID:"));
    //    h3->addWidget(idLabel);
    //    QHBoxLayout *h4=new QHBoxLayout;
    //    h4->addWidget(new QLabel("Lane ID:"));
    //    h4->addWidget(laneLabel);

    QHBoxLayout *h2=new QHBoxLayout;
    h2->addWidget(new QLabel("Frequency"));

    freqSpinBox=new QSpinBox();
    freqSpinBox->setRange(1,1000);
    freqSpinBox->setSingleStep(10);
    freqSpinBox->setValue(10);
    h2->addWidget(freqSpinBox);

//    v->addLayout(h3);
//    v->addLayout(h4);
    v->addLayout(h2);

    setLayout(v);

    connect(freqSpinBox,SIGNAL(valueChanged(int)),this,SLOT(freqSpinBoxChanged(int)));
}

void inductionloopPropertiesWidget::setInductionLoop(QList<induction_loop*> l)
{
    if (l.size()==1)
    {
        currentInductionLoops=l;
        induction_loop *in=l.first();
        freqSpinBox->setValue(in->frequency());
    }
    else
    {
        qDebug() << "ERRO, ainda nao implementado (induction loop prop)";
    }
}
void inductionloopPropertiesWidget::freqSpinBoxChanged(int f)
{
    foreach (induction_loop *i,currentInductionLoops)
        i->setFrequency(f);

}
