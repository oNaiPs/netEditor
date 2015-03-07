#ifndef TABLEWIDGET_H
#define TABLEWIDGET_H

#include <QtGui>
#include <QTableWidget>

/// Inherited class, to catch events from the mouse, nothing much to say
class tableWidget : public QTableWidget
{
    Q_OBJECT

public:

    tableWidget(int a,int b, QWidget *p=0);

        void mouseReleaseEvent ( QMouseEvent *);
        void mousePressEvent ( QMouseEvent *);
signals:
        void mouseReleased();
        void mousePressed();
};

#endif // TABLEWIDGET_H
