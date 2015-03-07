#include <QGLWidget>
#include <QButtonGroup>
#include <QGroupBox>
#include <QColorDialog>

#include "editorSettings.h"
#include "config.h"
#include "editor.h"

treeWidgetItem::treeWidgetItem() :
        QTreeWidgetItem()
{
    widget = NULL;
}

treeWidgetItem::treeWidgetItem(QTreeWidget * parent, int type = Type) :
        QTreeWidgetItem(parent, type)
{
    widget = NULL;
}

treeWidgetItem::treeWidgetItem(QTreeWidgetItem * parent, int type = Type) :
        QTreeWidgetItem(parent, type)
{
    widget = NULL;
}

treeWidget::treeWidget()
{
    connect(this, SIGNAL(itemClicked(QTreeWidgetItem *,int)),this, SLOT(emitWidgetChanged(QTreeWidgetItem *,int)));
}

void treeWidget::emitWidgetChanged(QTreeWidgetItem *w, int)
{
    treeWidgetItem *i = dynamic_cast<treeWidgetItem *> (w);
    emit widgetChanged(i->getWidget());
}

simulatorWidget::simulatorWidget()
{
    //-----------
    QHBoxLayout *l1 = new QHBoxLayout;
    mult = new QSpinBox;
    mult->setRange(1, 20);
    QLabel *lab1 = new QLabel("Multiplier:");
    l1->addWidget(lab1);
    l1->addWidget(mult);
    //-------------
    QHBoxLayout *l2 = new QHBoxLayout;
    ts = new QSpinBox;
    ts->setRange(10, 100);
    ts->setSingleStep(5);
    QLabel *lab2 = new QLabel("TimeStep:");
    l2->addWidget(lab2);
    l2->addWidget(ts);
    //---------------
    QVBoxLayout *vl = new QVBoxLayout;
    vl->addLayout(l1);
    vl->addLayout(l2);

    this->setLayout(vl);
}

int simulatorWidget::getTimestep()
{
    return ts->value();
}

int simulatorWidget::getMultiplier()
{
    return mult->value();
}

guisettings::guisettings(editor *e)
{
    ed=e;
    //############# viewport mode ###############
    QButtonGroup *bgr = new QButtonGroup;
    gl_radiobutton = new QRadioButton("OpenGL", this);
    paintengine_radiobutton = new QRadioButton("QPaintEngine", this);
    bgr->addButton(gl_radiobutton);
    bgr->addButton(paintengine_radiobutton);

    QHBoxLayout *l1 = new QHBoxLayout;
    l1->addWidget(gl_radiobutton);
    l1->addWidget(paintengine_radiobutton);

    QGroupBox *gb1 = new QGroupBox("Viewport Mode");
    gb1->setLayout(l1);


    QLabel *laa= new QLabel("Viewport Color");
    viewportColorButton=new QPushButton("Set color...");
    connect(viewportColorButton,SIGNAL(clicked()),this,SLOT(setViewportColor()));
    QHBoxLayout *lay=new QHBoxLayout;
    lay->addWidget(laa);
    lay->addWidget(viewportColorButton);


    //#################   GENERAL
    QVBoxLayout *lg = new QVBoxLayout;
    lg->addWidget(gb1);
    lg->addLayout(lay);

    setLayout(lg);

    //########## initial values
    QSettings settings;

    settings.beginGroup("editorSettings");
    QString vt=settings.value("viewportType",QString("PAINTENGINE")).toString();

    setViewportType(vt);

}

void guisettings::setViewportColor()
{

    qDebug() << "aki";


    QColor c=QColorDialog::getColor(ed->getCentralGraphicsView()->backgroundBrush().color(),this,"Select Colour");
    qDebug() << c;

    if (c.isValid())
    {
        ed->getCentralGraphicsView()->setBackgroundBrush(c);
    }


}


QString guisettings::getViewportType()
{
    if (gl_radiobutton->isChecked())
        return "GLVIEWPORT";
    else
        return "PAINTENGINE";
}

