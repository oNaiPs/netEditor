#ifndef COMMON_H
#define COMMON_H

#include <QtGui>

/// Swap one pointer by another
template <class T>  void swapPointers(T* a,T* b);

/**
  * The function receives a QLineF and returns his perpendicular vector
  * \param ref Reference line
  * \returns The perpendicular unit vector
  */
QPointF perp_norm(QLineF ref);

/**
  * Calculates the minimum distance from the QPointF to the QLineF
  * \returns The closest QPointF within the line to the other QPointF
  */
QPointF DistancePointLine(QPointF p, QLineF l);



#endif // COMMON_H
