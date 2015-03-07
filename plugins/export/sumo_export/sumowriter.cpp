#include "sumowriter.h"
#include "nodes_writer.h"
#include "edges_writer.h"
#include "connections_writer.h"
#include "demand_writer.h"
#include "cfg_writer.h"
#include "induction_loops_sumo.h"

sumowriter::sumowriter(QString dir,sumo_export *e)
{

    //###################NODES################
    qDebug() << "Starting parsing nodes file";
    QFile nodesFile(dir + "/editor.nod.xml");

    if (!nodesFile.open(QFile::WriteOnly | QFile::Text))
    {
        qDebug() << "Couldn't open file for Writing! Returning";
        return;
    }

    nodes_writer parser(e->getPoints());
    parser.writeFile(&nodesFile);
    qDebug() << "Nodes file created!";
    //#################EDGES################
    qDebug() << "Starting parsing edges file";
    QFile roadsFile(dir + "/editor.edg.xml");

    if (!roadsFile.open(QFile::WriteOnly | QFile::Text))
    {
        qDebug() << "Couldn't open file for Writing! Returning";
        return;
    }

    edges_writer parser1(e->getSegments());
    parser1.writeFile(&roadsFile);
    qDebug() << "Roads file created!";
    //#################CONNECTIONS##############
    qDebug() << "Starting parsing connections file";
    QFile connFile(dir+"/editor.con.xml");

    if (!connFile.open(QFile::WriteOnly|QFile::Text))
    {
        qDebug() << "Couldn't open file for Writing! Returning";
        return;
    }

    connections_writer conn(e->getSegments());
    conn.writeFile(&connFile);
    //##############DEMAND#############

    qDebug() << "Starting parsing demand file";
    QFile demFile(dir+"/editor.rou.xml");

    if (!demFile.open(QFile::WriteOnly|QFile::Text))
    {
        qDebug() << "Couldn't open file for Writing! Returning";
        return;
    }

    demand_writer dem(e->getPoints());
    dem.writeFile(&demFile);
    //############CFG FILE###############
    QFile cfgFile(dir+"/editor.sumo.cfg");

    if (!cfgFile.open(QFile::WriteOnly|QFile::Text))
    {
        qDebug() << "Couldn't open file for Writing! Returning";
        return;
    }

    cfg_writer cfg(e->settings());
    cfg.writeFile(&cfgFile);
    //################INDUCTION LOOPS

    QFile indFile(dir+"/editor.det.xml");

    if (!indFile.open(QFile::WriteOnly|QFile::Text))
    {
        qDebug() << "Couldn't open file for Writing! Returning";
        return;
    }

    induction_loops_sumo ind(e->getSegments());
    ind.writeFile(&indFile);
}
