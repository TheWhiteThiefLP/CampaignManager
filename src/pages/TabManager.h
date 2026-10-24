#pragma once
#include <QWidget>

class QTabBar;
class QStackedWidget;

class TabManager : public QWidget {
    Q_OBJECT
public:
    explicit TabManager(QWidget *parent = nullptr);
    void openFile(const QString &filePath, const QString &title, QWidget *content);
    void saveTabs(const QString &campaignPath);
    void restoreTabs(const QString &campaignPath);

    signals:
        void tabRequested(const QString &filePath);

protected:
    bool eventFilter(QObject *obj, QEvent *event) override;

private slots:
    void closeTab(int index);
    void onTabChanged(int index);

private:
    int findTab(const QString &filePath) const;

    QTabBar *m_tabBar;
    QStackedWidget *m_stack;
    QStringList m_filePaths;
};