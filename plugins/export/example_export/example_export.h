#ifndef  EXPORT_EXAMPLE_H
#define  EXPORT_EXAMPLE_H

#include "iface.h"
#include <QObject>
#include <QString>

class example_export:public export_iface
{
    Q_OBJECT

    Q_INTERFACES(export_iface)

        public:


            QAction *exportAction();
    QString exportPluginDescription();
    QAction *exportSettings();



public slots:

    void export1();
    void settings();
};
#endif
