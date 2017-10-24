#ifndef FLOWDELEGATE_H
#define FLOWDELEGATE_H

#include <QStyledItemDelegate>
#include <flowcelldata.h>
#include <flowcellwidget.h>
#include <QPaintEngine>
#include <QPainter>

class FlowDelegate : public QStyledItemDelegate
{
    Q_OBJECT

public:
    explicit FlowDelegate(QWidget *parent = 0);
    ~FlowDelegate();

    void paint(QPainter *painter, const QStyleOptionViewItem &option,
               const QModelIndex &index) const override;
    QSize sizeHint(const QStyleOptionViewItem &option,
                   const QModelIndex &index) const override;
    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                          const QModelIndex &index) const override;
    void setEditorData(QWidget *editor, const QModelIndex &index) const override;
    void setModelData(QWidget *editor, QAbstractItemModel *model,
                      const QModelIndex &index) const override;

    void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option,
                              const QModelIndex &index) const;

};

#endif // FLOWDELEGATE_H
