#pragma once
#include <QMainWindow>
#include "ui_MainWindow.h"

class CampaignSelectPage;
class CampaignPage;

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);

private:
    void openCampaign(const QString &name);
    void showMainMenu();
    void updateMenus(); 

    Ui::MainWindow ui;
    CampaignSelectPage *m_menu = nullptr;
    CampaignPage *m_campaign = nullptr;
};