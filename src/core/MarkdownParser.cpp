#include "MarkdownParser.h"
#include <QFile>
#include <QTextStream>
#include <QRegularExpression>

MarkdownDocument MarkdownParser::parse(const QString &filePath) {
    MarkdownDocument doc;
    QFile f(filePath);
    if (!f.open(QIODevice::ReadOnly | QIODevice::Text))
        return doc;

    QTextStream in(&f);
    const QString content = in.readAll();
    f.close();

    // split frontmatter from body
    const QRegularExpression fmRegex(R"(^---\n(.*?)\n---\n(.*))",
        QRegularExpression::DotMatchesEverythingOption |
        QRegularExpression::MultilineOption);
    const auto match = fmRegex.match(content);

    if (!match.hasMatch()) {
        doc.body = content;
        return doc;
    }

    // parse frontmatter key: value pairs
    const QString fm = match.captured(1);
    for (const QString &line : fm.split('\n')) {
        const int colon = line.indexOf(':');
        if (colon < 0) continue;
        const QString key   = line.left(colon).trimmed();
        const QString value = line.mid(colon + 1).trimmed();
        doc.frontmatter[key] = value;
    }

    doc.body = match.captured(2);
    return doc;
}

bool MarkdownParser::save(const QString &filePath, const MarkdownDocument &doc) {
    QFile f(filePath);
    if (!f.open(QIODevice::WriteOnly | QIODevice::Text))
        return false;

    QTextStream out(&f);
    out << "---\n";
    for (auto it = doc.frontmatter.constBegin(); it != doc.frontmatter.constEnd(); ++it)
        out << it.key() << ": " << it.value() << "\n";
    out << "---\n";
    out << doc.body;
    return true;
}

QString MarkdownParser::extractSection(const QString &body, const QString &sectionKey) {
    // matches > [!key] Title\n> content lines
    const QRegularExpression re(
        QString(R"(> \[!%1\][^\n]*\n((?:> [^\n]*\n?)*))")
            .arg(QRegularExpression::escape(sectionKey)));
    const auto match = re.match(body);
    if (!match.hasMatch()) return {};

    // strip leading "> " from each line
    QString result;
    for (const QString &line : match.captured(1).split('\n')) {
        if (line.startsWith("> "))
            result += line.mid(2) + "\n";
        else if (line == ">")
            result += "\n";
    }
    return result.trimmed();
}

QString MarkdownParser::replaceSection(const QString &body, const QString &sectionKey,
                                        const QString &content) {
    const QRegularExpression re(
        QString(R"(> \[!%1\][^\n]*\n(?:> [^\n]*\n?)*)")
            .arg(QRegularExpression::escape(sectionKey)));

    // build replacement block
    QString block = QString("> [!%1] %2\n").arg(sectionKey,
        sectionKey == "wantsneeds" ? "Wants & Needs" :
        sectionKey == "description" ? "Description" :
        sectionKey == "carrying" ? "Carrying" : sectionKey);
    for (const QString &line : content.split('\n'))
        block += "> " + line + "\n";

    if (body.contains(re))
        return QString(body).replace(re, block);

    // section doesn't exist yet — append it
    return body + "\n" + block;
}