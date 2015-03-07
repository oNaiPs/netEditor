#ifndef FILTERPUSHBUTTON
#define  FILTERPUSHBUTTON

#include <QtGui>
#include <QPushButton>

class filterPushButton: public QPushButton
{
	QString filter;
	Q_OBJECT
public:

	filterPushButton(QString, QString);
	QString getFilter();

public slots:


void clickedd();
signals:

void clicked(QString);
};

#endif


