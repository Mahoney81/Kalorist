#ifndef TABWIDGET_H
#define TABWIDGET_H
#include <QtWidgets>

class TabContentWidget;

class TabWidget : public QTabWidget
{
	Q_OBJECT

public:
	TabWidget(QWidget *pParent);
	~TabWidget(void);

	TabContentWidget* currentContentWidget(void) const;
	void addTab(const QString& name);
	void setTabName(const QString& name);
	QString tabName(void) const;

	void copyToTab(const QString& name);
	void moveToTab(const QString& name);

	QStringList tabNameList(void) const;

	QString getCurrentMenuCsv(void) const;

signals:
	void dataChanged(void);

public slots:
	void addTab(void);
	void moveTab(int from, int to);
	void removeTab(int index);

private:
	QTabBar *m_pTabBar = NULL;

	TabContentWidget* createTabWidget(const QString& name);
	QString toFileName(const QString& name) const;
};

#endif // TABWIDGET_H
