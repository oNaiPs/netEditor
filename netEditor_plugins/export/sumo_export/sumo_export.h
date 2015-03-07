#ifndef  SUMO_EXPORT_H
#define  SUMO_EXPORT_H

#include "iface.h"
#include <QObject>
#include <QString>


class sumo_settings:public QDialog
{
    Q_OBJECT

    QString customExecDir;
    QLineEdit *netconvert,*sumogui;
    QLineEdit *dirPath,*_traciPort;
    QCheckBox *runSumo,*enableTraci;
    QPushButton *ok;


public:
    sumo_settings();
    ~sumo_settings();
    QPushButton *okButton(){return ok;}
    QString exportDir(){return dirPath->text();}
    bool traciEnabled(){return enableTraci->isChecked();}
    bool runAfterExport(){return runSumo->isChecked();}
    QString traciPort(){return _traciPort->text();}

public slots:
    QString findExecutable(QString);

    void setExecutables();
    void setCustomExecDir();
    QString netconvertExecutable();
    //QString od2tripsExecutable();
    QString sumoguiExecutable();

    void browseDir();

};

class sumo_export:public export_iface
{
    Q_OBJECT

    Q_INTERFACES(export_iface)

    sumo_settings *_settings;

        public:

    sumo_export();
    ~sumo_export();
    QAction *exportAction();
    QString exportPluginDescription();
    QAction *exportSettings();
    sumo_settings *settings();


public slots:
    void procede_export();
    void export1();
};


#endif
