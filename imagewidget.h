#ifndef IMGWIDGET_H
#define IMGWIDGET_H

#include <QWidget>
#include <QImage>

#include <QPolygon>
#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QSize>
#include <QResizeEvent>
#include <QDir>

#include <QPixmap>

#include <iostream>
using namespace std;

class ImgWidget : public QWidget
{
    Q_OBJECT

public:
    ImgWidget(QWidget *parent=NULL,QString path="");
    ~ImgWidget();

    void    setImage(QString filePath);
    void    addPolygon(QPolygon, bool bGT);
    void    calInterPoly();
    void    showInterPoly(int);
    void    addFiltGTIndex(int index);
    bool    bGTLoad(){if(m_GTList.count()!=0) return true;
                        else return false; }
    bool    bPredLoad(){if(m_PredList.count()!=0) return true;
                        else return false; }

    void    saveResult(QString path, int index);

    bool    m_bShowGT;
    bool    m_bShowInter;
    bool    m_bFiltGT;
    bool    m_bShowPred;

private:
    QString  path;
    QPixmap* basemap;
    QImage   image;


    QList<QPolygon> m_GTList;
    QList<QPolygon> m_GTInterList;
    QList<int>      m_filtGTList;
    QList<QPolygon> m_PredList;

protected:
    void paintEvent(QPaintEvent *);
    void resizeEvent(QResizeEvent*);
};

#endif // IMGWIDGET_H
