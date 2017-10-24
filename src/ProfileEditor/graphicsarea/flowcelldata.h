#ifndef FLOWCELLDATA_H
#define FLOWCELLDATA_H

#include <QMetaType>
#include <QObject>
#include <QPixmap>

class FlowCellData
{
public:
    explicit FlowCellData();
    ~FlowCellData();

    QString getTypecolor() const;
    void setTypecolor(const QString &csstext);

    QString getFramecolor() const;
    void setFramecolor(const QString &csstext);

    QString getContent() const;
    void setContent(const QString &htmlstr);

    QPixmap getTypepixmap() const;
    void setTypepixmap(const QPixmap &value);

    QString getType() const;
    void setType(const QString &value);

    int height() const;
    void setheight(int value);

private:
    QString type;
    QString typecolor;
    QString framecolor;
    QString content;
    QPixmap typepixmap;
    int cellheight;
};

Q_DECLARE_METATYPE(FlowCellData)

#endif // FLOWCELLDATA_H
