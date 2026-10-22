// MainWindow.cpp
#include "MainWindow.h"
#include "CampaignPage.h"
#include "MainMenuPage.h"

#include <QStackedWidget>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    setWindowTitle("DnD Campaign Manager");
    resize(1000, 650);

    m_stack = new QStackedWidget(this);
    m_menu = new MainMenuPage(this);
    m_campaign = new CampaignPage(this);
    m_stack->addWidget(m_menu);
    m_stack->addWidget(m_campaign);
    setCentralWidget(m_stack);

    connect(m_menu, &MainMenuPage::campaignOpened, this, [this](const QString &name) {
        m_campaign->setCampaign(name);
        m_stack->setCurrentWidget(m_campaign);
    });
    connect(m_campaign, &CampaignPage::backRequested, this, [this] {
        m_stack->setCurrentWidget(m_menu);
    });
}