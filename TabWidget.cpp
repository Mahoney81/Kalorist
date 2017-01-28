#include "stdafx.h"
#include "TabWidget.h"
#include "TabContentWidget.h"

TabWidget::TabWidget(QWidget *pParent) : QTabWidget(pParent)
{
	// Tab Bar
	m_pTabBar = new QTabBar(this);
	setTabBar(m_pTabBar);

	const QStringList namesList = QSettings().value("OpenedList").toStringList();
	if (!namesList.isEmpty())
	{
		for (const QString& name : namesList)
			QTabWidget::addTab(createTabWidget(name), name);
		setCurrentIndex(QSettings().value("LastList").toInt());
	}
	else
		addTab();

	// Properties
	setMovable(true);
	setTabsClosable(true);
#ifdef Q_OS_MAC
	setTabPosition(QTabWidget::South);
#else
	setTabPosition(QTabWidget::North);
#endif

	// Signals
	//connect(m_pTabBar, SIGNAL(addButtonClicked()), SLOT(addTab()));
	connect(m_pTabBar, &QTabBar::tabMoved, this, &TabWidget::moveTab);
	connect(m_pTabBar, &QTabBar::tabCloseRequested, this, &TabWidget::removeTab);
}

TabWidget::~TabWidget(void)
{
	QStringList namesList;
	for (int i = 0; i < count(); i++)
		namesList.append(tabText(i));
	QVariant value(namesList);
	QSettings().setValue("OpenedList", value);
	QSettings().setValue("LastList", currentIndex());
}

TabContentWidget* TabWidget::currentContentWidget(void) const
{
	return dynamic_cast<TabContentWidget*>(widget(currentIndex()));
}

void TabWidget::addTab(const QString& name)
{
	const int index = QTabWidget::addTab(createTabWidget(name), name);
	setCurrentIndex(index);
}

void TabWidget::setTabName(const QString& name)
{
	currentContentWidget()->setTabName(toFileName(name));
	setTabText(currentIndex(), name);
}

QString TabWidget::tabName(void) const
{
	return tabText(currentIndex());
}

void TabWidget::copyToTab(const QString& name)
{
	for (int i = 0; i < count(); i++)
	{
		if (tabText(i) == name)
		{
			TabContentWidget *pOtherWidget = dynamic_cast<TabContentWidget*>(widget(i));
			Q_ASSERT(pOtherWidget);
			TabContentWidget *pCurWidget = currentContentWidget();
			Q_ASSERT(pCurWidget != pOtherWidget);
			ProductPtr pDataItem = pCurWidget->getCurrentItem();
			pOtherWidget->addItem(pDataItem);
			break;
		}
	}
}

void TabWidget::moveToTab(const QString& name)
{
	for(int i = 0; i < count(); i++)
	{
		if (tabText(i) == name)
		{
			TabContentWidget *pOtherWidget = dynamic_cast<TabContentWidget*>(widget(i));
			Q_ASSERT(pOtherWidget);
			TabContentWidget *pCurWidget = currentContentWidget();
			Q_ASSERT(pCurWidget != pOtherWidget);
			ProductPtr pDataItem = pCurWidget->getCurrentItem();
			pOtherWidget->addItem(pDataItem);
			pCurWidget->deleteCurrentItem();
			break;
		}
	}
}

QStringList TabWidget::tabNameList(void) const
{
	QStringList nameList;
	for (int i = 0; i < count(); i++)
		nameList.append(toFileName(tabText(i)));
	return nameList;
}

QString TabWidget::getCurrentMenuCsv(void) const
{
	const TabContentWidget *pWidget = currentContentWidget();
	return pWidget->getDataAsCsv();
}

void TabWidget::addTab(void)
{
	const QString name = tr("Menu %1").arg(count() + 1);
	addTab(name);
}

void TabWidget::moveTab(int from, int to)
{
	tabBar()->moveTab(from, to);
}

void TabWidget::removeTab(int index)
{
	if (index != 0)
		QTabWidget::removeTab(index);
}

TabContentWidget* TabWidget::createTabWidget(const QString& name)
{
	TabContentWidget *pWidget = new TabContentWidget(toFileName(name), this);
	connect(pWidget, &TabContentWidget::dataChanged, this, &TabWidget::dataChanged);
	return pWidget;
}

QString TabWidget::toFileName(const QString& name) const
{
	QString fileName = name;
	fileName.replace(' ', '_');
	return fileName;
}
