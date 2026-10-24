#include "Settings.h"
#include <QCoreApplication>
#include <QDir>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QStandardPaths>

static QString defaultCampaignPath() {
    return QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation)
           + "/CampaignManager";
}

Settings::Settings() {
    m_filePath = QCoreApplication::applicationDirPath() + "/settings.json";
    load();
}

Settings &Settings::instance() {
    static Settings s;
    return s;
}

QString Settings::campaignPath() const {
    return m_campaignPath;
}

void Settings::setCampaignPath(const QString &path) {
    m_campaignPath = path;
    save();
}

void Settings::load() {
    QFile f(m_filePath);
    if (!f.open(QIODevice::ReadOnly)) {
        // File doesn't exist yet — use defaults and save them
        m_campaignPath = defaultCampaignPath();
        save();
        return;
    }

    const QJsonObject obj = QJsonDocument::fromJson(f.readAll()).object();
    m_campaignPath = obj.value("campaignPath").toString(defaultCampaignPath());
}

void Settings::save() {
    QJsonObject obj;
    obj["campaignPath"] = m_campaignPath;

    QFile f(m_filePath);
    if (f.open(QIODevice::WriteOnly))
        f.write(QJsonDocument(obj).toJson(QJsonDocument::Indented));
}