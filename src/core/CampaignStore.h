// CampaignStore.h
#pragma once
#include <QString>
#include <QStringList>

namespace CampaignStore {
    QString rootPath();          // e.g. Documents/DnD Manager
    QStringList campaigns();     // returns folder names
    bool create(const QString &name);
    bool rename(const QString &oldName, const QString &newName);
    bool remove(const QString &name);
}