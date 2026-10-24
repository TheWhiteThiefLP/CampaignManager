#include "CampaignPage.h"
#include "CampaignStore.h"
#include "TabManager.h"
#include <QComboBox>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QInputDialog>
#include <QLabel>
#include <QMenu>
#include <QMessageBox>
#include <QStyle>
#include <QTreeWidget>
#include <QVBoxLayout>
#include "NPCEditor.h"

CampaignPage::CampaignPage(QWidget *parent) : QWidget(parent) {
    ui.setupUi(this);

    ui.contentTree->setHeaderHidden(true);
    ui.contentTree->setDragEnabled(true);
    ui.contentTree->setAcceptDrops(true);
    ui.contentTree->setDropIndicatorShown(true);
    ui.contentTree->setDragDropMode(QAbstractItemView::InternalMove);
    ui.contentTree->setContextMenuPolicy(Qt::CustomContextMenu);
    ui.contentTree->setIndentation(16);

    m_tabs = new TabManager(this);
    auto *tabLayout = new QVBoxLayout(ui.tabContainer);
    tabLayout->setContentsMargins(0, 0, 0, 0);
    tabLayout->addWidget(m_tabs);

    ui.filterCombo->addItems({"All", "NPCs", "Items", "Locations", "Maps"});

    const QString btnStyle = "QPushButton { border: 1px solid #555; border-radius: 4px; "
                             "padding: 2px; font-size: 14px; }"
                             "QPushButton:hover { background: #3a3a3a; }";
    ui.btnNewEntry->setStyleSheet(btnStyle);
    ui.btnDelete->setStyleSheet(btnStyle);

    connect(ui.filterCombo, &QComboBox::currentIndexChanged, this, &CampaignPage::onFilterChanged);
    connect(ui.contentTree, &QTreeWidget::customContextMenuRequested, this, &CampaignPage::showContextMenu);
    connect(ui.contentTree, &QTreeWidget::itemDoubleClicked, this, &CampaignPage::onItemDoubleClicked);
    connect(m_tabs, &TabManager::tabRequested, this, &CampaignPage::reopenTab);

    connect(ui.btnNewEntry, &QPushButton::clicked, this, [this] {
        QMenu menu(this);
        menu.addAction("👤 NPC",        this, [this] { createEntry("NPC");      });
        menu.addAction("⚔ Item",        this, [this] { createEntry("Item");     });
        menu.addAction("📍 Location",   this, [this] { createEntry("Location"); });
        menu.addAction("🗺 Map",        this, [this] { createEntry("Map");      });
        menu.addSeparator();
        menu.addAction("📁 New Folder", this, [this] {
            bool ok = false;
            const QString name = QInputDialog::getText(this, "New Folder", "Folder name:",
                QLineEdit::Normal, {}, &ok).trimmed();
            if (!ok || name.isEmpty()) return;
            auto *current = ui.contentTree->currentItem();
            QTreeWidgetItem *parent = nullptr;
            if (current && current->data(0, Qt::UserRole).toString() == "folder")
                parent = current;
            addFolder(name, parent);
        });
        menu.exec(ui.btnNewEntry->mapToGlobal(ui.btnNewEntry->rect().bottomLeft()));
    });

    connect(ui.btnDelete, &QPushButton::clicked, this, [this] {
        auto *item = ui.contentTree->currentItem();
        if (!item) return;
        const auto answer = QMessageBox::question(this, "Delete",
            QString("Delete \"%1\"?").arg(item->text(0)));
        if (answer == QMessageBox::Yes)
            delete item;
    });

    refreshTree();
}

void CampaignPage::setCampaign(const QString &name) {
    if (!m_campaignPath.isEmpty())
        m_tabs->saveTabs(m_campaignPath);

    ui.campaignTitle->setText(name);
    m_campaignPath = CampaignStore::rootPath() + "/" + name;
    refreshTree();
    m_tabs->restoreTabs(m_campaignPath);
}

