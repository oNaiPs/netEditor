#include "feup_simulator_export.h"
#include "XMLNodeWriter.h"
#include "XMLRoadWriter.h"
#include "XMLTripAssignmentWriter.h"
#include "realRoad.h"
#include "segment.h"

void feup_simulator_export::procede_export()
{
    qDebug() << "FEUP_SIMULATOR_EXPORT_PLUGIN started";


    _settings->close();

    QString dir=_settings->exportDir();

    if (not QFileInfo(dir).isDir())
    {
        QMessageBox::critical(0,"Error","The selected destin is not a directory");
        return;
    }

    if (_settings->runAfterExport() and (not QFile::exists(_settings->feupSimulatorExecutable())))
    {
        QMessageBox::critical(0,"Error","The simulator executable was not found!");
        return ;
    }


    //#############EXPORTING
    nodeList=getPoints();


    foreach (point *n,nodeList)
    {

        qDebug() << "Node:" << n->id();
        QList<point *> tmpMasterRoad;

        if (n->segments().size()!=2)
        {
            foreach (segment *r,n->segments())
            {
                tmpMasterRoad.push_back(n);
                segment *road_it=r;
                point *node_it=n;

                while (road_it->otherPoint(node_it)->segments().size()==2)
                {
                    node_it=road_it->otherPoint(node_it);
                    road_it=node_it->getTheOtherSegment(road_it);
                    tmpMasterRoad.push_back(node_it);
                }
                tmpMasterRoad.push_back(road_it->otherPoint(node_it));
                addRoad(tmpMasterRoad);
                tmpMasterRoad.clear();
            }
        }
    }

    //CALL XMLWRITER


    //###################NODES################
    qDebug() << "Starting parsing nodes file";
    QFile nodesFile(dir + QDir::separator () + "Node.xml");

    if (!nodesFile.open(QFile::WriteOnly | QFile::Text))
    {
        qDebug() << "Couldn't open file for Writing! Returning";
        return;
    }

    XMLNodeWriter parser(nodeList,realRoads);
    parser.writeFile(&nodesFile);
    qDebug() << "Nodes file created!";
    //#################ROADS################
    qDebug() << "Starting parsing roads file";
    QFile roadsFile(dir + QDir::separator () + "Road.xml");

    if (!roadsFile.open(QFile::WriteOnly | QFile::Text))
    {
        qDebug() << "Couldn't open file for Writing! Returning";
        return;
    }

    XMLRoadWriter parser1(realRoads);
    parser1.writeFile(&roadsFile);
    qDebug() << "Roads file created!";
    //################################TRIP
    qDebug() << "Starting parsing trip assignementfile";
    QFile tripFile(dir + QDir::separator () + "TripAssignment.xml");



    if (!tripFile.open(QFile::WriteOnly | QFile::Text))
    {
        qDebug() << "Couldn't open file for Writing! Returning";
        return;
    }

    XMLTripAssignmentWriter parser2(nodeList);
    parser2.writeFile(&tripFile);
    //##########


    qDebug() << "Exported";
    //##################


    if (_settings->runAfterExport())
    {
        QProcess *netc=new QProcess;
        QFileInfo d(_settings->feupSimulatorExecutable());
        netc->setWorkingDirectory(d.absoluteDir().absolutePath());
        netc->start(_settings->feupSimulatorExecutable());
    }
}

feup_simulator_export::feup_simulator_export():export_iface()
{

    _settings=new feup_export_settings;
}
feup_simulator_export::~feup_simulator_export()
{
    delete _settings;

}

void feup_simulator_export::addRoad(QList<point *> nList) {

    foreach (realRoad *r,realRoads)
        if (*r==nList)return;

    realRoad *newRoad=new realRoad(nList);
    realRoads.push_back(newRoad);
    qDebug() << "Added Road ID =" << newRoad->id() << " -> from Node =" << newRoad->getFirstNode()->id() << " to Node =" << newRoad->getSecondNode()->id();
}

void feup_simulator_export::export1()
{
    QMessageBox::warning(0,"pre-alpha software","This is pre-alpha software plugin, expect a lot of bugs doing this!");

    _settings->show();
}

QAction *feup_simulator_export::exportAction()
{
    QAction *b=new QAction("Export to FEUP Simulator...",this);

    connect (b,SIGNAL(triggered()),SLOT(export1()));
    connect (_settings->okButton(),SIGNAL(clicked()),this,SLOT(procede_export()));
    return b;
}

