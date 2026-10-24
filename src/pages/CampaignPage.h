#pragma once
#include <QWidget>
#include "ui_CampaignPage.h"

class QTreeWidgetItem;
class TabManager;

class CampaignPage : public QWidget {
    Q_OBJECT
public:
    explicit CampaignPage(QWidget *parent = nullptr);
    void setCampaign(const QString &name);

private slots:
    void onFilterChanged(int);
    void showContextMenu(const QPoint &pos);
    void onItemDoubleClicked(QTreeWidgetItem *item, int column);
    void reopenTab(const QString &filePath);

private:
    void refreshTree();
    void loadDirectory(const QString &path, QTreeWidgetItem *parent, const QString &filter);
    void createEntry(const QString &type);
    QTreeWidgetItem* addFolder(const QString &name, QTreeWidgetItem *parent = nullptr);
    QTreeWidgetItem* addEntry(const QString &name, const QString &type,
                               QTreeWidgetItem *parent = nullptr,
                               const QString &filePath = {});

    QString m_campaignPath;
    TabManager *m_tabs = nullptr;
    Ui::CampaignPage ui;
};