void guisettings::setViewportType(QString t)
{
    gl_radiobutton->setChecked(0);
    paintengine_radiobutton->setChecked(0);

    if (t == "GLVIEWPORT")
    {
        gl_radiobutton->setChecked(1);
        ed->getCentralGraphicsView()->setViewport(new QGLWidget(QGLFormat(
                        QGL::SampleBuffers)));//FOR OPENGL VIEW
    }
    else if (t == "PAINTENGINE")
    {
        paintengine_radiobutton->setChecked(1);
        ed->getCentralGraphicsView()->setViewport(0);
    }

}


editorSettings::editorSettings(QWidget *parent) :
        QDialog(parent)
{

    this->setFixedSize(600, 300);
    QWidget *blank = new QWidget;

    //setWindowFlags(Qt::WindowSystemMenuHint);
    setWindowFlags(Qt::Dialog);

    //######create option pages
    pagesWidget = new QStackedWidget;
    simWidget = new simulatorWidget;
    guiSett = new guisettings(dynamic_cast<editor *> (parentWidget()));
    pagesWidget->addWidget(blank);
    pagesWidget->addWidget(simWidget);
    pagesWidget->addWidget(guiSett);

    //#### create tree widgets

    tree = new treeWidget;
    tree->setMaximumWidth(200);
    QStringList la;
    la << "Options";
    tree->setHeaderLabels(la);
    connect(tree, SIGNAL(widgetChanged(QWidget *)),pagesWidget, SLOT(setCurrentWidget(QWidget *)));

    //###########   import/export options
    treeWidgetItem *simWidgetItem = new treeWidgetItem(tree);
    simWidgetItem->setText(0, tr("Import/Export Plugins Options"));
    simWidgetItem->setWidget(blank);
    tree->insertTopLevelItem(0, simWidgetItem);

    treeWidgetItem *simEngine = new treeWidgetItem(simWidgetItem);
    simEngine->setText(0, tr("Simulator Engine"));
    simEngine->setWidget(simWidget);

    //############ gui settings
    treeWidgetItem *guiOptWidget = new treeWidgetItem(tree);
    guiOptWidget->setText(0, tr("GUI Options"));
    guiOptWidget->setWidget(blank);
    tree->insertTopLevelItem(0, guiOptWidget);

    treeWidgetItem *guiOptWidget1 = new treeWidgetItem(guiOptWidget);
    guiOptWidget1->setText(0, tr("GUI Rendering Settings"));
    guiOptWidget1->setWidget(guiSett);

    //##########################


    QPushButton *applyButton = new QPushButton("OK");
    QPushButton *closeButton = new QPushButton("Cancel");
    QPushButton *expandcollapse = new QPushButton("Expand/Collapse");

    connect(closeButton, SIGNAL(clicked()), this, SLOT(close()));
    connect(applyButton, SIGNAL(clicked()), this, SLOT(applyChanges()));
    connect(expandcollapse, SIGNAL(clicked()), this, SLOT(expandcollapse()));

    QVBoxLayout *treeLayout = new QVBoxLayout;
    treeLayout->addWidget(tree);
    treeLayout->addWidget(expandcollapse);

    QHBoxLayout *formLayout = new QHBoxLayout;
    formLayout->addLayout(treeLayout);
    formLayout->addWidget(pagesWidget, 1);

    QHBoxLayout *buttonsLayout = new QHBoxLayout;
    buttonsLayout->addStretch(1);
    buttonsLayout->addWidget(applyButton);
    buttonsLayout->addWidget(closeButton);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addLayout(formLayout);
    mainLayout->addStretch(1);
    mainLayout->addSpacing(12);
    mainLayout->addLayout(buttonsLayout);

    setLayout(mainLayout);

    setWindowTitle(tr("Settings"));
    tree->expandAll();
}

editorSettings::~editorSettings(void)
{
    QSettings settings1;
    settings1.beginGroup("editorSettings");
    settings1.setValue("timestep", simWidget->getTimestep());
    settings1.setValue("viewportType",guiSett->getViewportType());
}

void editorSettings::applyChanges()
{
    qDebug() << "void editorSettings::applyChanges()";

    guiSett->setViewportType(guiSett->getViewportType());

    close();
}

void editorSettings::expandcollapse()
{
    static bool expanded=1;
    if (expanded)
    {
        tree->collapseAll();
        expanded=0;
    }
    else
    {
        tree->expandAll();
        expanded=1;
    }

}
