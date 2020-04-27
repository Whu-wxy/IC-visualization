#include "utils.h"

bool sortFileName(QString s1, QString s2)
{
    QStringList s1List = s1.split("_");
    QStringList s2List = s2.split("_");
    s1 = s1List.at(s1List.count()-1);
    if(s1.endsWith(".jpg"))
        s1.remove(".jpg");
    else if(s1.endsWith(".txt"))
        s1.remove(".txt");
    s2 = s2List.at(s2List.count()-1);
    if(s2.endsWith(".jpg"))
        s2.remove(".jpg");
    else if(s2.endsWith(".txt"))
        s2.remove(".txt");

    return s1.toInt() < s2.toInt();
}

void    countInterSection(QString GTDir, bool filtInvalid)
{
    QDir dir(GTDir);
    if(!dir.exists())
        return;
    QStringList GTNameList;
    GTNameList = dir.entryList(QDir::Files, QDir::Name);

    if(GTNameList.count() == 0)
        return;

    std::sort(GTNameList.begin(), GTNameList.end(), sortFileName);

    if(dir.isEmpty())
        return;

    int allCount = 0;
    int labCount = 0;
    for(int i=0; i<GTNameList.count(); i++)
    {
        QList<QPolygon> polyList;
        int count = 0;

        QFile file(dir.path()+QDir::separator()+GTNameList.at(i));
        //read labels
        if(file.open(QIODevice::ReadOnly))
        {
            while (!file.atEnd())
            {
                QByteArray line = file.readLine();
                line = line.simplified();
                if(!QChar(line.at(0)).isDigit())
                    line = line.remove(0, 3);

                if(filtInvalid)
                    if(line.endsWith("###"))
                        continue;

                QPolygon poly = readICLabel(QString(line));
                polyList.append(poly);
            }
            file.close();
        }

        // count intersection boxes
        for(int j=0; j<polyList.count(); j++)
        {
            QPolygon poly1 = polyList.at(j);
            for(int k=0; k<polyList.count(); k++)
            {
                if(k == j)
                    continue;
                QPolygon poly2 = polyList.at(k);
                if(poly1.intersects(poly2))
                {
                    count++;
                    break;
                }
            }
        }
        //
        //   qDebug()<<GTNameList.at(i)<<":"<<count<<"/"<<polyList.count();
        labCount = polyList.count() + labCount;
        allCount = allCount+count;
    }
    qDebug()<<"intersection boxes Count"<<":"<<allCount<<"/"<<labCount;
}

void    countVertival(QString GTDir, bool filtInvalid)
{
    QDir dir(GTDir);
    if(!dir.exists())
        return;
    QStringList GTNameList;
    GTNameList = dir.entryList(QDir::Files, QDir::Name);
    if(GTNameList.count() == 0)
        return;

    std::sort(GTNameList.begin(), GTNameList.end(), sortFileName);

    if(dir.isEmpty())
        return;

    int allCount = 0;
    int labCount = 0;
    for(int i=0; i<GTNameList.count(); i++)
    {
        QList<QPolygon> polyList;
        int count = 0;

        QFile file(dir.path()+QDir::separator()+GTNameList.at(i));
        //read labels
        if(file.open(QIODevice::ReadOnly))
        {
            while (!file.atEnd())
            {
                QByteArray line = file.readLine();
                line = line.simplified();
                if(!QChar(line.at(0)).isDigit())
                    line = line.remove(0, 3);

                if(filtInvalid)
                    if(line.endsWith("###"))
                        continue;

                QPolygon poly = readICLabel(QString(line));
                polyList.append(poly);
            }
            file.close();
        }

        // count vertical boxes
        for(int j=0; j<polyList.count(); j++)
        {
            QPolygon poly = polyList.at(j);
            QPoint leftTop = poly.at(0);
            QPoint rightTop = poly.at(1);
            QPoint leftBot = poly.at(3);

            float lengthH = sqrt(pow(leftTop.x()-rightTop.x(), 2)+pow(leftTop.y()-rightTop.y(), 2));
            float lengthV = sqrt(pow(leftTop.x()-leftBot.x(), 2)+pow(leftTop.y()-leftBot.y(), 2));
            if(lengthV > lengthH*2)
                count++;
        }
        //
        qDebug()<<GTNameList.at(i)<<":"<<count<<"/"<<polyList.count();
        labCount = polyList.count() + labCount;
        allCount = allCount+count;
    }
    qDebug()<<"vertical boxes Count"<<":"<<allCount<<"/"<<labCount;
}

