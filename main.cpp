#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // Set application metadata
    a.setApplicationName("Board Game Collection");
    a.setApplicationVersion("2.0");
    a.setOrganizationName("GameDev Team");

    // Create and show main window
    MainWindow w;
    w.show();

    return a.exec();
}