QAction *feup_simulator_export::exportSettings()
{
    return NULL;
}

QString feup_simulator_export::exportPluginDescription()
{
    return "Paulo's Simulator export tool";
}

QString feup_export_settings::findExecutable(QString ex)
{
    QString path;
    foreach (QString s,QProcess::systemEnvironment())
    {
        if (s.left(5)=="PATH=")
            path=s.right(s.length()-5);
    }
    if (not customExecDir.isEmpty())
        path.push_front(customExecDir+QString(":"));

    foreach (QString p1,path.split(':'))
    {
        QDir d(p1);
        d.setFilter(QDir::Executable);
        if (d.exists(ex))
            return d.absoluteFilePath(ex);
    }
    return "";

}

void feup_export_settings::setCustomExecDir()
{
    customExecDir=(QFileDialog::getExistingDirectory(0,"Select destination dir", "."));

    setExecutables();
}

void feup_export_settings::setExecutables()
{

#ifdef Q_OS_WIN32
    simulator_exec->setText(findExecutable("Simulator.exe"));
#else
    simulator_exec->setText(findExecutable("Simulator"));
#endif

}

QString feup_export_settings::feupSimulatorExecutable()
{
    return simulator_exec->text();
}

feup_export_settings::~feup_export_settings()
{
    QSettings settings;
    settings.beginGroup("feup_plugin_options");
    settings.setValue("simulator_path",simulator_exec->text());
    settings.endGroup();
}

feup_export_settings::feup_export_settings():QDialog()
{
    QVBoxLayout *v1=new QVBoxLayout;
    QTabWidget *b1=new QTabWidget;
    v1->addWidget(b1);

    //##########################
    QWidget *sim_settings=new QWidget;
    runSimulator=new QCheckBox("Run Simulator after export");
    runSimulator->setChecked(1);
    QVBoxLayout *v2=new QVBoxLayout;
    v2->addWidget(runSimulator);
    sim_settings->setLayout(v2);
    b1->addTab(sim_settings,"Simulator Options");

    QHBoxLayout *h4=new QHBoxLayout;
    dirPath=new QLineEdit;
    dirPath->setText(QDir::currentPath());
    QPushButton *br=new QPushButton("...");
    h4->addWidget(new QLabel("Export dir"));
    h4->addWidget(dirPath);
    h4->addWidget(br);
    connect (br,SIGNAL(clicked()),this,SLOT(browseDir()));
    v2->addLayout(h4);

    //##########################
    QVBoxLayout *v=new QVBoxLayout;

    QGroupBox *executablesBox=new QGroupBox("Executables");
    v->addWidget(executablesBox);
    QVBoxLayout *exec_layout=new QVBoxLayout;
    executablesBox->setLayout(exec_layout);

    QPushButton *p1=new QPushButton("Set custom search path...");
    exec_layout->addWidget(p1);
    connect(p1,SIGNAL(clicked()),this,SLOT(setCustomExecDir()));

    QHBoxLayout *h1=new QHBoxLayout;
    QLabel *l1=new QLabel("simulator");
    simulator_exec=new QLineEdit;
    h1->addWidget(l1);
    h1->addWidget(simulator_exec);
    exec_layout->addLayout(h1);


    QHBoxLayout *h3=new QHBoxLayout;
    ok=new QPushButton("OK");
    QPushButton *cancel=new QPushButton("Cancel");
    connect (cancel,SIGNAL(clicked()),this,SLOT(close()));
    h3->addWidget(ok);
    h3->addWidget(cancel);
    v1->addLayout(h3);

    QWidget *w=new QWidget;
    w->setLayout(v);
    b1->addTab(w,"Executables");

    setWindowTitle("Simulator Settings");
    setLayout(v1);


    //########## restore settings
    QSettings settings;
    settings.beginGroup("feup_plugin_options");
    simulator_exec->setText(settings.value("simulator_path","").toString());
    settings.endGroup();

    if (simulator_exec->text()=="")
        setExecutables();
}

void feup_export_settings::browseDir()
{
    dirPath->setText(QFileDialog::getExistingDirectory(0,"Select destination dir", "."));
}



Q_EXPORT_PLUGIN2(export_iface,feup_simulator_export);

