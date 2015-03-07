#ifndef OBJECT_H
#define OBJECT_H

#include <QtGui>
#include <QGraphicsItem>

class object: public QObject,public QGraphicsItem
{
    /// point id
    QString _id;

    quint32 _drawingState;
public:
    object();

    /// Getter - returns point id
    QString id();

    /// Setter - sets point id
    void setID(QString);

    /// Sets drawing state in case the point was imported from plugin
    void setDrawingState(quint32 d);

    /// Gets drawing state
    quint32 drawingState();


};

#endif // OBJECT_H
