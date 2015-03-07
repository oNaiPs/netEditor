#ifndef REALPARSER_H_
#define REALPARSER_H_

#include <QtGui>
#include <iostream>

#include "point.h"
#include "realRoad.h"
#include "iface.h"

class feup_export_settings;

class feup_simulator_export:public export_iface
{
    Q_OBJECT


    Q_INTERFACES(export_iface)

            feup_export_settings *_settings;

        private:

    QList<realRoad *> realRoads;
    QList<point *> nodeList;
    QAction *exportAction();
    QString exportPluginDescription();
    QAction *exportSettings();

public:
feup_simulator_export();
~feup_simulator_export();
    void addRoad(QList<point *>);

public slots:
    void export1();
    void procede_export();
};

class feup_export_settings:public QDialog
{
    Q_OBJECT

    QString customExecDir;
    QLineEdit *simulator_exec;
    QLineEdit *dirPath;
    QCheckBox *runSimulator;
    QPushButton *ok;

public:
    feup_export_settings();
    ~feup_export_settings();
    QPushButton *okButton(){return ok;}
    QString exportDir(){return dirPath->text();}
    bool runAfterExport(){return runSimulator->isChecked();}

public slots:
    QString findExecutable(QString);

    void setExecutables();
    void setCustomExecDir();
    QString feupSimulatorExecutable();

    void browseDir();

};


#endif /*REALPARSER_H_*/
