// MainMenuPage.h
#pragma once
#include <QWidget>
#include "ui_QTMainMenu.h"

class MainMenuPage : public QWidget {
    Q_OBJECT
public:
    explicit MainMenuPage(QWidget *parent = nullptr);
    void refresh();

    signals:
        void campaignOpened(const QString &name);

private slots:
    void onNewCampaign();
    void onOpenCampaign();
    void onSelectionChanged();
    void showContextMenu(const QPoint &pos);

private:
    Ui::QTMainMenu ui;  
};