QPolygon readICLabel(QString label)
{
    QStringList labelList;
    QPolygon polygon;
    QPoint pt;
    int count = 0;

    if(label.contains(","))  //IC15 | IC13-TEST
    {
        labelList = label.split(",");

        count = labelList.count();

        if(labelList.count() > 8)
            count -= 1;
        else if(count == 5)
            count -= 1;
    }
    else
    {
        labelList = label.split(" ");  //IC13
        count = labelList.count();
        if(labelList.count() > 4)
            count -= 1;
    }

    if(count == 8)              //IC15
    {
        for(int i=0; i<count; i++)
        {
            if(i % 2 == 0)
            {
                pt.setX(labelList.at(i).toInt());
            }
            else if(i % 2 == 1)
            {
                pt.setY(labelList.at(i).toInt());
                polygon.append(pt);
            }
        }
    }
    else if(count == 4)         //IC13
    {
        QPoint topLeft, bottomRight;
        topLeft.setX(labelList.at(0).toInt());
        topLeft.setY(labelList.at(1).toInt());
        bottomRight.setX(labelList.at(2).toInt());
        bottomRight.setY(labelList.at(3).toInt());
        QRect rect(topLeft,bottomRight);
        polygon = QPolygon(rect);
    }

    return  polygon;
}


QPolygon readIC17Label(QString label)
{
    QStringList labelList;
    QPolygon polygon;
    QPoint pt;
    int count = 0;

    labelList = label.split(",");
    count = labelList.count();
    if(count % 2 == 0)
        count -= 2;    // label 最后是语言类别+字符
    else
        count -= 1;    // pred 最后加置信度

    for(int i=0; i<count; i++)
    {
        if(i % 2 == 0)
        {
            pt.setX(labelList.at(i).toInt());
        }
        else if(i % 2 == 1)
        {
            pt.setY(labelList.at(i).toInt());
            polygon.append(pt);
        }
    }

    return  polygon;
}


QPolygon readCTWTXTLabel(QString label)
{
    QStringList labelList;
    QPolygon polygon;
    QPoint pt;
    int count = 0;

    if(!label.contains(","))
        return polygon;
    labelList = label.split(",");

    count = labelList.count();

    if(labelList.count() > 32)
        count -= 1;

    int minX = labelList.at(0).toInt();
    int minY = labelList.at(1).toInt();
    for(int i=4; i<count; i++)
    {
        if(i % 2 == 0)
        {
            pt.setX(minX+labelList.at(i).toInt());
        }
        else if(i % 2 == 1)
        {
            pt.setY(minY+labelList.at(i).toInt());
            polygon.append(pt);
        }
    }

    return  polygon;
}

QPolygon readCTWTXTE2ELabel(QString label)
{
    QStringList labelList;
    QPolygon polygon;
    QPoint pt;
    int count = 0;

    if(!label.contains(","))
        return polygon;
    labelList = label.split(",");

    count = labelList.count();
    qDebug()<<count;

    if(labelList.count() > 32)
        count -= 1;

    //    int minX = labelList.at(0).toInt();
    //    int minY = labelList.at(1).toInt();
    for(int i=0; i<count; i++)
    {
        if(i % 2 == 0)
        {
            pt.setX(labelList.at(i).toInt());
        }
        else if(i % 2 == 1)
        {
            pt.setY(labelList.at(i).toInt());
            polygon.append(pt);
        }
    }

    return  polygon;
}


QPolygon readTotalText2ICLabel(QString label)
{
    QStringList labelList;
    QPolygon polygon;
    QPoint pt;
    int count = 0;

    if(!label.contains(","))
        return polygon;
    labelList = label.split(",");

    count = labelList.count();

    for(int i=0; i<count; i++)
    {
        if(i % 2 == 0)
        {
            pt.setX(labelList.at(i).toInt());
        }
        else if(i % 2 == 1)
        {
            pt.setY(labelList.at(i).toInt());
            polygon.append(pt);
        }
    }

    return  polygon;
}

