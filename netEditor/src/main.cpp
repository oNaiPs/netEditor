#include <QApplication>
#include <signal.h>

#include "editor.h"
#include "config.h"

/*! \mainpage netEditor
 *
 * \section intro_sec Introduction
 *
 * This is the introduction.
 *
 * \section install_sec Installation
 *
 * \subsection step1 Step 1: Opening the box
 *
 * etc...
 */



/**
  * @file main.cpp
  * @brief Contains the main function, sets information about the domain name
  * for the config file
  * @author José Pereira <jlpereira@fe.up.pt>
  */

int main(int argc, char *argv[])
{
    std::cout << "HELLO!" << std::endl;
    QApplication app(argc, argv);

    QCoreApplication::setOrganizationName("FEUP");
    QCoreApplication::setOrganizationDomain("www.fe.up.pt");
    QCoreApplication::setApplicationName("editor");

    editor editor_instance;
    editor_instance.show();
    app.connect(&app, SIGNAL(lastWindowClosed()), &app, SLOT(quit()));
    return app.exec();
}
