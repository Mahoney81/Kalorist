#include "stdafx.h"
#include "TableModel.h"
#include "Exception.h"
#include <QDomDocument>

TableModel::TableModel(const QString& tabName, QObject *pParent) : QAbstractTableModel(pParent),
	XML_FILE_PATH(QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation) + "/")
{
	QDir().mkpath(QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation));
	loadData(tabName);
}

void TableModel::loadData(const QString& tabName)
{
	setTabName(tabName);
	beginResetModel();
	m_dataList.clear();

	QDomDocument doc("Kalorist");
	QFile file(XML_FILE_PATH + tabName + ".xml");
	if (!file.open(QIODevice::ReadOnly))
		return;
	if (!doc.setContent(&file))
	{
		file.close();
		return;
	}
	file.close();

	QDomElement docElem = doc.documentElement();

	QDomNode node = docElem.firstChild();
	while (!node.isNull())
	{
		QDomElement element = node.toElement(); // try to convert the node to an element.
		if (!element.isNull() && element.tagName() == "DataItem")
		{
			ProductPtr pProduct(new Product());
			pProduct->name = element.attribute("name");
			pProduct->energy100g = element.attribute("energy100g").toInt();
			pProduct->totalWeight = element.attribute("totalWeight").toInt();
			pProduct->portionsCount = element.attribute("portionsCount").toInt();
			if (pProduct->portionsCount == 0)
				pProduct->portionsCount = 1;
			pProduct->portionWeight = element.attribute("portionWeight").toInt();
			m_dataList.append(pProduct);
		}
		node = node.nextSibling();
	}

	endResetModel();
}

void TableModel::saveData(void)
{
	QDomDocument doc("Kalorist");
	QDomElement root = doc.createElement("DataList");
	doc.appendChild(root);

	for (ProductPtr pProduct : m_dataList)
	{
		QDomElement tag = doc.createElement("DataItem");
		tag.setAttribute("name", pProduct->name);
		tag.setAttribute("energy100g", pProduct->energy100g);
		tag.setAttribute("totalWeight", pProduct->totalWeight);
		tag.setAttribute("portionsCount", pProduct->portionsCount);
		tag.setAttribute("portionWeight", pProduct->portionWeight);
		root.appendChild(tag);
	}

	QFile file(XML_FILE_PATH + m_tabName + ".xml");
	if (file.open(QIODevice::WriteOnly | QIODevice::Truncate))
	{
		file.write(doc.toString().toUtf8());
		file.close();
	}
}

void TableModel::setTabName(const QString& tabName)
{
	QString oldFileName = XML_FILE_PATH + m_tabName + ".xml";
	if(QFile::exists(oldFileName))
	{
		QString newFileName = XML_FILE_PATH + tabName + ".xml";
		if(!QFile::rename(oldFileName, newFileName))
			throw Exception(tr("Could not rename menu '%1'!").arg(m_tabName));
	}

	m_tabName = tabName;
}

int TableModel::rowCount(const QModelIndex& parent) const
{
	Q_UNUSED(parent);

	return m_dataList.count();
}

int TableModel::columnCount(const QModelIndex& parent) const
{
	Q_UNUSED(parent);

	return 7;
}

QVariant TableModel::data(const QModelIndex& index, int role) const
{
	try
	{
		if (index.isValid())
		{
			int row = index.row();
			int column = index.column();

			ProductPtr pDataItem = m_dataList[row];

			if (role == Qt::DisplayRole)
			{
				int portionCalories = calculatePortionCalories(pDataItem);

				switch (column)
				{
				case 0:
					return pDataItem->name;
				case 1:
					return tr("%1 cal").arg(pDataItem->energy100g);
				case 2:
					return tr("%1 g").arg(pDataItem->totalWeight);
				case 3:
					return tr("%1").arg(pDataItem->portionsCount);
				case 4:
					return tr("%1 g").arg(pDataItem->portionWeight);
				case 5:
					return tr("%1 cal").arg(portionCalories);
				case 6:
					return tr("%1% GDP").arg(portionCalories * 100 / DAY_CALORIES);
				}
			}
			else if (role == Qt::EditRole)
			{
				switch (column)
				{
				case 0:
					return pDataItem->name;
				case 1:
					return pDataItem->energy100g;
				case 2:
					return pDataItem->totalWeight;
				case 3:
					return pDataItem->portionsCount;
				case 4:
					return pDataItem->portionWeight;
				}
			}
			else if (role == Qt::TextAlignmentRole)
			{
				if (column == 0)
					return Qt::AlignLeft;
				else
					return Qt::AlignRight;
			}
			else if (role == Qt::BackgroundColorRole)
			{
				const QColor redColor = QColor(Qt::red).lighter();
				const QColor yellowColor = QColor(Qt::yellow).lighter();
				switch (column)
				{
				case 0:
					return pDataItem->name.isEmpty() ? redColor : QVariant();
				case 1:
					return pDataItem->energy100g == 0 ? redColor : QVariant();
				case 2:
					if (pDataItem->totalWeight == 0)
						return  redColor;
					else if (!pDataItem->isConsistent())
						return yellowColor;
					else
						return QVariant();
				case 3:
					if(pDataItem->portionsCount < 1 && pDataItem->totalWeight != 0)
						return redColor;
					else if (!pDataItem->isConsistent())
						return yellowColor;
					else
						return QVariant();
				case 4:
					if (pDataItem->portionWeight == 0)
						return redColor;
					else if (!pDataItem->isConsistent())
						return yellowColor;
					else
						return QVariant();
				case 5:
				case 6:
					return QColor(230, 230, 230);
				}
			}
		}
	}
	catch (Exception& ex)
	{
		qDebug() << "TableModel::data(...) const Exception: " + ex.what();
	}

	return QVariant();
}

