#ifndef TIFFIMG_H
#define TIFFIMG_H

#include <QWidget>
#include <QGestureEvent>
#include <QPinchGesture>
#include <QImage>
#include <QWheelEvent>
#include <QPanGesture>
#include <QMouseEvent>

#include <QPolygon>
#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QSize>

class tiffImg : public QWidget
{
    Q_OBJECT

public:
    tiffImg(QWidget *parent=NULL,QString path="");
    ~tiffImg();

    void    setImage(QString filePath);
    void    addPolygon(QPolygon, bool bGT);
    bool    bGTLoad(){if(m_GTList.count()!=0) return true;
                        else return false; }
    bool    bPredLoad(){if(m_PredList.count()!=0) return true;
                        else return false; }

    bool    m_bShowGT;
    bool    m_bShowPred;

private:
    ///// image /////
    QString path;
    QImage image;

    ///// image position and scale /////
    float imgX;
    float imgY;

    QList<QPolygon> m_GTList;
    QList<QPolygon> m_PredList;

protected:
    void paintEvent(QPaintEvent *);

    void resizeEvent(QResizeEvent*);
};

#endif // TIFFIMG_H
