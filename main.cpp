#include "maindlg.h"

#include <QApplication>
#include <QStyleFactory>
#include "utils.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QApplication::setStyle(QStyleFactory::create("simplestyle"));

//    MainDlg w;
//    w.show();

    //F:\\zzxs\\Experiments\\dl-data\\CTW\\ctw1500\\test\\gt
    //F:\\zzxs\\Experiments\\dl-data\\TotalText\\train\\gt
    //F:\\zzxs\\Experiments\\dl-data\\ICDAR\\ICDAR2015\\train\\gt
    //
    countInterSection("F:\\zzxs\\Experiments\\dl-data\\ICDAR\\ICDAR2015\\test\\gt", false, IC15);
    //
    //countVertival("F:\\zzxs\\Experiments\\dl-data\\ICDAR\\ICDAR2015\\test\\gt", false);

    return a.exec();
}