void CampaignPage::onFilterChanged(int) {
    refreshTree();
}

void CampaignPage::onItemDoubleClicked(QTreeWidgetItem *item, int) {
    if (!item) return;
    const QString type     = item->data(0, Qt::UserRole).toString();
    const QString filePath = item->data(0, Qt::UserRole + 2).toString();
    if (type == "folder" || filePath.isEmpty()) return;

    const QString title = item->text(0).mid(item->text(0).indexOf(' ') + 1);

    QWidget *editor = nullptr;
    if (type == "NPC")
        editor = new NPCEditor(filePath);
    else {
        auto *placeholder = new QLabel("Editor coming soon: " + filePath);
        placeholder->setAlignment(Qt::AlignCenter);
        editor = placeholder;
    }

    m_tabs->openFile(filePath, title, editor);
}

void CampaignPage::reopenTab(const QString &filePath) {
    if (filePath.isEmpty()) return;
    auto *placeholder = new QLabel("Restored: " + filePath);
    placeholder->setAlignment(Qt::AlignCenter);
    const QString title = QFileInfo(filePath).baseName();
    m_tabs->openFile(filePath, title, placeholder);
}

void CampaignPage::createEntry(const QString &type) {
    bool ok = false;
    const QString name = QInputDialog::getText(this, "New " + type, "Name:",
        QLineEdit::Normal, {}, &ok).trimmed();
    if (!ok || name.isEmpty()) return;

    auto *current = ui.contentTree->currentItem();
    QTreeWidgetItem *parent = nullptr;
    QString parentPath = m_campaignPath;

    if (current) {
        if (current->data(0, Qt::UserRole).toString() == "folder") {
            parent = current;
            parentPath = current->data(0, Qt::UserRole + 1).toString();
        } else if (current->parent()) {
            parent = current->parent();
            parentPath = current->parent()->data(0, Qt::UserRole + 1).toString();
        }
    }

    const QMap<QString, QString> prefixes = {
        {"NPC", "npc_"}, {"Item", "item_"},
        {"Location", "loc_"}, {"Map", "map_"}
    };
    const QString fileName = prefixes.value(type, "npc_") + name + ".md";
    const QString fullPath = parentPath + "/" + fileName;

    QFile file(fullPath);
    if (file.open(QIODevice::WriteOnly)) {
        file.write(QString("---\ntype: %1\nname: %2\ntags: []\n---\n\n")
                       .arg(type, name).toUtf8());
        file.close();
    }

    addEntry(name, type, parent, fullPath);
}

QTreeWidgetItem* CampaignPage::addFolder(const QString &name, QTreeWidgetItem *parent) {
    const QString parentPath = parent
        ? parent->data(0, Qt::UserRole + 1).toString()
        : m_campaignPath;
    const QString fullPath = parentPath + "/" + name;
    QDir().mkpath(fullPath);

    auto *folder = parent
        ? new QTreeWidgetItem(parent)
        : new QTreeWidgetItem(ui.contentTree);
    folder->setText(0, "📁 " + name);
    folder->setData(0, Qt::UserRole,     "folder");
    folder->setData(0, Qt::UserRole + 1, fullPath);
    folder->setExpanded(true);
    folder->setFlags(folder->flags() | Qt::ItemIsDropEnabled | Qt::ItemIsDragEnabled);
    return folder;
}

QTreeWidgetItem* CampaignPage::addEntry(const QString &name, const QString &type,
                                         QTreeWidgetItem *parent, const QString &filePath) {
    const QMap<QString, QString> icons = {
        {"NPC",      "👤"},
        {"Item",     "⚔"},
        {"Location", "📍"},
        {"Map",      "🗺"},
    };
    auto *item = parent
        ? new QTreeWidgetItem(parent)
        : new QTreeWidgetItem(ui.contentTree);
    item->setText(0, icons.value(type, "•") + " " + name);
    item->setData(0, Qt::UserRole,     type);
    item->setData(0, Qt::UserRole + 2, filePath);
    item->setFlags(item->flags() | Qt::ItemIsDragEnabled);
    item->setFlags(item->flags() & ~Qt::ItemIsDropEnabled);
    return item;
}

