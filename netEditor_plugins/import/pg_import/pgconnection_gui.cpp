#include <string.h>

#include "pg_import.h"
#include "pgconnection_gui.h"
#include "road.h"
#include "segment.h"
#include "point.h"

pgconnection_gui::pgconnection_gui(pg_import *parent) :
    QDialog(qobject_cast<QWidget*>(parent))
{
    ui.setupUi(this);

    pg_iface=parent;

    ui.ok_button->setDefault(1);
    connect(ui.ok_button, SIGNAL(clicked()), this, SLOT(ok()));
    connect(ui.cancel_button, SIGNAL(clicked()), this, SLOT(cancel()));
    connect(ui.test_button, SIGNAL(clicked()), this, SLOT(test()));

    QSettings settings;
    settings.beginGroup("postgres_connection");
    ui.host->setText(settings.value("host", "127.0.0.1").toString());
    ui.port->setValue(settings.value("port", "5432").toInt());
    ui.dbname->setText(settings.value("dbname", "").toString());
    ui.username->setText(settings.value("username", "").toString());
    ui.password->setText(settings.value("password", "").toString());
    settings.endGroup();
}

pgconnection_gui::~pgconnection_gui()
{

}

void pgconnection_gui::ok()
{

    //  if (QMessageBox::warning(this->parentWidget(),"Are you sure?","This operation will clear all items on the scene, do you want to continue? TODO",QMessageBox::Yes,QMessageBox::No)==QMessageBox::No)
    //    return;

    //save settings for the next time
    QSettings settings;
    settings.beginGroup("postgres_connection");
    settings.setValue("host", ui.host->text());
    settings.setValue("port", QString::number(ui.port->value()));
    settings.setValue("dbname", ui.dbname->text());
    settings.setValue("username", ui.username->text());
    settings.setValue("password", ui.password->text());
    settings.beginGroup("postgres_connection");
    close();

    QMap<int, point *> points;
    QMap<int, segment *> segments;
    QMap<int, road *> roads;
    QMap<int, lane*> lanes;

    PGconn *Conn;
    Conn = connect_now();
    if (PQstatus(Conn) != CONNECTION_OK)
    {
        showError(PQerrorMessage(Conn));
        return;
    }

    //########### DETERMINE ENVELOPE
    PGresult* R =PQexec(Conn,"select astext(st_envelope(st_collect(st_transform(the_geom,32629)))) from points;");//must change this to have in account points from segments
    if (PQresultStatus(R) != PGRES_TUPLES_OK)
    {
        showError(PQerrorMessage(Conn));
        PQclear(R);
        return;
    }

    QPolygonF p = pgpolygon_to_qpolygon(PQgetvalue(R, 0, 0));
    QPointF translate = p.boundingRect().center();

    //########### FETCH POINTS ###############
    QString command(
            "SELECT id,AsText(st_translate(st_transform(the_geom,32629),-");
    command.push_back(QString::number(translate.x()));
    command.push_back(",-");
    command.push_back(QString::number(translate.y()));
    command.push_back(")) FROM points;");

    qDebug() << command;
    R = PQexec(Conn, command.toAscii().data());
    if (PQresultStatus(R) != PGRES_TUPLES_OK)
    {
        showError(PQerrorMessage(Conn));
        PQclear(R);
        return;
    }

    int id_row = PQfnumber(R, "id");
    int pos_row = PQfnumber(R, "astext");

    for (int i = 0; i < PQntuples(R); i++)
    {
        point *tmp = new point();

        QString s = PQgetvalue(R, i, pos_row);
        QStringList sl = s.split(" ", QString::SkipEmptyParts);

        qreal xx = sl[0].right(sl[0].size() - 6).toFloat();
        qreal yy = sl[1].left(sl[1].size() - 1).toFloat();

        tmp->setPos(QPointF(xx * X_TRANSFORM,yy * Y_TRANSFORM));

        //tmp->setID(atoi(PQgetvalue(R, i, id_row)));

        qDebug() << "Created point id:" << tmp->id() << "x:" << tmp->x()
                << "y:" << tmp->y();

        qDebug() << tmp->info();
        //points.insert(tmp->id(), tmp);

        pg_iface->addPoint(tmp);

    }

    PQclear(R);

    //########### FECTH ROADS ####################
    R = PQexec(Conn, "SELECT * from roads;");
    if (PQresultStatus(R) != PGRES_TUPLES_OK)
    {
        showError(PQerrorMessage(Conn));
        PQclear(R);
        return;
    }

    int name_row = PQfnumber(R, "name");
    id_row = PQfnumber(R, "id");

    for (int i = 0; i < PQntuples(R); i++)
    {
        road *tmp = new road(PQgetvalue(R, i, name_row));
        tmp->setID(atoi(PQgetvalue(R, i, id_row)));

        roads.insert(tmp->id(), tmp);
        qDebug() << tmp->info();
    }

    PQclear(R);

    //######## FETCH SEGMENTS
    command.clear();
    command.push_back(
            "select id,start_point_id,final_point_id,road_id,lanes_number,lane_width,AsText(st_translate(st_transform(the_geom,32629),-");
    command.push_back(QString::number(translate.x()));
    command.push_back(",-");
    command.push_back(QString::number(translate.y()));
    command.push_back(")) from segments;");

    R = PQexec(Conn, command.toAscii().data());
    if (PQresultStatus(R) != PGRES_TUPLES_OK)
    {
        showError(PQerrorMessage(Conn));
        PQclear(R);
        return;
    }

    id_row = PQfnumber(R, "id");

    for (int i = 0; i < PQntuples(R); i++)
    {
        segment *tmp = new segment(points.value(atoi(PQgetvalue(R, i, PQfnumber(R,"start_point_id")))),points.value(atoi(PQgetvalue(R, i, PQfnumber(R,"final_point_id")))));
        //tmp->setID(atoi(PQgetvalue(R, i, id_row)));

        tmp->setLanesWidth(atof(PQgetvalue(R, i, PQfnumber(R, "lane_width"))));


        //MUDAR BASE DE DADOS
        tmp->setForwardLanesNumber(atoi(PQgetvalue(R, i,PQfnumber(R, "lanes_number"))));


        //############# path
        QStringList sl = QString(PQgetvalue(R, i, PQfnumber(R, "astext"))).split(",", QString::SkipEmptyParts);

        sl[0] = sl[0].right(sl[0].size() - 11);
        sl.last() = sl.last().left(sl.last().size() - 1);

        QStringList p1 = sl[0].split(" ", QString::SkipEmptyParts);
        qreal x1 = p1[0].toFloat();
        qreal y1 = p1[1].toFloat();

        QVector<QPointF> path;
        path.push_back(QPointF(x1 * X_TRANSFORM, y1 * Y_TRANSFORM));

        for (int j = 1; j < sl.size(); j++)
        {
            QStringList px = sl[j].split(" ", QString::SkipEmptyParts);
            qreal xx = px[0].toFloat();
            qreal yx = px[1].toFloat();
            path.push_back(QPointF(xx * X_TRANSFORM, yx * Y_TRANSFORM));
        }
        //######################
        tmp->setPolyline(path);
        tmp->setParentRoad(roads.value(
                atoi(PQgetvalue(R, i, PQfnumber(R, "road_id")))));
        qDebug() << tmp->info();

        //segments.insert(tmp->id(), tmp);
    }

    //######### FETCH LANES ##########

    R = PQexec(Conn, "select * from lanes;");
    if (PQresultStatus(R) != PGRES_TUPLES_OK)
    {
        showError(PQerrorMessage(Conn));
        PQclear(R);
        return;
    }
    for (int i = 0; i < PQntuples(R); i++)
    {
        lane *tmp = new lane(segments.value(atoi(PQgetvalue(R, i, PQfnumber(R,
                                                                            "segment_id")))));
        tmp->setCarAllowed(atoi(PQgetvalue(R, i, PQfnumber(R, "car_allowed"))));
        //tmp->setDirection(atoi(PQgetvalue(R, i, PQfnumber(R, "direction"))));
        tmp->setSpeedLimit(atoi(PQgetvalue(R, i, PQfnumber(R, "speed_limit"))));
      //  tmp->setID(atoi(PQgetvalue(R, i, PQfnumber(R, "id"))));

        //bug

        qDebug() << "Lane id:" << tmp->id();

        lanes.insert(tmp->id(), tmp);
    }

    //apply rules after creating
    for (int i = 0; i < PQntuples(R); i++)
    {

        //lane *tmp = lanes.value(atoi(PQgetvalue(R, i, PQfnumber(R, "id"))));

        qDebug() << atoi(PQgetvalue(R, i, PQfnumber(R, "left_lane")));

        //tmp->setLeftLane(lanes.value(atoi(PQgetvalue(R, i, PQfnumber(R,"left_lane")))));

        qDebug() << atoi(PQgetvalue(R, i, PQfnumber(R, "right_lane")));

        //tmp->setRightLane(lanes.value(atoi(PQgetvalue(R, i, PQfnumber(R,"right_lane")))));
    }

    //##################
    //tmp
    foreach (segment *s,segments)
    {
        pg_iface->addSegment(s);
    }



    //### tirar isto, usar funcoes especializadas
    //scene->_points=points;
    //scene->_lanes=lanes;

    //scene->update();

    qDebug() << "done";
}

