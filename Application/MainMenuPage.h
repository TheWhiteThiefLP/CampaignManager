// MainMenuPage.h
#pragma once
#include <QWidget>

class QListWidget;
class QPushButton;

class MainMenuPage : public QWidget {
    Q_OBJECT
public:
    explicit MainMenuPage(QWidget *parent = nullptr);

    signals:
        void campaignOpened(const QString &name);

private:
    void addCampaign();
    void deleteCampaign();
    void openSelected();
    void updateButtons();
    void persist();

    QListWidget *m_list;
    QPushButton *m_openBtn;
    QPushButton *m_deleteBtn;
};