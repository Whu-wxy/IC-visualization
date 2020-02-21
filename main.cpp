#include "maindlg.h"

#include <QApplication>
#include <QStyleFactory>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QApplication::setStyle(QStyleFactory::create("simplestyle"));

    MainDlg w;
    w.show();
    return a.exec();
}
