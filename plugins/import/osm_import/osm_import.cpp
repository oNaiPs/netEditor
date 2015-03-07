#include "osm_import.h"
#include "edges_reader.h"
#include "nodes_reader.h"
#include "connections_reader.h"
#include "osm_reader.h"
#include <QtGui>
#include <QtWebKit>

QAction *osm_import::importAction()
{
    QAction *a=new QAction("Import Network from OpenStreetMap...",this);
    //settings_qaction->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_S));

    connect (a,SIGNAL(triggered()),SLOT(import()));

    return a;
}

void osm_import::import()
{
    qDebug() << "void osm_import::import()";




    buildDialog();

}


void osm_import::buildDialog()
{
    dialog=new QDialog();
    dialog->setWindowTitle("OpenStreetMaps import plugin");

    QVBoxLayout *v=new QVBoxLayout;


    QPushButton *cancelb=new QPushButton("Cancel");
    QPushButton *okb=new QPushButton("Import this area...");


    connect (cancelb,SIGNAL(clicked()),dialog,SLOT(close()));
    connect(okb,SIGNAL(clicked()),this,SLOT(okButtonClicked()));



    QHBoxLayout *h=new QHBoxLayout;
    h->addWidget(okb);
    h->addWidget(cancelb);
    osm_view = new QWebView;
    v->addWidget(osm_view);
    v->addLayout(h);
    osm_view->load(QUrl("qrc:/osm.html"));

    dialog->setLayout(v);
    dialog->setWindowModality(Qt::ApplicationModal);
    dialog->show();

}

void osm_import::okButtonClicked()
{
    QVariant top,left,bottom,right;
    top=osm_view->page()->mainFrame()->evaluateJavaScript("getTop()");
    left=osm_view->page()->mainFrame()->evaluateJavaScript("getLeft()");
    right=osm_view->page()->mainFrame()->evaluateJavaScript("getRight()");
    bottom=osm_view->page()->mainFrame()->evaluateJavaScript("getBottom()");


    if (top.toString()=="" || left.toString()=="" || right.toString()=="" || bottom.toString()=="")
    {
        QMessageBox::warning(0,"Warning","Couldn't get bounding box, please wait for page to load");
        return;
    }

    QNetworkAccessManager *manager = new QNetworkAccessManager(this);
    connect(manager, SIGNAL(finished(QNetworkReply*)),this, SLOT(replyFinished(QNetworkReply*)));

    QString query("http://api.openstreetmap.org/api/0.6/map?bbox=");
    query.push_back(left.toString());
    query.push_back(",");
    query.push_back(bottom.toString());
    query.push_back(",");
    query.push_back(right.toString());
    query.push_back(",");
    query.push_back(top.toString());

    qDebug() << "query=" << query;

    QNetworkReply *reply=manager->get(QNetworkRequest(QUrl(query)));
    connect(reply,SIGNAL(downloadProgress(qint64,qint64)),this,SLOT(updateDownloadProgress(qint64,qint64)));

    progress=new QProgressDialog("Downloading network...", "Abort", 0, 0, NULL);
    progress->show();

    dialog->close();
}

void osm_import::updateDownloadProgress(qint64 b, qint64 total)
{
    qDebug() << "Downloading...";
    if (progress->maximum()==-1 && total!=-1)
        progress->setMaximum(total);

    if (b!=-1)
        progress->setValue(b);


}


void osm_import::replyFinished(QNetworkReply* n)
{
    QVariant top,left,bottom,right;
    top=osm_view->page()->mainFrame()->evaluateJavaScript("getTop()");
    left=osm_view->page()->mainFrame()->evaluateJavaScript("getLeft()");
    right=osm_view->page()->mainFrame()->evaluateJavaScript("getRight()");
    bottom=osm_view->page()->mainFrame()->evaluateJavaScript("getBottom()");

    progress->setLabelText("Adding to netEditor...");
    osm_reader o(n,QRectF(QPointF(top.toFloat(),left.toFloat()),QPointF(bottom.toFloat(),right.toFloat())),this);
    progress->close();
    qDebug() << "ola";
}

QAction *osm_import::importSettings()
{
    return NULL;
}


QString osm_import::importPluginDescription()
{
    return "This is the OpenStreetMap (alpha plugin)";

}


Q_EXPORT_PLUGIN2(import_iface,osm_import);
