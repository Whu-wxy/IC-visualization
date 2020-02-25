#ifndef UTILS_H
#define UTILS_H

#include <QObject>
#include <QFile>
#include <QPolygon>
#include <QDir>
#include <QFileInfo>
#include <QDebug>

void    countInterSection(QString GTDir, bool filtInvalid);

QPolygon readICLabel(QString label);


#endif // UTILS_H
