// MainWindow.h
#pragma once
#include <QMainWindow>

class QStackedWidget;
class MainMenuPage;
class CampaignPage;

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);

private:
    QStackedWidget *m_stack;
    MainMenuPage *m_menu;
    CampaignPage *m_campaign;
};