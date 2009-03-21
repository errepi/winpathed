#include "path_editor.h"

#include <QtGui>
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    PathEditor w;
    w.setWindowTitle("WinPathEd");
    w.show();
    return a.exec();
}
