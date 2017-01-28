#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtWidgets>
#include "ui_MainWindow.h"

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow(void);
	~MainWindow(void);

private slots:
	void on_pNewMenuAction_triggered(void);
	void on_pOpenMenuAction_triggered(void);
	void on_pRenameMenuAction_triggered(void);
	void on_pCloseMenuAction_triggered(void);
	void on_pRecordAddAction_triggered(void);
	void on_pRecordDeleteAction_triggered(void);
	void on_pRecordDeleteAllAction_triggered(void);
	void on_pRecordCopyToAction_triggered(void);
	void on_pRecordMoveToAction_triggered(void);
	void on_pExitAction_triggered(void) { close(); }
	void on_pExportAction_triggered(void);
	void on_pAboutAction_triggered(void);
	void on_pAboutQtAction_triggered(void);
	void onDataChangeTriggered(void);

private:
	Ui::MainWindowClass m_ui;

	void contextMenuEvent(QContextMenuEvent *pEvent) override;
};

#endif // MAINWINDOW_H
