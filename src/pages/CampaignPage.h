// CampaignPage.h
#pragma once
#include <QWidget>

class QLabel;

class CampaignPage : public QWidget {
    Q_OBJECT
public:
    explicit CampaignPage(QWidget *parent = nullptr);
    void setCampaign(const QString &name);

    signals:
        void backRequested();

private:
    QLabel *m_title;
};