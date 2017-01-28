#ifndef TABLEMODEL_H
#define TABLEMODEL_H

#include <QtGui>
#include "Product.h"

class TableModel : public QAbstractTableModel
{
	Q_OBJECT
public:
	TableModel(const QString& tabName, QObject *pParent);

	void loadData(const QString& tabName);
	void saveData(void);

	void setTabName(const QString& tabName);

	virtual int rowCount(const QModelIndex& parent = QModelIndex()) const override;
	virtual int columnCount(const QModelIndex& parent = QModelIndex()) const override;
	virtual QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
	virtual QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

	virtual bool setData(const QModelIndex& index, const QVariant& value, int role = Qt::EditRole) override;
	virtual Qt::ItemFlags flags(const QModelIndex& index) const override;

	virtual bool insertRows(int row, int count, const QModelIndex& parent = QModelIndex()) override;
	virtual bool removeRows(int row, int count, const QModelIndex& parent = QModelIndex()) override;

	void addItem(ProductPtr pDataItem);
	ProductPtr dataItem(const QModelIndex& index) const;

	int totalEnergy(void) const;

	void clear(void);

private:
	QList<ProductPtr> m_dataList;

	const int DAY_CALORIES = 2500;
	const QString XML_FILE_PATH;
	QString m_tabName;

	int calculatePortionCalories(ProductPtr pProduct) const;
};

#endif // TABLEMODEL_H
