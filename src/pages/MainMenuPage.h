#pragma once
#include <QWidget>
#include "ui_CampaignSelect.h"

class MainMenuPage : public QWidget {
    Q_OBJECT
public:
    explicit MainMenuPage(QWidget *parent = nullptr);

    signals:
        void campaignOpened(const QString &name);

public slots:
    void onNewCampaign();
    void onOpenCampaign();
    void refresh();

private slots:
    void onSelectionChanged();
    void showContextMenu(const QPoint &pos);

private:
    Ui::CampaignSelect ui;
};