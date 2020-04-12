#include "imagewidget.h"
#include <QPainter>
#include <QDebug>

ImgWidget::ImgWidget(QWidget *parent,QString path)
    : QWidget(parent)
{
    basemap = NULL;

    m_bShowGT = true;
    m_bShowInter = true;
    m_bFiltGT = false;
    m_bShowPred = true;

    if(path.length() == 0)
        return;

    image.load(path);

    if(image.isNull())
        return;

    basemap = new QPixmap(image.width(), image.height());
}

ImgWidget::~ImgWidget()
{

}

void ImgWidget::setImage(QString filePath)
{
    if(filePath.length() == 0)
        return;

    m_GTList.clear();
    m_GTInterList.clear();
    m_filtGTList.clear();
    m_PredList.clear();

    image.load(filePath);

    if(image.isNull())
        return;

    if(basemap != NULL)
    {
        delete basemap;
        basemap = NULL;
    }

    basemap = new QPixmap(image.width(), image.height());

    update();
}

void ImgWidget::addPolygon(QPolygon poly, bool bGT)
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

void ImgWidget::calInterPoly()
{
    m_GTInterList.clear();
    for(int i=0; i<m_GTList.count(); i++)
    {
        QPolygon poly1 = m_GTList.at(i);
        for(int j=i+1; j<m_GTList.count(); j++)
        {
            QPolygon poly2 = m_GTList.at(j);
            if(poly1.intersects(poly2))
                m_GTInterList.append(poly1.intersected(poly2));
        }
    }
}

void ImgWidget::showInterPoly(int state)
{
    if(state == 0)
    {
        m_bShowInter = false;
        update();
    }
    else if(state == 2)
    {
        m_bShowInter = true;
        update();
    }
}

void ImgWidget::addFiltGTIndex(int index)
{
    m_filtGTList.append(index);
}


void ImgWidget::saveResult(QString path, int index)
{
    if(basemap == NULL)
        return;

    path = path + QDir::separator() + "save_" + QString::number(index) + ".jpg";

    basemap->save(path, "jpg");
}

void ImgWidget::paintEvent(QPaintEvent *)
{
    if(basemap == NULL)
        return;
    basemap->fill(Qt::transparent);

    QPainter p(basemap);
    p.setRenderHint(QPainter::Antialiasing, true);

    if(!image.isNull())
    {
        p.drawImage(0, 0, image);

        QPen oldPen = p.pen();

        QPen newPen;
        newPen.setColor(QColor(Qt::green));
        newPen.setWidth(3);
        p.setPen(newPen);
        if(m_bShowGT)
        {
            for(int i=0; i<m_GTList.count(); i++)
            {
                if(m_bFiltGT)  //过滤###的文本区域
                {
                    if(m_filtGTList.contains(i))
                        continue;
                    p.drawPolygon(m_GTList.at(i));
                }
                else
                {
                    if(m_filtGTList.contains(i))
                    {
                        QPen filtPen;
                        filtPen.setColor(QColor(Qt::yellow));
                        filtPen.setWidth(3);
                        p.setPen(filtPen);
                        p.drawPolygon(m_GTList.at(i));
                        p.setPen(newPen);
                    }
                    else
                        p.drawPolygon(m_GTList.at(i));
                }

            }
        }
        //GT的交叉区域
        if(m_bShowInter)
        {
            QPen interPen;
            interPen.setColor(QColor(Qt::blue));
            interPen.setWidth(3);
            p.setPen(interPen);
            for(int i=0; i<m_GTInterList.count(); i++)
            {
                p.drawPolygon(m_GTInterList.at(i));
            }
            p.setPen(newPen);
        }

        newPen.setColor(QColor(Qt::red));
        p.setPen(newPen);

        //prediction区域
        if(m_bShowPred)
        {
            for(int i=0; i<m_PredList.count(); i++)
            {
                p.drawPolygon(m_PredList.at(i));
            }
        }
        p.setPen(oldPen);
    }

    QPainter basePainter(this);
    QPixmap basemapScale = basemap->scaled(this->width(), this->height(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
    basePainter.drawPixmap(0,0,basemapScale);
}

void ImgWidget::resizeEvent(QResizeEvent* e)
{
    QSize size = e->size();

    update();
}

