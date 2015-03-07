#ifndef PGCONNECTION_GUI_H
#define PGCONNECTION_GUI_H

#include <QtGui>

#include "ui_pgconnection_gui.h"
#include "libpq-fe.h"
#include "graphicsScene.h"
#include "pg_import.h"

class graphicsScene;

class pgconnection_gui : public QDialog
{
    Q_OBJECT

    pg_import *pg_iface;
public:
    pgconnection_gui(pg_import *parent);
    ~pgconnection_gui();
    PGconn *connect_now();
private:
   // graphicsScene *scene;
    Ui::pgconnection_gui ui;
    void showError(char *);
    QPolygonF &pgpolygon_to_qpolygon(QString);
public slots:

	void ok();
	void cancel();
	void test();

};

#endif // PGCONNECTION_GUI_H
