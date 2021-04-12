#ifndef JSONLOADER_H
#define JSONLOADER_H

#include <mainwindow.h>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>

class JsonLoader
{
public:
    JsonLoader(MainWindow *parent);
    JsonLoader(MainWindow *parent, const QString caption, const QString dir, const QString filter);

    void openFileWindow();
    bool readData();

    QString getFileName() const;
    void setFileName(const QString &value);

private:
    QString fileName;

    MainWindow *parent;
    QString caption;
    QString dir;
    QString filter;
};

#endif // JSONLOADER_H
