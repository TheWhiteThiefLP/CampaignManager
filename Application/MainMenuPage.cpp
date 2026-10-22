// MainMenuPage.cpp
#include "MainMenuPage.h"
#include "CampaignStore.h"

#include <QHBoxLayout>
#include <QInputDialog>
#include <QLabel>
#include <QListWidget>
#include <QMessageBox>
#include <QPushButton>
#include <QVBoxLayout>

MainMenuPage::MainMenuPage(QWidget *parent) : QWidget(parent) {
    auto *title = new QLabel("DnD Campaign Manager", this);
    title->setAlignment(Qt::AlignCenter);
    QFont f = title->font();
    f.setPointSize(24);
    f.setBold(true);
    title->setFont(f);

    m_list = new QListWidget(this);
    m_list->addItems(CampaignStore::load());

    auto *newBtn = new QPushButton("New Campaign", this);
    m_openBtn = new QPushButton("Open", this);
    m_deleteBtn = new QPushButton("Delete", this);

    auto *buttons = new QHBoxLayout;
    buttons->addWidget(newBtn);
    buttons->addStretch();
    buttons->addWidget(m_deleteBtn);
    buttons->addWidget(m_openBtn);

    auto *layout = new QVBoxLayout(this);
    layout->setContentsMargins(40, 30, 40, 30);
    layout->addWidget(title);
    layout->addWidget(new QLabel("Your campaigns:", this));
    layout->addWidget(m_list, 1);
    layout->addLayout(buttons);

    connect(newBtn, &QPushButton::clicked, this, &MainMenuPage::addCampaign);
    connect(m_openBtn, &QPushButton::clicked, this, &MainMenuPage::openSelected);
    connect(m_deleteBtn, &QPushButton::clicked, this, &MainMenuPage::deleteCampaign);
    connect(m_list, &QListWidget::itemDoubleClicked, this, &MainMenuPage::openSelected);
    connect(m_list, &QListWidget::itemSelectionChanged, this, &MainMenuPage::updateButtons);

    updateButtons();
}

void MainMenuPage::addCampaign() {
    bool ok = false;
    const QString name = QInputDialog::getText(this, "New Campaign", "Campaign name:",
                                               QLineEdit::Normal, {}, &ok).trimmed();
    if (!ok || name.isEmpty()) return;

    if (!m_list->findItems(name, Qt::MatchExactly).isEmpty()) {
        QMessageBox::warning(this, "Duplicate", "A campaign with that name already exists.");
        return;
    }
    m_list->addItem(name);
    persist();
}

void MainMenuPage::deleteCampaign() {
    auto *item = m_list->currentItem();
    if (!item) return;

    const auto answer = QMessageBox::question(
        this, "Delete Campaign", QString("Delete \"%1\"?").arg(item->text()));
    if (answer != QMessageBox::Yes) return;

    delete m_list->takeItem(m_list->row(item));
    persist();
}

void MainMenuPage::openSelected() {
    if (auto *item = m_list->currentItem())
        emit campaignOpened(item->text());
}

void MainMenuPage::updateButtons() {
    const bool has = m_list->currentItem() != nullptr;
    m_openBtn->setEnabled(has);
    m_deleteBtn->setEnabled(has);
}

void MainMenuPage::persist() {
    QStringList names;
    for (int i = 0; i < m_list->count(); ++i)
        names << m_list->item(i)->text();
    CampaignStore::save(names);
}