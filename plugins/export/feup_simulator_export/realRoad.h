#ifndef REALsegment_H_
#define REALsegment_H_

#include "point.h"

class realRoad
{

    QList < point *> nodeList;
    static int globalID;
    QString _id;

public:
    realRoad()
    {
        _id=QString::number(globalID++);
    }

    realRoad(QList <point * >);


    point *getFirstNode(){if (nodeList.size()>0)return nodeList[0];return NULL;}
    point *getSecondNode(){if (nodeList.size()>0)return nodeList.last();return NULL;}

    segment *getRoadAdjacentToFirstNode()
    {
        if (nodeList.size()>1)
            return nodeList[0]->getCommonSegment(nodeList[1]);
        return NULL;
    }

    segment *getRoadAdjacentToSecondNode()
    {
        if (nodeList.size()>1)
            return nodeList[nodeList.size()-2]->getCommonSegment(nodeList[nodeList.size()-1]);
        return NULL;
    }

    point *getTheOtherNode(point *n)
    {
        if (n==getFirstNode())
            return getSecondNode();
        return getFirstNode();
    }

    bool contains(point *n1){foreach (point *n2,nodeList)if (n1==n2)return 1;return 0;}
    QString id(){return _id;}

    QList < point * > getNodeList(){return nodeList;}

    bool operator==(realRoad r){
        return matchList(r.getNodeList());
    }

    bool operator==(QList <point *>nl){
        return matchList(nl);
    }

    bool matchList(QList<point *>);

    bool encontraNaLinha(point *);


    void imprime()
    {
        foreach (point *n,nodeList)
            qDebug() << "node " << n->id();
    }

};

#endif /*REALsegment_H_*/

