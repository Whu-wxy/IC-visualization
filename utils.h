#ifndef UTILS_H
#define UTILS_H

#include <QObject>
#include <QFile>
#include <QPolygon>
#include <QDir>
#include <QFileInfo>
#include <QDebug>
#include <math.h>

void    countInterSection(QString GTDir, bool filtInvalid);   //统计有交集框的数量

void    countVertival(QString GTDir, bool filtInvalid);   //统计竖直文本数量

QPolygon readICLabel(QString label);   //读取ICDAR13/15标签

QPolygon readCTWTXTLabel(QString label);  //CTW-Recognition Annotation-14个点


#endif // UTILS_H
