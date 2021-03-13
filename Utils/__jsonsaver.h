#ifndef JSONSAVER_H
#define JSONSAVER_H

#include "mainwindow.h"
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>

class JsonSaver
{
public:
    JsonSaver(MainWindow *parent);
    JsonSaver(MainWindow *parent, const QString caption, const QString dir, const QString filter);

    QString openFileWindow();
    void writeData();
    void setParent(MainWindow *value);
    void setCaption(const QString &value);
    void setDir(const QString &value);
    void setFilter(const QString &value);

private:
    QString fileName;

    MainWindow *parent;
    QString caption;
    QString dir;
    QString filter;
};

#endif // JSONSAVER_H
