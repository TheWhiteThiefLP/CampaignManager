// CampaignStore.cpp
#include "CampaignStore.h"
#include <QDir>
#include <QStandardPaths>
#include "Settings.h"

QString CampaignStore::rootPath() {
    const QString path = Settings::instance().campaignPath();
    QDir().mkpath(path);
    return path;
}

QStringList CampaignStore::campaigns() {
    return QDir(rootPath()).entryList(QDir::Dirs | QDir::NoDotAndDotDot, QDir::Name);
}

bool CampaignStore::create(const QString &name) {
    return QDir(rootPath()).mkdir(name);
}

bool CampaignStore::rename(const QString &oldName, const QString &newName) {
    return QDir(rootPath()).rename(oldName, newName);
}

bool CampaignStore::remove(const QString &name) {
    return QDir(rootPath() + "/" + name).removeRecursively();
}