void CampaignPage::refreshTree() {
    ui.contentTree->clear();
    if (m_campaignPath.isEmpty()) return;
    loadDirectory(m_campaignPath, nullptr, ui.filterCombo->currentText());
}

void CampaignPage::loadDirectory(const QString &path, QTreeWidgetItem *parent, const QString &filter) {
    QDir dir(path);

    for (const QString &subDir : dir.entryList(QDir::Dirs | QDir::NoDotAndDotDot, QDir::Name)) {
        auto *folder = addFolder(subDir, parent);
        loadDirectory(path + "/" + subDir, folder, filter);
    }

    for (const QString &file : dir.entryList(QDir::Files, QDir::Name)) {
        QString type = "NPC";
        if (file.startsWith("item_"))     type = "Item";
        else if (file.startsWith("loc_")) type = "Location";
        else if (file.startsWith("map_")) type = "Map";
        else if (file.startsWith("npc_")) type = "NPC";

        if (filter != "All") {
            const QMap<QString, QString> filterMap = {
                {"NPCs", "NPC"}, {"Items", "Item"},
                {"Locations", "Location"}, {"Maps", "Map"}
            };
            if (filterMap.value(filter) != type) continue;
        }

        QString displayName = file;
        if (displayName.contains('_'))
            displayName = displayName.mid(displayName.indexOf('_') + 1);
        displayName = displayName.left(displayName.lastIndexOf('.'));

        addEntry(displayName, type, parent, path + "/" + file);
    }
}

void CampaignPage::showContextMenu(const QPoint &pos) {
    auto *item = ui.contentTree->itemAt(pos);
    QMenu menu(this);

    if (!item) {
        menu.addAction("📁 New Folder", this, [this] {
            bool ok = false;
            const QString name = QInputDialog::getText(this, "New Folder", "Folder name:",
                QLineEdit::Normal, {}, &ok).trimmed();
            if (!ok || name.isEmpty()) return;
            addFolder(name);
        });
    } else {
        const QString type = item->data(0, Qt::UserRole).toString();
        if (type == "folder") {
            menu.addAction("Rename", this, [this, item] {
                bool ok = false;
                const QString name = QInputDialog::getText(this, "Rename", "New name:",
                    QLineEdit::Normal, item->text(0).mid(3), &ok).trimmed();
                if (ok && !name.isEmpty())
                    item->setText(0, "📁 " + name);
            });
            menu.addAction("📁 New Folder inside", this, [this, item] {
                bool ok = false;
                const QString name = QInputDialog::getText(this, "New Folder", "Folder name:",
                    QLineEdit::Normal, {}, &ok).trimmed();
                if (ok && !name.isEmpty())
                    addFolder(name, item);
            });
            menu.addSeparator();
        } else {
            menu.addAction("Rename", this, [this, item] {
                bool ok = false;
                const QString name = QInputDialog::getText(this, "Rename", "New name:",
                    QLineEdit::Normal, item->text(0).mid(3), &ok).trimmed();
                if (ok && !name.isEmpty()) {
                    const QString t = item->data(0, Qt::UserRole).toString();
                    const QMap<QString, QString> icons = {
                        {"NPC", "👤"}, {"Item", "⚔"}, {"Location", "📍"}, {"Map", "🗺"}
                    };
                    item->setText(0, icons.value(t, "•") + " " + name);
                }
            });
            menu.addSeparator();
        }
        menu.addAction("🗑 Delete", this, [this, item] {
            const auto answer = QMessageBox::question(this, "Delete",
                QString("Delete \"%1\"?").arg(item->text(0)));
            if (answer == QMessageBox::Yes)
                delete item;
        });
    }

    menu.exec(ui.contentTree->viewport()->mapToGlobal(pos));
}