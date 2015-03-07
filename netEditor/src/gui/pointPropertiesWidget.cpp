#include "pointPropertiesWidget.h"

pointPropertiesWidget::pointPropertiesWidget(graphicsScene *s)
{
    scene=s;

    v=new QVBoxLayout;
    setLayout(v);

    pos=new QLabel;
    pos->setAlignment(Qt::AlignCenter);
    segments_=new QLabel;

    QHBoxLayout *h1=new QHBoxLayout;
    v->addWidget(pos);
    v->addWidget(segments_);
    v->addLayout(h1);


    flow=new QSpinBox(this);
    flow->setSuffix("cars/h");
    flow->setRange(1,2000);

    v->addWidget(flow);

    percentage=new QSpinBox(this);
    percentage->setSuffix("%");
    percentage->setRange(0,100);
    percentage->setValue(100);
    v->addWidget(percentage);

    table = new tableWidget(0, 4, this);//TODO tirar isto
    table->setColumnWidth(1, table->width() / table->columnCount());
    table->setColumnWidth(2, table->width() / table->columnCount());
    table->setColumnWidth(3, table->width() / table->columnCount());
    table->setColumnWidth(0, table->width() / table->columnCount());
    v->addWidget(table);

    _type_label=new QLabel("Type");
    _type=new QComboBox;
    //please respect point.point_type order
    _type->addItem("Priority");
    _type->addItem("Traffic Light");
    _type->addItem("Right Before Left");

    QHBoxLayout *h2=new QHBoxLayout;
    h2->addWidget(_type_label);
    h2->addWidget(_type);
    v->addLayout(h2);

    connect (percentage,SIGNAL(valueChanged(int)),this,SLOT(percentageChanged(int)));
    connect(flow,SIGNAL(valueChanged(int)),this,SLOT(flowChanged(int)));
    connect(table, SIGNAL(mousePressed()),this, SLOT(highlightWay()));
    connect(table, SIGNAL(mouseReleased()),this, SLOT(unHighlightWay()));
    connect(_type,SIGNAL(currentIndexChanged(int)),this,SLOT(typeChanged(int)));
}


void pointPropertiesWidget::typeChanged(int)
{
    if (_currentPoints.size()==1)
        _currentPoints[0]->setPointType((point_type)_type->currentIndex());
}
void pointPropertiesWidget::percentageChanged(int a)
{
    if (_currentPoints.size()==1)
        _currentPoints[0]->setCarsPercent(a);
}


void pointPropertiesWidget::flowChanged(int a)
{
    if (_currentPoints.size()==1)
        _currentPoints[0]->setFlow(a);
}

void pointPropertiesWidget::setPoints(QList<point*> &p)
{
    _currentPoints=p;

    if (p.size()!=1)
    {
        pos->setText("More than one");

        flow->setVisible(0);
        percentage->setVisible(0);
        table->setVisible(0);
        _type->setVisible(false);
        _type_label->setVisible(false);

    }
    else
    {

        table->setVisible(1);
        pos->setText(QString("x:") + QString::number(p[0]->pos().x())+ QString(" y:") + QString::number(p[0]->pos().y()));
        segments_->setText(QString::number(p[0]->segments().size())+QString(" segments"));

        if (_currentPoints[0]->isSource())
        {
            findPossibleWays();
            table->setVisible(1);
            percentage->setVisible(1);
            flow->setVisible(1);

            flow->setValue(p[0]->flow());
            percentage->setValue(p[0]->carsPercent());
        }
        else
        {
            table->setVisible(0);
            flow->setVisible(0);
            percentage->setVisible(0);
        }

        if (_currentPoints[0]->isSourceOrDrain())
        {
            _type->setVisible(false);
            _type_label->setVisible(false);
        }
        else
        {
            _type->setCurrentIndex(_currentPoints[0]->pointType());
            _type->setVisible(true);
            _type_label->setVisible(true);
        }

    }
}

void pointPropertiesWidget::findPossibleWays()
{
    qDebug() << "void pointPropertiesWidget::findPossibleWays()";

    _currentPoints[0]->processPossibleWays();
    QList<QList<point *> > possibleWays = _currentPoints[0]->getCalculatedPossibleWays();



    //###################### TABLE SORT

    table->setRowCount(possibleWays.size());

    QStringList list1;
    list1.push_back("O");
    list1.push_back("D");
    list1.push_back("r");
    list1.push_back("pi");
    table->setHorizontalHeaderLabels(list1);

    for (int i = 0; i < possibleWays.size(); i++)
    {
        QTableWidgetItem *a = new QTableWidgetItem(_currentPoints[0]->id());
        QTableWidgetItem *b = new QTableWidgetItem(possibleWays[i].last()->id());

        QString caminho;
        for (int j = 1; j < possibleWays[i].size() - 1; j++)
        {
            caminho += (possibleWays[i][j]->id());
            if (j!=possibleWays[i].size()-2)
                caminho +=QString(".");
        }

        QTableWidgetItem *c = new QTableWidgetItem(caminho);

        a->setFlags(Qt::NoItemFlags);
        b->setFlags(Qt::NoItemFlags);
        c->setFlags(Qt::NoItemFlags);

        table->setItem(i, 0, a);
        table->setItem(i, 1, b);
        table->setItem(i, 2, c);

    }

    table->sortByColumn(0, Qt::AscendingOrder);

    for (int i = 0; i < possibleWays.size(); i++)
    {
        QTableWidgetItem *d = new QTableWidgetItem(QString::number(_currentPoints[0]->possibleWaysDistribution[i]));
        table->setItem(i, 3, d);

    }

    table->setColumnWidth(0, 20);
    table->setColumnWidth(1, 20);
    table->setColumnWidth(2, 55);
    table->setColumnWidth(3, 33);


    table->resizeColumnsToContents();
    //connect(table, SIGNAL(itemChanged(QTableWidgetItem *)),this, SIGNAL(changed()));

}

void pointPropertiesWidget::highlightWay()
{
    QList<QList<point *> > pw = _currentPoints[0]->getCalculatedPossibleWays();


    foreach (QTableWidgetItem *item,table->selectedItems())
    {
        for (int i = 0; i < pw[item->row()].size() - 1; i++)
        {
            pw[item->row()][i]->getCommonSegment(pw[item->row()][i + 1])->setHighlighted(1);

            selectedWaySegments.push_back(pw[item->row()][i]->getCommonSegment(pw[item->row()][i + 1]));
        }
    }
}

void pointPropertiesWidget::unHighlightWay()
{

    foreach (segment *s,selectedWaySegments)
        s->setHighlighted(0);

    selectedWaySegments.clear();


}

