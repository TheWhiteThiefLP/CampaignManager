// MainWindow.cpp
#include "MainWindow.h"
#include "CampaignSelectPage.h"
#include "CampaignPage.h"
#include "SettingsDialog.h"
#include <QPushButton>
#include <QFileDialog>
#include <QDir>
#include "CampaignStore.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    ui.setupUi(this);

    m_menu = new CampaignSelectPage(this);
    m_campaign = new CampaignPage(this);

    ui.pageStack->addWidget(m_menu);
    ui.pageStack->addWidget(m_campaign);
    ui.pageStack->setCurrentWidget(m_menu);

    // hide Close Campaign initially
    ui.actionClose_Campaign->setVisible(false);

    connect(ui.actionNew_Campaign,   &QAction::triggered, m_menu,     &CampaignSelectPage::onNewCampaign);
    connect(ui.actionClose_Campaign, &QAction::triggered, this,       &MainWindow::showMainMenu);
    connect(ui.actionPreferences,    &QAction::triggered, this,       [this] {
        SettingsDialog dlg(this);
        if (dlg.exec() == QDialog::Accepted)
            m_menu->refresh();
    });

    // MainWindow.cpp
    connect(ui.actionOpen_Campaign, &QAction::triggered, this, [this] {
        const QString path = QFileDialog::getExistingDirectory(
            this,
            "Select Campaign Folder",
            CampaignStore::rootPath());

        if (path.isEmpty()) return;

        const QString name = QDir(path).dirName();
        openCampaign(name);
    });

    connect(m_menu,     &CampaignSelectPage::campaignOpened, this, &MainWindow::openCampaign);
}

void MainWindow::openCampaign(const QString &name) {
    m_campaign->setCampaign(name);
    ui.pageStack->setCurrentWidget(m_campaign);
    setWindowTitle(name + " — DnD Campaign Manager");
    ui.actionClose_Campaign->setVisible(true);
}

void MainWindow::showMainMenu() {
    ui.pageStack->setCurrentWidget(m_menu);
    setWindowTitle("DnD Campaign Manager");
    ui.actionClose_Campaign->setVisible(false);
}