void pgconnection_gui::cancel()
{
    qDebug() << "cancel";
    this->close();
}

void pgconnection_gui::test()
{
    PGconn *Conn;
    Conn = connect_now();
    if (PQstatus(Conn) == CONNECTION_OK)
    {
        QMessageBox::information(this, "Ready to connect",
                                 "Connection Sucessful!");

    }
    else
        showError(PQerrorMessage(Conn));
    PQfinish(Conn);
}

void pgconnection_gui::showError(char *c)
{
    QString a("The following error was reported:\n");
    a.push_back(c);
    QMessageBox::critical(this, "Error!", a);
}

PGconn *
        pgconnection_gui::connect_now()
{
    PGconn* Conn = NULL;

    Conn = PQconnectdb(((ui.host->text().isEmpty() ? "" : ("host="
                                                           + ui.host->text())) + (ui.port->text().isEmpty() ? "" : (" port="
                                                                                                                    + QString::number(ui.port->value())))
                        + (ui.dbname->text().isEmpty() ? "" : (" dbname="
                                                               + ui.dbname->text())) + (ui.username->text().isEmpty() ? ""
                                                                                    : (" user=" + ui.username->text()))
                        + (ui.password->text().isEmpty() ? "" : (" password="
                                                                 + ui.password->text()))).toAscii().data());

    return Conn;
}

QPolygonF &
        pgconnection_gui::pgpolygon_to_qpolygon(QString s)
{
    QStringList sl = s.split(" ", QString::SkipEmptyParts);
    QStringList new_sl;
    QList<QPointF> points;

    sl[0] = sl[0].right(sl[0].size() - 9);
    new_sl.push_back(sl[0]);

    for (int i = 1; i < sl.size() - 1; i++)
        new_sl += sl[i].split(',', QString::SkipEmptyParts);

    sl.last() = sl.last().left(sl.last().size() - 2);
    new_sl.push_back(sl.last());

    QPolygonF *p = new QPolygonF;

    for (int i = 1; i < new_sl.size(); i += 2)
        p->push_back(QPointF(new_sl[i - 1].toFloat(), new_sl[i].toFloat()));

    return *p;
}

