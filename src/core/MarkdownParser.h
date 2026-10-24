#pragma once
#include <QMap>
#include <QString>
#include <QStringList>

struct MarkdownDocument {
    QMap<QString, QString> frontmatter;  // key-value pairs from ---
    QString body;                         // everything after the second ---
};

namespace MarkdownParser {
    MarkdownDocument parse(const QString &filePath);
    bool save(const QString &filePath, const MarkdownDocument &doc);
    QString extractSection(const QString &body, const QString &sectionKey);
    QString replaceSection(const QString &body, const QString &sectionKey, const QString &content);
}