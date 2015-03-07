#ifndef IMPORT_IFACE_H
#define IMPORT_IFACE_H

#include <QtGui>
#include "graphicsScene.h"
#include "point.h"

/// The class import_iface is an interface to import plugins, so one can add support for some external application without the need for compiling the whole project. \n\n It will be possible to add support for scripting in the future.
class import_iface:public QObject
{
    /// Pointer to the scene, to have access to the objects in it
    graphicsScene *scene;

public:
    graphicsScene *getScene(){return scene;}

    /// Setter - sets the scene
    void setScene(graphicsScene *s){scene=s;}

    /**
      * Routine to create a point to the scene given a location
      * \returns the point created, NULL if impossible to create
      * \n See graphicsScene::createPoint(QPointF) for more
      */
    point* createPoint(QPointF p){return scene->createPoint(p);}




    /** Routine to add a road to the scene (pointer)
  *
  */
    void addRoad(road *r){return scene->addRoad(r);}
    /** Routine to add a point to the scene (pointer)
  *
  */
    void addPoint(point *p){return scene->addPoint(p);}
    /**
      * Routine to add a segment to the scene
      * \returns the segment created, NULL if impossible
      * \n See graphicsScene::createPoint(QPointF) for more
      */
    segment *createSegment(point *start_p,point *final_p,QString _id=""){return scene->createSegment(start_p,final_p,_id);}

    /** Routine to add a segment to the scene (pointer)
      *
      */
    void addSegment(segment *p){return scene->addSegment(p);}


    /** Routine to add a manouvre to the scene (pointer)
          *
          */
    bool addManouvre(lane *l1,lane *l2){return scene->addManouvre(l1,l2);}


    /**
      * Search for point on scene
      * \param p Position on the scene
      */
    point *grepPoint(QPointF p){return scene->grepPoint(p);}

    /// destructor
    virtual ~import_iface(){}

    /**
      * To be implemented, returns an QAction to the import plugin, to be shown on the
      * toolbar and menus
      */
    virtual QAction *importAction() =0;
    /// Action to call the settings of the plugin
    virtual QAction *importSettings()=0;

    /// Some description to be shown on the plugin menu
    virtual QString importPluginDescription()=0;

};

/// The class export_iface is an interface to export plugins, so one can add support for some external application without the need for compiling the whole project. \n\n It will be possible to add support for scripting in the future.
class export_iface:public QObject
{
    /// Pointer to the scene, to have access to the objects in it
    graphicsScene *scene;

public:
    /// Setter - sets the scene
    void setScene(graphicsScene *s){scene=s;}
    virtual ~export_iface(){}

    /// Getter - gets all the lanes on the scene
    QList<lane *>getLanes(){return scene->lanes();}
    /// Getter - gets all the points on the scene
    QList<point *> getPoints(){return scene->points();}
    /// Getter - gets all the roads on the scene
    QList<road *> getRoads(){return scene->roads();}
    /// Getter - gets all segments on the scene
    QList<segment *>getSegments(){return scene->segments();}

    /**
      * To be implemented, returns an QAction to the import plugin, to be shown on the
      * toolbar and menus
      */
    virtual QAction *exportAction()=0;

    /// Action to call the settings of the plugin
    virtual QAction *exportSettings()=0;

    /// Some description to be shown on the plugin menu
    virtual QString exportPluginDescription()=0;
};

Q_DECLARE_INTERFACE(import_iface,"ImportSystem");
Q_DECLARE_INTERFACE(export_iface,"ExportSystem");

#endif // IMPORT_IFACE_H
