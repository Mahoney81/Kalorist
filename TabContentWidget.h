#ifndef TABCONTENTWIDGET_H
#define TABCONTENTWIDGET_H
#include <QtWidgets>
#include "Product.h"

class TableModel;

class TabContentWidget : public QWidget
{
	Q_OBJECT

public:
	TabContentWidget(const QString& tabName, QWidget *pParent);
	~TabContentWidget(void);

	void setTabName(const QString& name);
	void setFilter(const QString& text);

	void addItem(void);
	void addItem(ProductPtr pProduct);
	void deleteCurrentItem(void);
	void deleteAllItems(void);

	ProductPtr getCurrentItem(void);
	int totalEnergy(void) const;
	int itemCount(void) const;

	QString getDataAsCsv(void) const;

signals:
	void dataChanged(void);

private:
	QTableView *m_pTableView = NULL;
	QLineEdit *m_pQuickSearchEdit = NULL;
	const QString m_tabName;

	TableModel* tableViewModel(void) const;

private slots:
	void onQuickSearchEditTextChanged(const QString& text);
};

#endif // TABCONTENTWIDGET_H
