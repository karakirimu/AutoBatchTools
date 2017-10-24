#include "flowdelegate.h"

FlowDelegate::FlowDelegate(QWidget *parent)
    : QStyledItemDelegate(parent)
{

}

FlowDelegate::~FlowDelegate()
{

}

void FlowDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option,
                         const QModelIndex &index) const
{
    if(index.data().canConvert<FlowCellData>()){
        if(option.state & QStyle::State_Selected){
//            painter->fillRect(option.rect, option.palette.highlight());
        } else if(option.state & QStyle::State_Sunken) {
//            painter->fillRect(option.rect, option.palette.dark());
        } else {
//            painter->fillRect(option.rect, option.palette.midlight());
        }
    }else{
        QStyledItemDelegate::paint(painter, option, index);
    }
}

QSize FlowDelegate::sizeHint(const QStyleOptionViewItem &option,
                             const QModelIndex &index) const
{
    return QStyledItemDelegate::sizeHint(option, index);
}

QWidget *FlowDelegate::createEditor(QWidget *parent,
                                    const QStyleOptionViewItem &option,
                                    const QModelIndex &index) const
{
    if(index.data().canConvert<FlowCellData>()){
        FlowCellWidget *cell = new FlowCellWidget(parent);
        return cell;
    }else{
        return QStyledItemDelegate::createEditor(parent, option, index);
    }
}

void FlowDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    if(index.data().canConvert<FlowCellData>()){
        FlowCellData celldata = qvariant_cast<FlowCellData>(index.data());
        FlowCellWidget *cellwidget = qobject_cast<FlowCellWidget *>(editor);
//        cellwidget->setCellData(celldata);
//        cellwidget->show();
    }else{
        QStyledItemDelegate::setEditorData(editor, index);
    }
}

void FlowDelegate::setModelData(QWidget *editor, QAbstractItemModel *model,
                                const QModelIndex &index) const
{
    if(index.data().canConvert<FlowCellData>()){
        FlowCellWidget *cellwidget = qobject_cast<FlowCellWidget *>(editor);
//        model->setData(index, QVariant::fromValue(cellwidget->getCelldata()));
    }else{
        QStyledItemDelegate::setModelData(editor, model, index);
    }
}

void FlowDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option,
                                        const QModelIndex &index) const
{
    if(index.data().canConvert<FlowCellData>()){
        editor->setGeometry(option.rect);
    }
}
