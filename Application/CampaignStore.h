// CampaignStore.h
#pragma once
#include <QStringList>

namespace CampaignStore {
    QStringList load();
    void save(const QStringList &names);
}