#include <QMenu>
#include <QMenuBar>
#include <QMessageBox>
#include <QToolBar>
#include <QScrollBar>
#include <QLineEdit>
#include <QStatusBar>

#include "editor.h"
#include "config.h"
#include "plugindialog.h"

editor::editor(QWidget *parent) :
    QMainWindow(parent)
{
    createScene();

    createView();

    createDocks();

    createStatusBar();

    createLayers();

    createActions();

    lineMode->setChecked(1); //DEFAULT MODE
    view->startLineMode();

    createToolBar();

    createMenuBar();

    loadPlugins();

    createHelpMenu();

    restoreSettings();

    setWindowIcon(QIcon(":/images/icon.png"));

    edSettings=new editorSettings(this);

    setWindowTitle(QString("netEditor v")+QString(NETEDITOR_RELEASE));
}

editor::~editor()
{
    for (int i=0;i<exportPlugins.size();i++)
        delete exportPlugins[i];
    for (int i=0;i<importPlugins.size();i++)
        delete importPlugins[i];
}



void editor::createHelpMenu()
{
    QMenu *help=new QMenu("Help");
    help->addAction("About",this,SLOT(showAboutDialog()));
    bar->addMenu(help);
}

void editor::showAboutDialog()
{
    QString about;
    QTextStream t(&about);

    t << "<center><font size=7>netEditor v" << NETEDITOR_RELEASE << "</font><br>";
    t << "a universal network traffic modeler<br><br>";
    t << "Copyright &copy; 2008-2011 FEUP/LIACC<br>";
    t << "Faculdade de Engenharia do Porto<br>";
    t << "Complex Systems Analysis and Engineering - Interest Group<br>";
    t << "Artificial Intelligence and Computer Science Laboratory<br><br>";
    t << "<a href=\"http://wardrop.fe.up.pt\">wardrop.fe.up.pt</a></center>";

    QMessageBox::about(this,"About netEditor",about);
}

void editor::createDocks()
{
    propertiesDock=new QDockWidget("Properties",this);
    propertiesDock->setMinimumWidth(200);
    propertiesDock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);

    previewDock=new QDockWidget("Preview");
    previewDock->setWidget(previewWidget);
    previewWidget->setFixedHeight(170);

    view->setDock(propertiesDock);
    addDockWidget(Qt::LeftDockWidgetArea, propertiesDock);
    addDockWidget(Qt::LeftDockWidgetArea, previewDock);
}


void editor::openSettingsDialog()
{
    edSettings->exec();
}

void editor::createView()
{
    view=new centralGraphicsView(scene);

    this->setCentralWidget(view);

    previewWidget= new previewGraphicsView(scene,view);

    connect(view->verticalScrollBar(), SIGNAL(actionTriggered(int)), previewWidget, SLOT(previewChanged()));
    connect(view->horizontalScrollBar(), SIGNAL(actionTriggered(int)), previewWidget, SLOT(previewChanged()));
    connect(view, SIGNAL(viewportChanged()), previewWidget, SLOT(previewChanged()));
    connect(scene, SIGNAL(sceneRectChanged(QRectF)), previewWidget, SLOT(previewChanged()));
    // previewWidget->setRenderHints(QPainter::Antialiasing
    //                  | QPainter::HighQualityAntialiasing | QPainter::TextAntialiasing
    //                |QPainter::SmoothPixmapTransform);
}

void editor::createMenuBar()
{
    bar=new QMenuBar;
    setMenuBar(bar);
    file=new QMenu("File");
    file->addAction(view_plugins);
    file->addSeparator();
    file->addAction(quit);


    bar->addMenu(file);

    edit=new QMenu("Tools");
    edit->addAction(panMode);
    edit->addAction(lineMode);
    edit->addAction(selectMode);
    edit->addAction(rectselect);
    edit->addAction(polylineMode);
    edit->addAction(microeditionMode);

    edit->addSeparator();
    edit->addAction(settings_qaction);
    edit->addAction(selectAll);

    bar->addMenu(edit);
}

