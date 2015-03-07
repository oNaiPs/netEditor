#include <QtGui>
#include <ogrsf_frmts.h>

#include "point.h"
#include "gdal_import.h"

QAction *gdal_import::importAction()
{
    QAction *a=new QAction("GDAL Import...",this);
    //settings_qaction->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_S));

    connect (a,SIGNAL(triggered()),SLOT(import()));

    return a;
}

void gdal_import::import()
{
    point *pp1,*pp2;

    qDebug() << "void gdal_import::import()";
    QString fileName = QFileDialog::getOpenFileName(0, tr("Open File"),QDir::currentPath(), tr("All Files (*.*)"));
    qDebug() << fileName;

    OGRRegisterAll();

    OGRDataSource *poDS;

    poDS = OGRSFDriverRegistrar::Open(fileName.toAscii(),FALSE);
    if (poDS == NULL)
    {
        QMessageBox::information(0, "Incompatible File Type!","Incompatible File Type!");
        return;
    }
    QTime t;
    t.start();
    OGRLayer *poLayer;

    poLayer = poDS->GetLayer(0);

    OGRFeature *poFeature;

    qDebug() << "poDS->GetLayerCount()" << poDS->GetLayerCount();

    //########### variables
    QString name;
    QVector<QPointF> points;
    QPointF point;

    poLayer->ResetReading();

    //for (int i=0;i<poLayer->GetFeatureCount();i++)
    //while ((poFeature = poLayer->GetNextFeature()) != NULL)
    int limit=poLayer->GetFeatureCount();
    QProgressDialog *progressDialog = new QProgressDialog("Importing...","Abort Import",0,limit);
    progressDialog->setWindowModality(Qt::WindowModal);

    int limit1=limit/100;

    for (int i=0;i<limit;i++)
    {

        if (i%limit1==0)
        {
            progressDialog->setValue(i);
            if (progressDialog->wasCanceled())
                break;
        }
        poFeature = poLayer->GetFeature(i);

        qDebug()  << (int) i / poLayer->GetFeatureCount();

        name = QString(poFeature->GetFieldAsString(4));

        OGRGeometry *poGeometry;

        poGeometry = poFeature->GetGeometryRef();
        if (poGeometry != NULL && wkbFlatten(poGeometry->getGeometryType()) == wkbPoint)
        {
            OGRPoint *poPoint = (OGRPoint *) poGeometry;

            qDebug() << poPoint->getX() << poPoint->getY();
        }
        else if (poGeometry != NULL && wkbFlatten(poGeometry->getGeometryType()) == wkbLineString)
        {
            OGRLineString *poLine = (OGRLineString *) poGeometry;

            points.clear();

            for (int i = 0; i < poLine->getNumPoints(); i++)
            {

                point.setX(poLine->getX(i) + 43000);
                point.setY(poLine->getY(i) - 166000);

                points.push_back(point);

            }
        }
        else if (poGeometry != NULL && wkbFlatten(poGeometry->getGeometryType()) == wkbMultiLineString)
        {
            OGRMultiLineString *poLine = (OGRMultiLineString *) poGeometry;
            points.clear();

            for (int i = 0; i < poLine->getDimension(); i++)
            {
                OGRLineString
                        *tmp =
                        dynamic_cast<OGRLineString*> (poLine->getGeometryRef(i));
                for (int i = 0; i < tmp->getNumPoints(); i++)
                {

                    point.setX(tmp->getX(i) + 43000);
                    point.setY(tmp->getY(i) - 166000);

                    points.push_back(point);

                }



            }

        }


        if (points.size()<2)
            continue;

        pp1=grepPoint(points.first());
        pp2=grepPoint(points.last());

        if (not pp1)
            pp1=createPoint(points.first());
        if (not pp2)
            pp2=createPoint(points.last());

        points.remove(0);
        points.remove(points.size()-1);

        segment *s=createSegment(pp1,pp2);

        if (s!=NULL)
        {
            s->setPolyline(points);
            s->setBackwardLanesNumber(1);
            s->setForwardLanesNumber(0);
        }

        points.clear();

        OGRFeature::DestroyFeature(poFeature);

        //view->createAndAddRoad(name, points);

    }

    progressDialog->setValue(limit);
    OGRDataSource::DestroyDataSource(poDS);
    qDebug() << t.elapsed() << "milis to import";
}

QAction *gdal_import::importSettings()
{
    return NULL;
}

QString gdal_import::importPluginDescription()
{
    return "GDAL lib import implementation";

}


Q_EXPORT_PLUGIN2(import_iface,gdal_import);
