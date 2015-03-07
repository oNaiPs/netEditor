#ifndef pg_import_H
#define pg_import_H

#include "iface.h"
#include <QObject>
#include <QString>

class pg_import:public import_iface
{
    Q_OBJECT
    Q_INTERFACES(import_iface)


        public:

            QAction *importAction();
    QString importPluginDescription();
    QAction *importSettings();

public slots:

    void import();

};
#endif
