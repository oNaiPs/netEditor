#include "realRoad.h"

int realRoad::globalID=0;

realRoad::realRoad(QList <point *> nl) {
        _id=QString::number(globalID++);

	nodeList =nl;
}

bool realRoad::matchList(QList<point *> nl1) {
	bool ok=0;

        foreach(point *n1, nl1) {
		ok=encontraNaLinha(n1);
		if (ok==0)
			return 0;
	}
	return 1;
}

bool realRoad::encontraNaLinha(point *n) {
        foreach (point *n1,nodeList)
	if (n1==n)return 1;
	return 0;
}

