#ifndef LISTMIMEDATA_H
#define LISTMIMEDATA_H

#include <QMimeData>

class ListMimeData : public QMimeData
{
    Q_OBJECT
public:
    ListMimeData();

    void setList(const QList<QStringList> &list);
    QList<QStringList> list() const;

    QStringList formats();
    bool hasFormat(const QString &mimetype) const Q_DECL_OVERRIDE;

protected:
    QVariant retrieveData(const QString &mimetype, QVariant::Type preferredType) const Q_DECL_OVERRIDE;

};

#endif // LISTMIMEDATA_H
