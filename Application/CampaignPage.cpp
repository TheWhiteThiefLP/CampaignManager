// CampaignPage.cpp
#include "CampaignPage.h"

#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>

CampaignPage::CampaignPage(QWidget *parent) : QWidget(parent) {
    auto *back = new QPushButton("← Back to campaigns", this);
    m_title = new QLabel(this);
    QFont f = m_title->font();
    f.setPointSize(20);
    f.setBold(true);
    m_title->setFont(f);

    auto *layout = new QVBoxLayout(this);
    layout->addWidget(back, 0, Qt::AlignLeft);
    layout->addWidget(m_title);
    layout->addWidget(new QLabel("NPCs, items, maps and locations will go here.", this));
    layout->addStretch();

    connect(back, &QPushButton::clicked, this, &CampaignPage::backRequested);
}

void CampaignPage::setCampaign(const QString &name) {
    m_title->setText(name);
}