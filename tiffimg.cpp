#include "tiffimg.h"
#include <QPainter>
#include <QImage>
#include <QDebug>

tiffImg::tiffImg(QWidget *parent,QString path)
    : QWidget(parent)
{
    if(path.length() == 0)
        return;
    image.load(path);

    if(image.isNull())
        return;
    imgX=width()/2-image.width()/2;
    imgY=height()/2-image.height()/2;

    m_bShowGT = true;
    m_bShowPred = true;
}

tiffImg::~tiffImg()
{

}

void tiffImg::setImage(QString filePath)
{
    if(filePath.length() == 0)
        return;

    m_GTList.clear();
    m_PredList.clear();

    image.load(filePath);

    if(image.isNull())
        return;
    imgX=width()/2-image.width()/2;
    imgY=height()/2-image.height()/2;

    update();
}

void tiffImg::addPolygon(QPolygon poly, bool bGT)
{
    if(bGT)
    {
        m_GTList.append(poly);
    }
    else
    {
        m_PredList.append(poly);
    }
}

void tiffImg::paintEvent(QPaintEvent *)
{
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing);

    if(!image.isNull())
    {
        p.drawImage(imgX, imgY, image);

        QPen oldPen = p.pen();

        p.translate(imgX, imgY);

        QPen newPen;
        newPen.setColor(QColor(Qt::green));
        p.setPen(newPen);
        if(m_bShowGT)
        {
            for(int i=0; i<m_GTList.count(); i++)
            {
                p.drawPolygon(m_GTList.at(i));
            }
        }

        newPen.setColor(QColor(Qt::red));
        p.setPen(newPen);
        if(m_bShowPred)
        {
            for(int i=0; i<m_PredList.count(); i++)
            {
                p.drawPolygon(m_PredList.at(i));
            }
        }
        p.setPen(oldPen);
    }
}

void tiffImg::resizeEvent(QResizeEvent* e)
{
    QSize size = e->size();
    imgX=width()/2-image.width()/2;
    imgY=height()/2-image.height()/2;
    update();
}

