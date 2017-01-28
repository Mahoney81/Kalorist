#include "stdafx.h"
#include "TabContentWidget.h"
#include "TableModel.h"
#include "TableItemDelegate.h"

TabContentWidget::TabContentWidget(const QString& tabName, QWidget *pParent) : QWidget(pParent), m_tabName(tabName)
{
	m_pTableView = new QTableView(this);
	m_pTableView->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);

	m_pQuickSearchEdit = new QLineEdit(this);
	m_pQuickSearchEdit->setPlaceholderText(tr("Find a record"));
	m_pQuickSearchEdit->setClearButtonEnabled(true);
	connect(m_pQuickSearchEdit, &QLineEdit::textChanged, this, &TabContentWidget::onQuickSearchEditTextChanged);

	QVBoxLayout *pLayout = new QVBoxLayout(this);
	pLayout->addWidget(m_pTableView);
	pLayout->addWidget(m_pQuickSearchEdit);
	pLayout->setMargin(1);
	pLayout->setSpacing(1);
	setLayout(pLayout);

	TableModel *pModel = new TableModel(tabName, m_pTableView);
	connect(pModel, &TableModel::dataChanged, this, &TabContentWidget::dataChanged);
	connect(pModel, &TableModel::modelReset, this, &TabContentWidget::dataChanged);
	connect(pModel, &TableModel::rowsInserted, this, &TabContentWidget::dataChanged);
	connect(pModel, &TableModel::rowsRemoved, this, &TabContentWidget::dataChanged);

	QSortFilterProxyModel *pProxyModel = new QSortFilterProxyModel(this);
	pProxyModel->setSourceModel(pModel);
	m_pTableView->setModel(pProxyModel);
	m_pTableView->setItemDelegate(new TableItemDelegate(m_pTableView));
	m_pTableView->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);

	emit dataChanged();

	const QByteArray headerState = QSettings().value(m_tabName + "headerState").toByteArray();
	bool isOk = false;
	if(!headerState.isEmpty())
		isOk = m_pTableView->horizontalHeader()->restoreState(headerState);
	if(!isOk)
	{
		m_pTableView->setColumnWidth(0, 180);
		int count = m_pTableView->horizontalHeader()->count();
		for(int i = 1; i < count; i++)
			m_pTableView->setColumnWidth(i, 100);
		m_pTableView->setColumnWidth(3, 70);
		m_pTableView->setColumnWidth(6, 70);
	}

	m_pTableView->scrollToBottom();
}

TabContentWidget::~TabContentWidget(void)
{
	QSettings().setValue(m_tabName + "headerState", m_pTableView->horizontalHeader()->saveState());
}

void TabContentWidget::setTabName(const QString& name)
{
	TableModel *pModel = tableViewModel();
	pModel->setTabName(name);
}

void TabContentWidget::setFilter(const QString& text)
{
	static_cast<QSortFilterProxyModel*>(m_pTableView->model())->setFilterRegExp(QRegExp(text, Qt::CaseInsensitive,
		QRegExp::FixedString));
}

void TabContentWidget::addItem(void)
{
	m_pTableView->scrollToBottom();

	const int count = m_pTableView->model()->rowCount();
	m_pTableView->model()->insertRow(count);

	const QModelIndex index = m_pTableView->model()->index(count, 0);
	m_pTableView->setCurrentIndex(index);
	m_pTableView->scrollTo(index, QAbstractItemView::EnsureVisible);
	m_pTableView->edit(index);
}

void TabContentWidget::addItem(ProductPtr pProduct)
{
	QSortFilterProxyModel *pProxyModel = static_cast<QSortFilterProxyModel*>(m_pTableView->model());
	Q_ASSERT(pProxyModel);
	TableModel *pModel = dynamic_cast<TableModel*>(pProxyModel->sourceModel());
	Q_ASSERT(pModel);
	pModel->addItem(pProduct);
}

void TabContentWidget::deleteCurrentItem(void)
{
	const QModelIndex index = m_pTableView->currentIndex();
	if (index.isValid())
		m_pTableView->model()->removeRow(index.row());
}

void TabContentWidget::deleteAllItems(void)
{
	if (m_pTableView->model()->rowCount() == 0)
		return;

	if (QMessageBox::question(this, tr("Warning"), tr("You are about to delete ALL the records in the menu. Do you want to proceed?"), QMessageBox::Yes | QMessageBox::No, QMessageBox::No) == QMessageBox::Yes)
	{
		TableModel *pModel = tableViewModel();
		pModel->clear();
	}
}

ProductPtr TabContentWidget::getCurrentItem(void)
{
	const QModelIndex index = m_pTableView->currentIndex();
	if (!index.isValid())
		throw ExceptionPtr(new Exception(tr("No item selected!")));

	const TableModel *pModel = tableViewModel();
	if (!pModel)
		throw ExceptionPtr(new Exception(tr("Invalid internal state!")));

	return pModel->dataItem(index);
}

int TabContentWidget::totalEnergy(void) const
{
	const TableModel *pModel = tableViewModel();
	if(pModel)
		return pModel->totalEnergy();
	else
		return 0;
}

int TabContentWidget::itemCount(void) const
{
	const TableModel *pModel = tableViewModel();
	if(pModel)
		return pModel->rowCount();
	else
		return 0;
}

QString TabContentWidget::getDataAsCsv(void) const
{
	const TableModel *pModel = tableViewModel();
	if (!pModel)
		throw ExceptionPtr(new Exception(tr("Invalid internal state!")));

	QString csvData;
	csvData += QString("Name,Energy100g,TotalWeight,PortionsCount,PortionWeight\r\n");
	const int count = pModel->rowCount();
	for (int i = 0; i < count; i++)
	{
		ProductPtr pProduct = pModel->dataItem(pModel->index(i, 0));
		csvData += QString("%1,%2,%3,%4,%5\r\n").arg(pProduct->name).arg(pProduct->energy100g).arg(pProduct->totalWeight)
			.arg(pProduct->portionsCount).arg(pProduct->portionWeight);
	}

	return csvData;
}

TableModel* TabContentWidget::tableViewModel(void) const
{
	QSortFilterProxyModel *pProxyModel = dynamic_cast<QSortFilterProxyModel*>(m_pTableView->model());
	Q_ASSERT(pProxyModel);
	TableModel *pModel = dynamic_cast<TableModel*>(pProxyModel->sourceModel());
	Q_ASSERT(pModel);
	return pModel;
}

void TabContentWidget::onQuickSearchEditTextChanged(const QString& text)
{
	setFilter(text);
}