void editor::createActions()
{

    //ACTIONS
    microeditionMode=new QAction(QIcon(":/images/microedition.png"),"MicroEdition",this);
    microeditionMode->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_M));
    panMode=new QAction(QIcon(":/images/hand.png"),"Pan Tool",this);
    selectMode=new QAction(QIcon(":/images/pointer.png"),"Select Tool",this);
    selectMode->setShortcut(QKeySequence(Qt::Key_S));
    lineMode=new QAction(QIcon(":/images/line.png"),"Create Segment",this);
    lineMode->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_L));
    polylineMode=new QAction(QIcon(":/images/polyline.png"),"Polyline Tool",this);
    polylineMode->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_P));
    gmaps=new QAction("GMaps Overlay",this);
    settings_qaction=new QAction("Settings",this);
    settings_qaction->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_S));
    quit=new QAction("Quit",this);
    quit->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_Q));
    rectselect=new QAction(QIcon(":/images/rectselect.png"),"Selection (Rectangular)",this);
    selectAll=new QAction("Select All",this);
    selectAll->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_A));

    view_plugins=new QAction ("View Loaded Plugins",this);
    induction_loop=new QAction(QIcon(":/images/induction_loop.png"),"Add induction loop",this);



    microeditionMode->setCheckable(1);
    panMode->setCheckable(1);
    lineMode->setCheckable(1);
    polylineMode->setCheckable(1);
    selectMode->setCheckable(1);
    rectselect->setCheckable(1);
    gmaps->setCheckable(1);
    induction_loop->setCheckable(1);

    connect(microeditionMode, SIGNAL(triggered()), view, SLOT(startMicroEditionMode()));
    connect(panMode, SIGNAL(triggered()), view, SLOT(startPanMode()));
    connect(lineMode, SIGNAL(triggered()), view, SLOT(startLineMode()));
    connect(polylineMode, SIGNAL(triggered()), view, SLOT(startPolylineMode()));
    connect(selectMode, SIGNAL(triggered()), view, SLOT(startSelectMode()));
    connect(rectselect, SIGNAL(triggered()), view, SLOT(startRectSelectMode()));
    connect(gmaps, SIGNAL(toggled(bool)), view,SLOT(setGMapsMode(bool)));
    connect(settings_qaction, SIGNAL(triggered()), this, SLOT(openSettingsDialog()));
    connect(quit, SIGNAL(triggered()), this, SLOT(close()));
    connect(selectAll,SIGNAL(triggered()),view,SLOT(selectAll()));
    connect(view_plugins,SIGNAL(triggered()),this,SLOT(showPluginDialog()));
    connect(induction_loop,SIGNAL(triggered()),view,SLOT(startInductionLoopMode()));

    mode=new QActionGroup(this);
    mode->setExclusive(true);
    mode->addAction(microeditionMode);
    mode->addAction(panMode);
    mode->addAction(lineMode);
    mode->addAction(polylineMode);
    mode->addAction(selectMode);
    mode->addAction(rectselect);
    mode->addAction(induction_loop); //EXPERIMENTAL ONLY - not working for now
}


void editor::createScene()
{
    scene=new graphicsScene(this);
    scene->setItemIndexMethod(QGraphicsScene::NoIndex);
    scene->setSceneRect(QRectF(-DEFAULTSCENESIZE, -DEFAULTSCENESIZE, DEFAULTSCENESIZE*2, DEFAULTSCENESIZE*2));
}

void editor::createToolBar()
{
    toolbar=new QToolBar("Mode");
    toolbar->addAction(panMode);
    toolbar->addAction(lineMode);
    toolbar->addAction(polylineMode);
    toolbar->addAction(selectMode);
    toolbar->addAction(rectselect);
    toolbar->addAction(microeditionMode);
    toolbar->addSeparator();
    toolbar->addAction(induction_loop);
    toolbar->addSeparator();

    toolbar->addAction(gmaps);

    addToolBar(toolbar);
}

void editor::createStatusBar()
{
    setStatusBar(new QStatusBar);

    QLabel *label = new QLabel(this);
    label->setAlignment(Qt::AlignCenter);

    QLabel *label1 = new QLabel(this);
    label1->setAlignment(Qt::AlignCenter);

    QLabel *label2 = new QLabel(this);
    label1->setAlignment(Qt::AlignCenter);

    commandSender =new QLineEdit;

    statusBar()->insertPermanentWidget(0, commandSender, 1);
    statusBar()->insertPermanentWidget(1, label1, 1);
    statusBar()->insertPermanentWidget(2, label, 1);
    statusBar()->insertPermanentWidget(3, label2, 1);

    connect(view, SIGNAL(showOnLabel1(QString)), label, SLOT(setText(QString)));
    connect(view, SIGNAL(showOnLabel2(QString)), label1, SLOT(setText(QString)));
    connect(view, SIGNAL(showOnLabel3(QString)), label2, SLOT(setText(QString)));
    connect(commandSender, SIGNAL(returnPressed()), this, SLOT(sendCommandToView()));
    connect(this,SIGNAL(receivedCommand(QString)),view,SLOT(receivedCommand(QString)));
    connect (view,SIGNAL(focusReceived()),commandSender,SLOT(setFocus()));

    //set default zoom
    label2->setText("100%");
}

