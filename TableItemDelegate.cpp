#include "stdafx.h"
#include "TableItemDelegate.h"
#include "ProductsModel.h"

TableItemDelegate::TableItemDelegate(QObject *pParent) : QItemDelegate(pParent)
{
}

QWidget* TableItemDelegate::createEditor(QWidget *pParent, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
	Q_UNUSED(option);
	if (index.column() == 0)
	{
		QComboBox *pCombo = new QComboBox(pParent);
		pCombo->setStyle(QStyleFactory::create("Fusion"));
		pCombo->setEditable(true);
		pCombo->setInsertPolicy(QComboBox::NoInsert);
		pCombo->setModel(new ProductsModel(pParent));
		return pCombo;
	}
	else
	{
		QSpinBox *pSpinBox = new QSpinBox(pParent);
		pSpinBox->setStyle(QStyleFactory::create("Fusion"));
		pSpinBox->setMinimum(0);
		pSpinBox->setMaximum(10000);
		return pSpinBox;
	}
}

void TableItemDelegate::setEditorData(QWidget *pEditor, const QModelIndex& index) const
{
	if (index.column() == 0)
	{
		QComboBox *pCombo = dynamic_cast<QComboBox*>(pEditor);
		Q_ASSERT(pCombo);
		pCombo->setEditText(index.data(Qt::EditRole).toString());
	}
	else
		QItemDelegate::setEditorData(pEditor, index);
}

void TableItemDelegate::setModelData(QWidget *pEditor, QAbstractItemModel *pModel, const QModelIndex& index) const
{
	if (index.column() == 0)
	{
		const QComboBox *pCombo = dynamic_cast<QComboBox*>(pEditor);
		Q_ASSERT(pCombo);
		const QString value = pCombo->currentText();
		pModel->setData(index, value);

		ProductsModel *pProductsModel = dynamic_cast<ProductsModel*>(pCombo->model());
		Q_ASSERT(pProductsModel);
		pProductsModel->addIfNotExist(value);
	}
	else
		QItemDelegate::setModelData(pEditor, pModel, index);
}
