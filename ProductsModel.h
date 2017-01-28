#ifndef PRODUCTSMODEL_H
#define PRODUCTSMODEL_H

#include <QAbstractListModel>

class ProductsModel : public QAbstractListModel
{
public:
	ProductsModel(QObject *pParent);

	bool addIfNotExist(const QString& value);

	virtual int rowCount(const QModelIndex& parent = QModelIndex()) const override;
	virtual QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
	virtual bool setData(const QModelIndex& index, const QVariant& value, int role = Qt::DisplayRole) override;

private:
	QStringList m_dataList;

	void load(void);
	void save(void);

	QString getFilePath(void) const;
};

#endif // PRODUCTSMODEL_H
