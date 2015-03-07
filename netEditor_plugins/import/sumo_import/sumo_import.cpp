#include "sumo_import.h"
#include "edges_reader.h"
#include "nodes_reader.h"
#include "connections_reader.h"
#include <QtGui>


QAction *sumo_import::importAction()
{
    QAction *a=new QAction("Import Network from SUMO...",this);
    //settings_qaction->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_S));

    connect (a,SIGNAL(triggered()),SLOT(import()));

    return a;
}

void sumo_import::import()
{
    qDebug() << "Sumo import plugin... Starting";

    QDir d(QFileDialog::getExistingDirectory(0,"Select Sumo XML Files dir", "."));

    QStringList lista=d.entryList(QDir::Files);

    QString filesname;

    QHash<QString,point *> pointsHash;
    QHash<QString,segment *> segmentsHash;

    foreach (QString l,lista)
    {
        if (l.contains(".nod.xml"))
        {
            qDebug() << "Reading nodes...";
            QFile *fi=new QFile(d.absolutePath()+QDir::separator() + l);
            fi->open(QFile::ReadOnly);
            nodes_reader r(fi);
            pointsHash=r.getNodes();
            QList<point*> points=pointsHash.values();
            for (int i=0;i<points.size();i++)
                addPoint(points[i]);

            filesname=l.split(".nod.xml")[0];
        }

    }
    //if (d.absolutePath()+QDir::separator()+ filesname + QString(".edg.xml"))
    foreach (QString l,lista)
    {
        if (l.contains(".edg.xml"))
        {
            qDebug() << "Reading edges...";
            QFile *fi=new QFile(d.absolutePath()+QDir::separator() + l);
            fi->open(QFile::ReadOnly);
            edges_reader r(fi,pointsHash);
            segmentsHash=r.getSegments();
            QList<segment *> segments=segmentsHash.values();

            for (int i=0;i<segments.size();i++)
                addSegment(segments[i]);
        }
    }


    foreach (QString l,lista)
    {
        if (l.contains(".con.xml"))
        {
            qDebug() << "Reading connections...";
            QFile *fi=new QFile(d.absolutePath()+QDir::separator() + l);
            fi->open(QFile::ReadOnly);
            connections_reader r1(fi,segmentsHash);
            QList<lane *> l1s=r1.getL1s();
            QList<lane *> l2s=r1.getL2s();
            for (int i=0;i<l1s.size();i++)
                addManouvre(l1s[i],l2s[i]);
        }
    }
    qDebug() << "Sumo import plugin... Done";
}

QAction *sumo_import::importSettings()
{
    return NULL;
}


QString sumo_import::importPluginDescription()
{
    return "Sumo Import plugin -> Alpha Stuff";

}


Q_EXPORT_PLUGIN2(import_iface,sumo_import);
