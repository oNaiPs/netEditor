#ifndef CONFIG_H_
#define CONFIG_H_

//remove this
#define SUMO_012

/// Current modes of edition on the netEditor
enum EDITIONMODE {LINE=0,POLYLINE,SELECT,RECTSELECT,PAN,MICROEDITION,INDUCTION_LOOP};

/// Custom items on the QGraphicsScene
enum OBJECT_TYPE {POINT_TYPE=0,SEGMENT_TYPE,INDUCTION_LOOP_TYPE};

#define NETEDITOR_RELEASE "1.0a1svn"

#define DEFAULT_SCALE_FACTOR 1
#define X_TRANSFORM 1  * DEFAULT_SCALE_FACTOR //Everything is drawn with a factor of
#define Y_TRANSFORM -1 * DEFAULT_SCALE_FACTOR

#define DEFAULT_LANE_WIDTH 5
#define DEFAULT_POINT_DIAMETER 10

#define DEFAULTSCENESIZE 10000

#define RIGHT_CARS 1 //right side cars; not implemented yet

#define DEFAULT_VEHS_PER_HOUR 900
#define DEFAULT_VEHS_PERCENT 50

#define MINIMUM_ROAD_LENGTH 2
#define MINIMUM_LANE_WIDTH 1

#define GRIDSCALE 20
#define MOUSEZOOMFACTOR 10

//####### LAYERS - INITIAL STATE ########

#define DEFAULT_SEGMENT_DRAWING_STATE   50 //0x00101110b
#define DEFAULT_POINT_DRAWING_STATE     10 //0x00000110b
#define DEFAULT_COMPONENTS "Segments(ID,Reference,Street,Arrows,Shape) Points(ID,Circle)";

//object visibility
#define OBJECT_TOGGLE      1

//per-object type
#define SEGMENT_ID          2
#define SEGMENT_REFERENCE   3
#define SEGMENT_STREET      4
#define SEGMENT_ARROWS      5
#define SEGMENT_SHAPE       6

#define POINT_ID          2
#define POINT_CIRCLE      3

//#####################

#define GET_BIT(ref,bit) ((ref >> (bit)) & 1)
#define SET_BIT(ref,bit) (ref |= 1 << (bit))
#define UNSET_BIT(ref,bit) (ref &=  ~(1 << (bit)))

#endif /*CONFIG_H_*/
