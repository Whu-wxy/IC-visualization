#include "imagewidget.h"
#include <QPainter>
#include <QDebug>

ImgWidget::ImgWidget(QWidget *parent,QString path)
    : QWidget(parent)
{
    basemap = NULL;

    m_bShowGT = true;
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
                if(m_bFiltGT)
                {
                    if(m_filtGTList.contains(i))
                        continue;
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

    QPainter basePainter(this);
    basePainter.drawPixmap(0,0,*basemap);
}

void ImgWidget::resizeEvent(QResizeEvent* e)
{
    QSize size = e->size();

    update();
}

