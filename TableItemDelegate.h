#ifndef TABLEITEMDELEGATE_H
#define TABLEITEMDELEGATE_H

#include <QItemDelegate>

class TableItemDelegate : public QItemDelegate
{
public:
	TableItemDelegate(QObject *pParent);

	virtual QWidget* createEditor(QWidget *pParent, const QStyleOptionViewItem& option, const QModelIndex& index) const override;
	virtual void setEditorData(QWidget *pEditor, const QModelIndex& index) const override;
	virtual void setModelData(QWidget *pEditor, QAbstractItemModel *pModel, const QModelIndex& index) const override;
};

#endif // TABLEITEMDELEGATE_H
