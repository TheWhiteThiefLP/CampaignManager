#pragma once
#include <QString>

class Settings {
public:
    static Settings &instance();

    QString campaignPath() const;
    void setCampaignPath(const QString &path);

    void load();
    void save();

private:
    Settings();
    QString m_campaignPath;
    QString m_filePath;
};