void editor::sendCommandToView()
{
    emit receivedCommand(commandSender->text());
    commandSender->clear();
}

void editor::createLayers()
{
    layers=new layersWidget;
    layersDock=new QDockWidget("Layers");
    addDockWidget(Qt::RightDockWidgetArea,layersDock);
    layersDock->setMaximumWidth(200);
    layersDock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);

    layersDock->setWidget(layers);

    connect (layers,SIGNAL(stateChanged(QString,quint32)),scene,SLOT(layersChanged(QString,quint32)));
    layers->initializeVariables();
    scene->setLayersWidget(layers);
}

void editor::restoreSettings()
{
    QSettings settings;
    settings.beginGroup("MainWindow");
    resize(settings.value("size", QSize(800, 600)).toSize());
    move(settings.value("pos", QPoint(200, 200)).toPoint());
    settings.endGroup();
}

void editor::closeEvent(QCloseEvent *)
{
    qDebug() << "void editor::closeEvent(QCloseEvent *)";

    QSettings settings;
    settings.beginGroup("MainWindow");
    settings.setValue("size", size());
    settings.setValue("pos", pos());
    settings.endGroup();
}




void editor::showPluginDialog()
{
    QDir dir=QDir::currentPath();
    dir.cd("plugins");
    PluginDialog *a=new PluginDialog(dir.absolutePath(),dir.entryList(QDir::Files),this);
    a->setWindowModality(Qt::WindowModal);
    a->show();
}

void editor::loadPlugins()
{
    plugins_menu=new QMenu("Plugins");
    bar->addMenu(plugins_menu);

    QDir dir(QCoreApplication::applicationDirPath());
    dir.cd("plugins");
    if (!dir.exists())
    {
        QMessageBox::information(0,"Error","Plugins folder not found! Please copy your plugins to plugins folder on the program dir");
        return;
    }


    QList<QAction *> plugins_settings;
    QList<QAction *> plugins_actions;

    foreach (QString s,dir.entryList(QDir::Files))
    {
        qDebug() << "Trying to open:" << dir.absoluteFilePath(s);
        QPluginLoader p(dir.absoluteFilePath(s));
        QObject *a=p.instance();
        if (a)
        {
            qDebug() << "\tLoaded plugin";

            import_iface *i=qobject_cast<import_iface *>(a);
            importPlugins.push_back(i);

            if (i)
            {
                qDebug() << "Found import content plugin";
                i->setScene(scene);
                plugins_actions.push_back(i->importAction());

                QAction *import_settings=i->importSettings();
                if (import_settings)
                    plugins_settings.push_back(import_settings);
            }

            export_iface *e=qobject_cast<export_iface *>(a);
            if (e)
            {

                exportPlugins.push_back(e);
                qDebug() << "Found export content plugin";
                e->setScene(scene);
                plugins_actions.push_front(e->exportAction());
                QAction *export_settings=e->exportSettings();
                if (export_settings)
                    plugins_settings.push_back(export_settings);
            }

            pluginFiles.push_back(dir.absoluteFilePath(s));
        }
        else
        {
            qDebug() << "\tCouldn't load plugin:" << p.errorString();
            //QMessageBox::warning(this,"Plugin not loaded",p.errorString());
        }
    }

    plugins_menu->addSeparator();


    plugins_menu->addActions(plugins_actions);
    plugins_menu->addSeparator();
    plugins_menu->addActions(plugins_settings);

    QAction *pluginSettingsAction=new QAction(tr("General Plugin Settings"),this);
    connect(pluginSettingsAction,SIGNAL(triggered()),this,SLOT(showPluginsSettings()));
    plugins_menu->addAction(pluginSettingsAction);


}

void editor::showPluginsSettings()
{

    qApp->quit();
    QProcess::startDetached(qApp->arguments()[0], qApp->arguments());

}
