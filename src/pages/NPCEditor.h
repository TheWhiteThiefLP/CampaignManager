#pragma once
#include <QWidget>
#include "MarkdownParser.h"

class QVBoxLayout;
class QLineEdit;
class QComboBox;
class QSpinBox;
class QTextEdit;
class QPushButton;

class NPCEditor : public QWidget {
    Q_OBJECT
public:
    explicit NPCEditor(const QString &filePath, QWidget *parent = nullptr);

private slots:
    void save();

private:
    void load();
    void buildStatblock(QWidget *parent, QVBoxLayout *layout);
    void buildSection(const QString &title, const QString &key, QVBoxLayout *layout);

    QString m_filePath;
    MarkdownDocument m_doc;

    // frontmatter fields
    QLineEdit *m_name;
    QLineEdit *m_race;
    QLineEdit *m_class;
    QComboBox *m_alignment;
    QComboBox *m_status;
    QLineEdit *m_tags;

    // statblock
    QSpinBox  *m_str, *m_dex, *m_con, *m_int, *m_wis, *m_cha;
    QLineEdit *m_ac, *m_hp, *m_speed, *m_cr;
    QLineEdit *m_savingThrows, *m_skills, *m_senses, *m_languages;
    QLineEdit *m_resistances, *m_immunities, *m_conditionImmunities;

    // body sections
    QMap<QString, QTextEdit*> m_sections;
};