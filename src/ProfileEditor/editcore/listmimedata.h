#ifndef LISTMIMEDATA_H
#define LISTMIMEDATA_H

#include <QMimeData>

class ListMimeData : public QMimeData
{
    Q_OBJECT
public:
    ListMimeData();

    void setList(const QList<QStringList> *list);
//    QList<QStringList> list() const;

    QStringList formats();
    bool hasFormat(const QString &mimetype) const override;

protected:
    QVariant retrieveData(const QString &mimetype, QVariant::Type preferredType) const override;

private:
    QString toCsv(QString plainText);
};

#endif // LISTMIMEDATA_H
