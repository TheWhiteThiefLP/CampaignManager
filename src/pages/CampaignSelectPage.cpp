// CampaignSelectPage.cpp
#include "CampaignSelectPage.h"
#include "CampaignStore.h"
#include "SettingsDialog.h"
#include <QInputDialog>
#include <QMenu>
#include <QMessageBox>
#include <QPushButton>
#include <QListWidget>



CampaignSelectPage::CampaignSelectPage(QWidget *parent) : QWidget(parent) {
    ui.setupUi(this);
    ui.campaignList->setContextMenuPolicy(Qt::CustomContextMenu);

    connect(ui.newCampaignBtn, &QPushButton::clicked, this, &CampaignSelectPage::onNewCampaign);
    connect(ui.openBtn, &QPushButton::clicked, this, &CampaignSelectPage::onOpenCampaign);
    connect(ui.campaignList, &QListWidget::itemDoubleClicked, this, &CampaignSelectPage::onOpenCampaign);
    connect(ui.campaignList, &QListWidget::itemSelectionChanged, this, &CampaignSelectPage::onSelectionChanged);
    connect(ui.campaignList, &QListWidget::customContextMenuRequested, this, &CampaignSelectPage::showContextMenu);

    refresh();
}

void CampaignSelectPage::refresh() {
    ui.campaignList->clear();
    ui.campaignList->addItems(CampaignStore::campaigns());
    onSelectionChanged();
}

void CampaignSelectPage::onNewCampaign() {
    bool ok = false;
    const QString name = QInputDialog::getText(this, "New Campaign", "Campaign name:",
                                               QLineEdit::Normal, {}, &ok).trimmed();
    if (!ok || name.isEmpty()) return;

    if (!CampaignStore::create(name)) {
        QMessageBox::warning(this, "Error", "Could not create campaign. Check the name has no special characters.");
        return;
    }
    refresh();

    // Select the newly created item
    const auto items = ui.campaignList->findItems(name, Qt::MatchExactly);
    if (!items.isEmpty()) ui.campaignList->setCurrentItem(items.first());
}

void CampaignSelectPage::onOpenCampaign() {
    if (auto *item = ui.campaignList->currentItem())
        emit campaignOpened(item->text());
}

void CampaignSelectPage::onSelectionChanged() {
    ui.openBtn->setEnabled(ui.campaignList->currentItem() != nullptr);
}

void CampaignSelectPage::showContextMenu(const QPoint &pos) {
    auto *item = ui.campaignList->itemAt(pos);
    if (!item) return;
    ui.campaignList->setCurrentItem(item);

    QMenu menu(this);
    menu.addAction("Open", this, &CampaignSelectPage::onOpenCampaign);
    menu.addAction("Rename", this, [this, item] {
        bool ok = false;
        const QString newName = QInputDialog::getText(this, "Rename", "New name:",
                                                      QLineEdit::Normal, item->text(), &ok).trimmed();
        if (!ok || newName.isEmpty() || newName == item->text()) return;
        if (!CampaignStore::rename(item->text(), newName)) {
            QMessageBox::warning(this, "Error", "Could not rename campaign.");
            return;
        }
        refresh();
    });
    menu.addSeparator();
    menu.addAction("Delete", this, [this, item] {
        const auto answer = QMessageBox::question(
            this, "Delete", QString("Delete \"%1\"? This cannot be undone.").arg(item->text()));
        if (answer != QMessageBox::Yes) return;
        CampaignStore::remove(item->text());
        refresh();
    });
    menu.exec(ui.campaignList->viewport()->mapToGlobal(pos));
}