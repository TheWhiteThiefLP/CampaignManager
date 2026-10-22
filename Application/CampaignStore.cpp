// CampaignStore.cpp
#include "CampaignStore.h"
#include <QDir>
#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QStandardPaths>

static QString filePath() {
    const QString dir = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    QDir().mkpath(dir);
    return dir + "/campaigns.json";
}

QStringList CampaignStore::load() {
    QFile f(filePath());
    if (!f.open(QIODevice::ReadOnly)) return {};
    QStringList names;
    for (const QJsonValue &v : QJsonDocument::fromJson(f.readAll()).array())
        names << v.toString();
    return names;
}

void CampaignStore::save(const QStringList &names) {
    QFile f(filePath());
    if (!f.open(QIODevice::WriteOnly)) return;
    f.write(QJsonDocument(QJsonArray::fromStringList(names)).toJson());
}