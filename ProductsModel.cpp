#include "stdafx.h"
#include "ProductsModel.h"

ProductsModel::ProductsModel(QObject *pParent) : QAbstractListModel(pParent)
{
	load();
}

bool ProductsModel::addIfNotExist(const QString& value)
{
	if (!m_dataList.contains(value, Qt::CaseInsensitive))
	{
		beginResetModel();
		m_dataList.append(value);
		endResetModel();
		save();
		return true;
	}
	else
		return false;
}

int ProductsModel::rowCount(const QModelIndex& parent) const
{
	Q_UNUSED(parent);
	return m_dataList.count();
}

QVariant ProductsModel::data(const QModelIndex& index, int role) const
{
	if (index.isValid() && (role == Qt::DisplayRole || role == Qt::EditRole))
		return m_dataList[index.row()];
	else
		return QVariant();
}

bool ProductsModel::setData(const QModelIndex& index, const QVariant& value, int role)
{
	if (index.isValid() && role == Qt::EditRole)
	{
		m_dataList[index.row()] = value.toString();
		return true;
	}
	return false;
}

void ProductsModel::load(void)
{
	QString filePath = getFilePath();

	if(!QFile::exists(filePath))
		filePath = ":/products/products_" + QLocale::system().name();

	QFile file(filePath);
	if (file.open(QIODevice::ReadOnly))
	{
		QDomDocument doc("Products");
		if (!doc.setContent(&file))
		{
			file.close();
			return;
		}
		file.close();

		const QDomElement docElem = doc.documentElement();

		beginResetModel();

		m_dataList.clear();

		QDomNode node = docElem.firstChild();
		while (!node.isNull())
		{
			const QDomElement element = node.toElement(); // try to convert the node to an element.
			if (!element.isNull() && element.tagName() == "Product")
				m_dataList.append(element.attribute("name"));
			node = node.nextSibling();
		}

		qSort(m_dataList);

		endResetModel();
	}
}

void ProductsModel::save(void)
{
	QDomDocument doc("Products");
	QDomElement root = doc.createElement("ProductsList");
	doc.appendChild(root);

	for (const QString& data : m_dataList)
	{
		QDomElement tag = doc.createElement("Product");
		tag.setAttribute("name", data);
		root.appendChild(tag);
	}

	QFile file(getFilePath());
	if (file.open(QIODevice::WriteOnly | QIODevice::Truncate))
	{
		file.write(doc.toString().toUtf8());
		file.close();
	}
}

QString ProductsModel::getFilePath(void) const
{
	QString filePath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + QDir::separator() + "products_" + QLocale::system().name() + ".xml";
	return filePath;
}
