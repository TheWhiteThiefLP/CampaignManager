#include "NPCEditor.h"
#include "MarkdownParser.h"

#include <QComboBox>
#include <QFormLayout>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QScrollArea>
#include <QSpinBox>
#include <QTextEdit>
#include <QVBoxLayout>

NPCEditor::NPCEditor(const QString &filePath, QWidget *parent)
    : QWidget(parent), m_filePath(filePath) {

    // ── scroll area wraps everything ──────────────────────────────────────
    auto *scroll = new QScrollArea(this);
    scroll->setWidgetResizable(true);
    auto *root = new QWidget;
    scroll->setWidget(root);
    auto *rootLayout = new QVBoxLayout(this);
    rootLayout->setContentsMargins(0, 0, 0, 0);
    rootLayout->addWidget(scroll);

    auto *layout = new QVBoxLayout(root);
    layout->setContentsMargins(16, 16, 16, 16);
    layout->setSpacing(12);

    // ── header: title + save button ──────────────────────────────────────
    auto *headerRow = new QHBoxLayout;
    auto *titleLabel = new QLabel("NPC");
    QFont tf = titleLabel->font();
    tf.setPointSize(16);
    tf.setBold(true);
    titleLabel->setFont(tf);
    auto *saveBtn = new QPushButton("💾 Save");
    saveBtn->setFixedWidth(100);
    headerRow->addWidget(titleLabel);
    headerRow->addStretch();
    headerRow->addWidget(saveBtn);
    layout->addLayout(headerRow);

    // ── frontmatter fields ────────────────────────────────────────────────
    auto *fmGroup = new QGroupBox("Details");
    auto *fmForm  = new QFormLayout(fmGroup);
    fmForm->setSpacing(8);

    m_name      = new QLineEdit;
    m_race      = new QLineEdit;
    m_class     = new QLineEdit;
    m_alignment = new QComboBox;
    m_status    = new QComboBox;
    m_tags      = new QLineEdit;

    m_alignment->addItems({
        "Lawful Good", "Neutral Good", "Chaotic Good",
        "Lawful Neutral", "True Neutral", "Chaotic Neutral",
        "Lawful Evil", "Neutral Evil", "Chaotic Evil", "Unaligned"
    });
    m_status->addItems({"Alive", "Dead", "Unknown", "Missing"});

    fmForm->addRow("Name",      m_name);
    fmForm->addRow("Race",      m_race);
    fmForm->addRow("Class",     m_class);
    fmForm->addRow("Alignment", m_alignment);
    fmForm->addRow("Status",    m_status);
    fmForm->addRow("Tags",      m_tags);
    layout->addWidget(fmGroup);

    // ── statblock ─────────────────────────────────────────────────────────
    auto *sbGroup  = new QGroupBox("Statblock");
    auto *sbLayout = new QVBoxLayout(sbGroup);

    // six ability scores
    auto *statsRow    = new QHBoxLayout;
    auto *statsWidget = new QWidget;
    statsWidget->setLayout(statsRow);
    for (auto [label, spinPtr] : {
             std::pair{"STR", &m_str}, {"DEX", &m_dex}, {"CON", &m_con},
             {"INT", &m_int}, {"WIS", &m_wis}, {"CHA", &m_cha}}) {
        auto *col = new QVBoxLayout;
        auto *lbl = new QLabel(label);
        lbl->setAlignment(Qt::AlignCenter);
        *spinPtr = new QSpinBox;
        (*spinPtr)->setRange(1, 30);
        (*spinPtr)->setValue(10);
        (*spinPtr)->setAlignment(Qt::AlignCenter);
        col->addWidget(lbl);
        col->addWidget(*spinPtr);
        statsRow->addLayout(col);
    }
    sbLayout->addWidget(statsWidget);

    // remaining stat fields
    auto *sbForm = new QFormLayout;
    m_ac                  = new QLineEdit;
    m_hp                  = new QLineEdit;
    m_speed               = new QLineEdit;
    m_cr                  = new QLineEdit;
    m_savingThrows        = new QLineEdit;
    m_skills              = new QLineEdit;
    m_senses              = new QLineEdit;
    m_languages           = new QLineEdit;
    m_resistances         = new QLineEdit;
    m_immunities          = new QLineEdit;
    m_conditionImmunities = new QLineEdit;

    sbForm->addRow("Armor Class",           m_ac);
    sbForm->addRow("Hit Points",            m_hp);
    sbForm->addRow("Speed",                 m_speed);
    sbForm->addRow("Challenge Rating",      m_cr);
    sbForm->addRow("Saving Throws",         m_savingThrows);
    sbForm->addRow("Skills",                m_skills);
    sbForm->addRow("Senses",                m_senses);
    sbForm->addRow("Languages",             m_languages);
    sbForm->addRow("Damage Resistances",    m_resistances);
    sbForm->addRow("Damage Immunities",     m_immunities);
    sbForm->addRow("Condition Immunities",  m_conditionImmunities);
    sbLayout->addLayout(sbForm);
    layout->addWidget(sbGroup);

    // ── callout sections ──────────────────────────────────────────────────
    for (auto [title, key] : {
             std::pair{"Description", "description"},
             {"Wants & Needs",        "wantsneeds"},
             {"Carrying",             "carrying"}}) {
        auto *group  = new QGroupBox(title);
        auto *gLayout = new QVBoxLayout(group);
        auto *edit   = new QTextEdit;
        edit->setMinimumHeight(120);
        edit->setPlaceholderText(QString("Enter %1...").arg(title));
        gLayout->addWidget(edit);
        m_sections[key] = edit;
        layout->addWidget(group);
    }

    layout->addStretch();

    connect(saveBtn, &QPushButton::clicked, this, &NPCEditor::save);
    load();
}

