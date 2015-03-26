#ifndef LANE_NG_H_
#define LANE_NG_H_

#include "point.h"
#include "manouvre.h"
#include <QtGui>
#include "induction_loop.h"

class segment;
class manouvre;
class point;
class induction_loop;

/// Class lane, represents a lane inside a segment, and manages manouvres and other objects
class lane
{
    /// lane id
    int _id;

    static long int globalID;

    /// pointer to the parent segment
    segment *_segment;//parent

    /// The lane's speed limit
    int speed_limit;
    /// The type of entities allowed on the lane
    int car_allowed;
    /****************************
         1 - A passenger car
         2 - A taxi
         3 - A bus
         4 - A truck
         5 - A motorcycle
         6 - A bicycle
         7 - A pedestrian
         *****************************/
    /// Brush for drawing
    QBrush _brush;

    /// Induction loop objects (testing - maybe it will be deprecated after a object-plugin interface)
    QList<induction_loop *> _induction_loops;

public:
    /// Constructor, takes the parent segment as input parameter
    lane(segment *);

    ~lane();

    /// Setter - sets speed limit on the lane
    void setSpeedLimit(int);

    /// Setter - sets the entitied allowed on the lane
    void setCarAllowed(int);

    /// Getter - returns the manouvres with this lane as origin
    QList<manouvre*> originManouvres;
    /// Getter - returns the manouvres with this lane as destin
    QList<manouvre*> destinManouvres;

    /// Adds a manouvre to the lane, it will discard it isn't the origin/destin
    void addManouvre(manouvre*);
    /// Deletes a manouvre from this lane
    void delManouvre(manouvre*);

    /// Updates the manouvres appearence and position
    void updateManouvres();

    /// Getter - returns the manouvres present on the lane
    QList<manouvre*> manouvres();

    /// returns all the lanes to which a vehicle can go from this lane
    QList<lane *> getForwardLaneConnections();

    /// returns all the lanes from a vehicle can come to this lane
    QList<lane *> getBackwardLaneConnections();

    /// Adds an induction loop to the lane
    void addInductionLoop(induction_loop *);
    /// Deletes an inductio loop from the lane
    void delInductionLoop(induction_loop *);
    /// Getter - returns the induction loops on the lane
    QList<induction_loop *>inductionLoops(){return _induction_loops;}

    /// Getter - returns the parent segment
    segment* parentSegment(){return _segment;}

    /// Returns true if the lane is a forward lane, according to it's parent segment
    bool isForwardLane(point *p=NULL);

    /// Sorts the manouvres to simplify the visualization on the scene
    void sortManouvres(QList <manouvre *> &);

    /// Getter - returns the lane id
    int id()
    {
        return _id;
    }

    /// Setter - sets the lane id
    void setID(int a)
    {
        _id = a;
    }
};

#endif /* LANE_NG_H_ */
