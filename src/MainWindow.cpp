// MainWindow.cpp
#include "MainWindow.h"
#include "MainMenuPage.h"
#include "CampaignPage.h"
#include "SettingsDialog.h"
#include <QPushButton>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    ui.setupUi(this);

    // Settings button in top right of menu bar

    m_menu = new MainMenuPage(this);
    m_campaign = new CampaignPage(this);

    ui.pageStack->addWidget(m_menu);
    ui.pageStack->addWidget(m_campaign);
    ui.pageStack->setCurrentWidget(m_menu);

    connect(ui.actionPreferences, &QAction::triggered, this, [this] {
    SettingsDialog dlg(this);
    if (dlg.exec() == QDialog::Accepted)
        m_menu->refresh();
});

    connect(ui.actionNew_Campaign,  &QAction::triggered, m_menu, &MainMenuPage::onNewCampaign);
    connect(ui.actionOpen_Campaign, &QAction::triggered, m_menu, &MainMenuPage::onOpenCampaign);
    connect(m_menu,     &MainMenuPage::campaignOpened, this, &MainWindow::openCampaign);
    connect(m_campaign, &CampaignPage::backRequested,  this, &MainWindow::showMainMenu);
}

void MainWindow::openCampaign(const QString &name) {
    m_campaign->setCampaign(name);
    ui.pageStack->setCurrentWidget(m_campaign);
    setWindowTitle(name + " — DnD Campaign Manager");
}

void MainWindow::showMainMenu() {
    ui.pageStack->setCurrentWidget(m_menu);
    setWindowTitle("DnD Campaign Manager");
}