void NPCEditor::load() {
    m_doc = MarkdownParser::parse(m_filePath);
    const auto &fm = m_doc.frontmatter;

    m_name->setText(fm.value("name"));
    m_race->setText(fm.value("race"));
    m_class->setText(fm.value("class"));
    m_tags->setText(fm.value("tags"));

    const int alignIdx = m_alignment->findText(fm.value("alignment"));
    if (alignIdx >= 0) m_alignment->setCurrentIndex(alignIdx);
    const int statusIdx = m_status->findText(fm.value("status"));
    if (statusIdx >= 0) m_status->setCurrentIndex(statusIdx);

    // statblock fields stored in frontmatter
    m_ac->setText(fm.value("ac"));
    m_hp->setText(fm.value("hp"));
    m_speed->setText(fm.value("speed"));
    m_cr->setText(fm.value("cr"));
    m_savingThrows->setText(fm.value("savingThrows"));
    m_skills->setText(fm.value("skills"));
    m_senses->setText(fm.value("senses"));
    m_languages->setText(fm.value("languages"));
    m_resistances->setText(fm.value("resistances"));
    m_immunities->setText(fm.value("immunities"));
    m_conditionImmunities->setText(fm.value("conditionImmunities"));

    m_str->setValue(fm.value("str", "10").toInt());
    m_dex->setValue(fm.value("dex", "10").toInt());
    m_con->setValue(fm.value("con", "10").toInt());
    m_int->setValue(fm.value("int", "10").toInt());
    m_wis->setValue(fm.value("wis", "10").toInt());
    m_cha->setValue(fm.value("cha", "10").toInt());

    // body sections
    for (auto it = m_sections.begin(); it != m_sections.end(); ++it)
        it.value()->setPlainText(MarkdownParser::extractSection(m_doc.body, it.key()));
}

void NPCEditor::save() {
    auto &fm = m_doc.frontmatter;

    fm["type"]      = "NPC";
    fm["name"]      = m_name->text();
    fm["race"]      = m_race->text();
    fm["class"]     = m_class->text();
    fm["alignment"] = m_alignment->currentText();
    fm["status"]    = m_status->currentText();
    fm["tags"]      = m_tags->text();

    fm["str"] = QString::number(m_str->value());
    fm["dex"] = QString::number(m_dex->value());
    fm["con"] = QString::number(m_con->value());
    fm["int"] = QString::number(m_int->value());
    fm["wis"] = QString::number(m_wis->value());
    fm["cha"] = QString::number(m_cha->value());

    fm["ac"]                  = m_ac->text();
    fm["hp"]                  = m_hp->text();
    fm["speed"]               = m_speed->text();
    fm["cr"]                  = m_cr->text();
    fm["savingThrows"]        = m_savingThrows->text();
    fm["skills"]              = m_skills->text();
    fm["senses"]              = m_senses->text();
    fm["languages"]           = m_languages->text();
    fm["resistances"]         = m_resistances->text();
    fm["immunities"]          = m_immunities->text();
    fm["conditionImmunities"] = m_conditionImmunities->text();

    // save body sections
    for (auto it = m_sections.constBegin(); it != m_sections.constEnd(); ++it)
        m_doc.body = MarkdownParser::replaceSection(m_doc.body, it.key(),
                                                     it.value()->toPlainText());

    MarkdownParser::save(m_filePath, m_doc);
}