QVariant TableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
	if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
	{
		switch (section)
		{
		case 0:
			return tr("Name");
		case 1:
			return tr("100g Energy");
		case 2:
			return tr("Total Weight");
		case 3:
			return tr("Portions Count");
		case 4:
			return tr("Portion Weight");
		case 5:
			return tr("Portion Energy");
		case 6:
			return tr("Ratio");
		}
	}
	return QVariant();
}

bool TableModel::setData(const QModelIndex& index, const QVariant& value, int role)
{
	try
	{
		if (index.isValid() && role == Qt::EditRole)
		{
			int row = index.row();
			int column = index.column();

			ProductPtr pDataItem = m_dataList[row];

			switch (column)
			{
			case 0:
				pDataItem->name = value.toString();
				break;
			case 1:
				pDataItem->energy100g = value.toInt();
				break;
			case 2:
				pDataItem->totalWeight = value.toInt();
				pDataItem->portionWeight = pDataItem->totalWeight / qMax(pDataItem->portionsCount, 1);
				break;
			case 3:
				pDataItem->portionsCount = value.toInt();
				pDataItem->portionWeight = pDataItem->totalWeight / qMax(pDataItem->portionsCount, 1);
				break;
			case 4:
				pDataItem->portionWeight = value.toInt();
				break;
			}
			saveData();
			emit dataChanged(index, index);
			return true;
		}
		else
			return false;
	}
	catch (ExceptionPtr pException)
	{
		qDebug() << "TableModel::setData(...) Exception: " + pException->what();
		return false;
	}
}

Qt::ItemFlags TableModel::flags(const QModelIndex& index) const
{
	if (index.isValid())
	{
		if (index.column() < 5)
			return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable;
		else
			return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
	}
	else
		return 0;
}

bool TableModel::insertRows(int row, int count, const QModelIndex& parent)
{
	beginInsertRows(parent, row, row + count - 1);
	for (int i = 0; i < count; i++)
	{
		m_dataList.append(ProductPtr(new Product()));
	}
	saveData();
	endInsertRows();
	return true;
}

bool TableModel::removeRows(int row, int count, const QModelIndex& parent)
{
	try
	{
		beginRemoveRows(parent, row, row + count - 1);
		for (int i = 0; i < count; i++)
		{
			m_dataList.removeAt(row + i);
		}
		endRemoveRows();
		saveData();
	}
	catch (Exception& ex)
	{
		qDebug() << "TableModel::removeRows(...) Exception: " + ex.what();
		return false;
	}
	return true;
}

void TableModel::addItem(ProductPtr pDataItem)
{
	beginResetModel();
	m_dataList.append(pDataItem);
	endResetModel();
	saveData();
}

ProductPtr TableModel::dataItem(const QModelIndex& index) const
{
	Q_ASSERT(index.isValid());
	return m_dataList[index.row()];
}

int TableModel::totalEnergy(void) const
{
	int energy = 0;
	for(ProductPtr pProduct : m_dataList)
		energy += calculatePortionCalories(pProduct);
	return energy;
}

void TableModel::clear(void)
{
	beginResetModel();
	m_dataList.clear();
	endResetModel();
	saveData();
}

int TableModel::calculatePortionCalories(ProductPtr pProduct) const
{
	double calInOneGramm = pProduct->energy100g / 100.0;
	return pProduct->portionWeight * calInOneGramm;
}
