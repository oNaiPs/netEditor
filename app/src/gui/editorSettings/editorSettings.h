#ifndef EDITORSETTINGS_H_
#define EDITORSETTINGS_H_

#include <QRadioButton>
#include <QStackedWidget>
#include <QDialog>

#include "editor.h"

class editor;

class treeWidget: public QTreeWidget
{
Q_OBJECT

public:

	treeWidget();

public slots:

	void emitWidgetChanged(QTreeWidgetItem *w, int);

	signals:
	void widgetChanged(QWidget *);

};

class treeWidgetItem: public QTreeWidgetItem
{
	QWidget *widget;
public:
	treeWidgetItem();
	treeWidgetItem(QTreeWidget *, int);
	treeWidgetItem(QTreeWidgetItem *, int);
	void setWidget(QWidget *w)
	{
		widget = w;
	}
	QWidget *getWidget()
	{
		return widget;
	}
};

class simulatorWidget: public QWidget
{
	QSpinBox *mult, *ts;

public:
	simulatorWidget();

	int getMultiplier();
	int getTimestep();

};

class guisettings: public QWidget
{
    Q_OBJECT

	QRadioButton *gl_radiobutton, *paintengine_radiobutton, *button4, *button5;
QPushButton *viewportColorButton;
	editor *ed;
public:
	guisettings(editor *);

	QString getViewportType();
	void setViewportType(QString);
private slots:
        void setViewportColor();

};

class editorSettings: public QDialog
{
Q_OBJECT

public:
	editorSettings(QWidget *parent = 0);
	~editorSettings(void);

private:
	void setInitialValues();
	treeWidget *tree;
	simulatorWidget *simWidget;
	guisettings *guiSett;
	QStackedWidget *pagesWidget;
public slots:

	void applyChanges();
	void expandcollapse();

};

#endif /*EDITORSETTINGS_H_*/
