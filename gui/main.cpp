#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QStringList paths = qApp->libraryPaths();
    paths.append(".");
    paths.append("./platforms");
    qApp->setLibraryPaths(paths);

    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
