#include "TabManager.h"
#include <QLabel>
#include <QMouseEvent>
#include <QTabBar>
#include <QStackedWidget>
#include <QVBoxLayout>
#include <QSettings>

TabManager::TabManager(QWidget *parent) : QWidget(parent) {
    m_tabBar = new QTabBar(this);
    m_tabBar->setTabsClosable(true);
    m_tabBar->setMovable(true);
    m_tabBar->setExpanding(false);
    m_tabBar->setUsesScrollButtons(true);
    m_tabBar->setElideMode(Qt::ElideRight);
    m_tabBar->setStyleSheet(
        "QTabBar::tab {"
        "    padding: 4px 24px 4px 10px;"  // top right bottom left
        "    min-width: 80px;"
        "}"
        "QTabBar::tab:selected {"
        "    font-weight: bold;"
        "}"
        "QTabBar::close-button {"
        "    margin-right: 4px;"
        "}"
    );
    m_stack = new QStackedWidget(this);

    auto *layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);
    layout->addWidget(m_tabBar);
    layout->addWidget(m_stack, 1);

    connect(m_tabBar, &QTabBar::tabCloseRequested,  this, &TabManager::closeTab);
    connect(m_tabBar, &QTabBar::currentChanged,      this, &TabManager::onTabChanged);
    connect(m_tabBar, &QTabBar::tabBarDoubleClicked, this, &TabManager::closeTab);

    // middle click
    m_tabBar->installEventFilter(this);
}

bool TabManager::eventFilter(QObject *obj, QEvent *event) {
    if (obj == m_tabBar && event->type() == QEvent::MouseButtonRelease) {
        auto *e = static_cast<QMouseEvent*>(event);
        if (e->button() == Qt::MiddleButton) {
            const int index = m_tabBar->tabAt(e->pos());
            if (index >= 0) closeTab(index);
        }
    }
    return QWidget::eventFilter(obj, event);
}

void TabManager::openFile(const QString &filePath, const QString &title, QWidget *content) {
    // switch to existing tab if already open
    const int existing = findTab(filePath);
    if (existing >= 0) {
        m_tabBar->setCurrentIndex(existing);
        return;
    }

    // add new tab
    m_filePaths.append(filePath);
    m_stack->addWidget(content);
    const int index = m_tabBar->addTab(title);
    m_tabBar->setTabToolTip(index, filePath);
    m_tabBar->setCurrentIndex(index);
}

void TabManager::closeTab(int index) {
    if (index < 0 || index >= m_filePaths.size()) return;

    m_filePaths.removeAt(index);
    QWidget *widget = m_stack->widget(index);
    m_stack->removeWidget(widget);
    delete widget;
    m_tabBar->removeTab(index);
}

void TabManager::onTabChanged(int index) {
    if (index >= 0 && index < m_stack->count())
        m_stack->setCurrentIndex(index);
}

int TabManager::findTab(const QString &filePath) const {
    return m_filePaths.indexOf(filePath);
}

void TabManager::saveTabs(const QString &campaignPath) {
    QSettings settings;
    settings.beginGroup("tabs/" + campaignPath);
    settings.setValue("paths", m_filePaths);
    settings.setValue("current", m_tabBar->currentIndex());
    settings.endGroup();
}

void TabManager::restoreTabs(const QString &campaignPath) {
    QSettings settings;
    settings.beginGroup("tabs/" + campaignPath);
    const QStringList paths = settings.value("paths").toStringList();
    const int current = settings.value("current", 0).toInt();
    settings.endGroup();

    emit tabRequested(QString());  // signal CampaignPage to reopen each path
    for (const QString &path : paths)
        emit tabRequested(path);

    if (current >= 0 && current < m_tabBar->count())
        m_tabBar->setCurrentIndex(current);
}