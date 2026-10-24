#include <QApplication>
#include "src/MainWindow.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    QCoreApplication::setOrganizationName("DnDManager");
    QCoreApplication::setApplicationName("DnD Manager");

    MainWindow w;
    w.show();
    return app.exec();
}