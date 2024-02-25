#ifndef XLCELL_H
#define XLCELL_H

#include <QString>
#include <QStringList>
#include <QVariant>

class XlCell {
public:
    XlCell(int row = 0, int col = 0, const QVariant& content = QVariant())
        : m_row(row), m_col(col), m_content(content.toString()),
        m_clearedContent(m_content.remove("_x000D_").split('\n')) {}

    const QString& content() const { return m_content; }
    const QStringList& clearedContent() const { return m_clearedContent; }

    inline int row() const { return m_row; }
    inline int col() const { return m_col; }

private:
    int m_row{};
    int m_col{};
    QString m_content;
    QStringList m_clearedContent;
};

#endif // !XLCELL
