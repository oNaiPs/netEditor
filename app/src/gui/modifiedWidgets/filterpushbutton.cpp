/*
 * filterpushbutton.cpp
 *
 *  Created on: Nov 1, 2008
 *      Author: root
 */

#include "filterpushbutton.h"

filterPushButton::filterPushButton(QString q, QString s) :
	QPushButton(q)
{
	filter = s;
	connect(this, SIGNAL(clicked()), this, SLOT(clickedd()));
}

QString filterPushButton::getFilter()
{
	return filter;
}
void filterPushButton::clickedd()
{
	emit clicked(filter);
}

