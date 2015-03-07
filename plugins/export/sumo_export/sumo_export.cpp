#include "sumo_export.h"
#include <QtGui>
#include "sumowriter.h"


QString sumo_settings::findExecutable(QString ex)
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

void sumo_settings::setCustomExecDir()
{
    customExecDir=(QFileDialog::getExistingDirectory(0,"Select destination dir", "."));

    setExecutables();
}

void sumo_settings::setExecutables()
{

#ifdef Q_OS_WIN32
    netconvert->setText(findExecutable("netconvert.exe"));
    QString guis=findExecutable("sumo-gui.exe");
    if (guis=="")
       guis=findExecutable("guisim.exe");
    sumogui->setText(guis);
    
    sumogui->setText(findExecutable("guisim.exe"));
#else
    QString netc=findExecutable("sumo-netconvert");
    if (netc=="")
      netc=findExecutable("netconvert");
    netconvert->setText(netc);
    
    QString guis=findExecutable("sumo-guisim");
    if (guis=="")
       guis=findExecutable("sumo-gui");
    
    sumogui->setText(guis);
#endif

}

QString sumo_settings::netconvertExecutable()
{
    return netconvert->text();
}

QString sumo_settings::sumoguiExecutable()
{
    return sumogui->text();
}

sumo_settings::~sumo_settings()
{
    QSettings settings;
    settings.beginGroup("sumo_plugin_options");
    settings.setValue("guisim_path",sumogui->text());
    settings.setValue("netconvert_path",netconvert->text());
    settings.endGroup();
}

sumo_settings::sumo_settings():QDialog()
{
    QVBoxLayout *v1=new QVBoxLayout;
    QTabWidget *b1=new QTabWidget;
    v1->addWidget(b1);

    //##########################
    QWidget *sim_settings=new QWidget;
    runSumo=new QCheckBox("Run SUMO after export");
    runSumo->setChecked(1);
    QVBoxLayout *v2=new QVBoxLayout;
    v2->addWidget(runSumo);
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

    QHBoxLayout *h6=new QHBoxLayout;
    QLabel *l4=new QLabel("Enable TraCI control on port");
    enableTraci=new QCheckBox;
    _traciPort=new QLineEdit("1234");
    connect (enableTraci,SIGNAL(toggled(bool)),_traciPort,SLOT(setEnabled(bool)));
    h6->addWidget(l4);
    h6->addWidget(enableTraci);
    h6->addWidget(_traciPort);
    v2->addLayout(h6);

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
    QLabel *l1=new QLabel("sumo-netconvert");
    netconvert=new QLineEdit;
    h1->addWidget(l1);
    h1->addWidget(netconvert);
    exec_layout->addLayout(h1);



    QHBoxLayout *h5=new QHBoxLayout;
    QLabel *l3=new QLabel("sumo-guisim");
    sumogui=new QLineEdit;
    h5->addWidget(l3);
    h5->addWidget(sumogui);
    exec_layout->addLayout(h5);

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

    setWindowTitle("Sumo Settings");
    setLayout(v1);


    //########## restore settings
    QSettings settings;
    settings.beginGroup("sumo_plugin_options");
    sumogui->setText(settings.value("guisim_path","").toString());
    netconvert->setText(settings.value("netconvert_path","").toString());
    settings.endGroup();

    if (sumogui->text()=="" or netconvert->text()=="")
        setExecutables();
}

void sumo_settings::browseDir()
{
    dirPath->setText(QFileDialog::getExistingDirectory(0,"Select destination dir", "."));

}

sumo_export::sumo_export():export_iface()
{
    _settings=new sumo_settings;
}

sumo_export::    ~sumo_export()
{
    delete _settings;
}


QAction *sumo_export::exportAction()
{
    QAction *b=new QAction("Export to SUMO",this);
    b->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_E));
    connect (b,SIGNAL(triggered()),SLOT(export1()));
    connect (_settings->okButton(),SIGNAL(clicked()),this,SLOT(procede_export()));

    return b;
}


QAction *sumo_export::exportSettings()
{
    /*   QAction *b=new QAction("SUMO Settings",this);
    connect (b,SIGNAL(triggered()),SLOT(settings()));
    return b;*/
    return NULL;
}



QString sumo_export::exportPluginDescription()
{
    return "SUMO simulator Export Plugin\n\n This is the first plugin deployed for netEditor, it can export a network to this well known simulator, comments/bugfixes are wellcome";
}


void sumo_export::export1()
{
    _settings->exec();
}

void sumo_export::procede_export()
{
    qDebug() << "--------------void sumo_export::procede_export()";


    _settings->close();

    QString dir=_settings->exportDir();

    if (not QFileInfo(dir).isDir())
    {
        QMessageBox::critical(0,"Error","The selected destin is not a directory");
        return;
    }

    if (_settings->runAfterExport() && (! QFile::exists(_settings->netconvertExecutable())))
    {
        QMessageBox::critical(0,"Error","\"netconvert\" executable was not found, canceling export");
        return ;
    }

    {
        bool ok;

        int val=_settings->traciPort().toInt(&ok);
        if (_settings->traciEnabled() and (not ok) )
            if ((val<1) or (val>65535))
            {
            QMessageBox::critical(0,"Error","Check TraCI port");
            return ;
        }
    }


    sumowriter a(dir,this);

    QStringList args;
    args << "-e" << dir.toAscii() +"/editor.edg.xml" << "-n" << dir.toAscii() + "/editor.nod.xml" << "-x" << dir.toAscii() + "/editor.con.xml" <<  "-o" << dir.toAscii() +"/editor.net.xml" ;

    QProcess *netc=new QProcess;
    netc->start(_settings->netconvertExecutable(),args);

    if (netc->waitForFinished(10000)==false)
        QMessageBox::critical(0,"Error","Timeout waiting for \"netconvert\" to finish.");
    else
    {
        QString error=netc->readAllStandardError();
        bool okToRun=false;
        if (error!="")
            if (QMessageBox::question(0,"Error","There was an error converting the net files to sumo.\n\nMessage:\n" + error + "\nDo you want to continue?",QMessageBox::Yes | QMessageBox::No,QMessageBox::Yes)==QMessageBox::Yes)
                okToRun=true;
        if (error=="" or okToRun)
        {
            if (_settings->runAfterExport())
            {
                if (!QFile::exists(_settings->sumoguiExecutable()))
                {
                    QMessageBox::critical(0,"Error","The sumo-guisim executable was not found, canceling export");
                    return ;
                }
                QStringList sumo_args;
                sumo_args << "-c" << dir+QString("/editor.sumo.cfg");
               QProcess* sumogui=new QProcess;
                qDebug() << "Starting sumo GUI..." << _settings->sumoguiExecutable();
                sumogui->start(_settings->sumoguiExecutable(),sumo_args);

            }
            else
                QMessageBox::information(0,"Success","The net was successfully exported");
        }
    }
}


sumo_settings* sumo_export::settings()
{
    return _settings;
}


Q_EXPORT_PLUGIN2(export_iface,sumo_export);
