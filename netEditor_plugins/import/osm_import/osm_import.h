#ifndef EXAMPLE_IMPORT_H
#define EXAMPLE_IMPORT_H

#include "iface.h"
#include <QtGui>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QtWebKit>

class osm_import:public import_iface
{
    Q_OBJECT
    Q_INTERFACES(import_iface)

    QDialog *dialog;
    QWebView *osm_view;
    QProgressDialog *progress;
    void buildDialog();
     QNetworkReply *reply;
public:

    QAction *importAction();
    QString importPluginDescription();
    QAction *importSettings();

public slots:
    void replyFinished(QNetworkReply*);
    void import();
    void updateDownloadProgress(qint64,qint64);
    void okButtonClicked();
};
#endif
