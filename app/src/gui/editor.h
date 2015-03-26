#ifndef EDITOR_H
#define EDITOR_H

#include <QtWebKit>
#include <QMainWindow>
#include <QTextEdit>
#include <QActionGroup>

#include "graphicsScene.h"
#include "centralGraphicsView.h"
#include "previewGraphicsView.h"
#include "editorSettings.h"
#include "layersWidget.h"
#include "iface.h"

class editorSettings;
class graphicsScene;

/*!
*/

class editor : public QMainWindow
{
    Q_OBJECT

public:
    editor(QWidget *parent = 0);
    ~editor();

private:


    graphicsScene *scene;
    centralGraphicsView *view;
    previewGraphicsView *previewWidget;

    editorSettings *edSettings;

    void createActions();
    void createScene();
    void createView();
    void createDocks();
    void createToolBar();
    void createMenuBar();
    void createStatusBar();
    void createLayers();
    void restoreSettings();
    void loadPlugins();
    void createHelpMenu();


    QDockWidget *propertiesDock,*previewDock,*layersDock;

    QTextEdit *textEdit;

    QActionGroup *mode,*mode1;

    QMenuBar *bar;
    QMenu *file,*edit,*plugins_menu;
    QToolBar *toolbar;

    QLineEdit *commandSender;

    layersWidget *layers;


    //ACTIONS
    QAction *panMode, *selectMode, *microeditionMode, *lineMode,*polylineMode,  *exportToXml,
    *settings_qaction, *quit, *gmaps, *rectselect,  *selectAll,*createPoint, *view_plugins, *induction_loop;

    void closeEvent(QCloseEvent *event);

    QStringList pluginFiles;


    QVector<import_iface *> importPlugins;
    QVector<export_iface *> exportPlugins;
public:
    centralGraphicsView *getCentralGraphicsView(){return view;}
public slots:

    void openSettingsDialog();
    void sendCommandToView();
    void showPluginDialog();
    void showAboutDialog();
    void showPluginsSettings();
signals:

    void receivedCommand(QString);

};

#endif // EDITOR_H
