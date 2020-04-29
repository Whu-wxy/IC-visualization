#include "maindlg.h"

#include <QApplication>
#include <QStyleFactory>
#include "utils.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QApplication::setStyle(QStyleFactory::create("simplestyle"));

    MainDlg w;
    w.show();

    //
    //countInterSection("F:\\zzxs\\Experiments\\dl-data\\TotalText\\test\\gt", false, TOTAL);
    //
    //countVertival("F:\\zzxs\\Experiments\\dl-data\\ICDAR\\ICDAR2015\\test\\gt", false);

    return a.exec();
}
