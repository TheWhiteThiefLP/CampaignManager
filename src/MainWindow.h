#pragma once
#include <QMainWindow>
#include "ui_MainWindow.h"

class MainMenuPage;
class CampaignPage;

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);

private:
    void openCampaign(const QString &name);
    void showMainMenu();

    Ui::MainWindow ui;
    MainMenuPage *m_menu = nullptr;
    CampaignPage *m_campaign = nullptr;
};