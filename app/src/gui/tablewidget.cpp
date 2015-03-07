#include "tablewidget.h"

tableWidget::tableWidget(int a,int b, QWidget *p):QTableWidget(a,b,p)
{
}

void tableWidget::mouseReleaseEvent ( QMouseEvent *e)
{


    QTableWidget::mouseReleaseEvent(e);
        emit mouseReleased();
}

void tableWidget::mousePressEvent ( QMouseEvent *e)
{
    QTableWidget::mousePressEvent(e);

emit mousePressed();
}
