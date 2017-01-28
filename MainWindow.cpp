#include "stdafx.h"
#include "MainWindow.h"
#include "TabContentWidget.h"
#include "OpenListDialog.h"

MainWindow::MainWindow(void)
{
	m_ui.setupUi(this);

	// Ensure that profile folder is exist
	QDir().mkpath(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation));

	connect(m_ui.pTabWidget, &TabWidget::dataChanged, this, &MainWindow::onDataChangeTriggered);
	connect(m_ui.pTabWidget, &TabWidget::currentChanged, this, &MainWindow::onDataChangeTriggered);

	const QSize windowSize = QSettings().value("WindowSize", QSize(800, 480)).toSize();
	if (windowSize.width() > 100 && windowSize.height() > 100)
		resize(windowSize);

	onDataChangeTriggered();
}

MainWindow::~MainWindow(void)
{
	QSettings().setValue("WindowSize", size());
}

void MainWindow::on_pNewMenuAction_triggered(void)
{
	try
	{
		QInputDialog dlg;
		dlg.setInputMode(QInputDialog::TextInput);
		dlg.setWindowTitle(tr("New Menu"));
		dlg.setLabelText(tr("Enter new menu's name:"));
		dlg.setTextValue(tr("Menu %1").arg(m_ui.pTabWidget->count() + 1));
		dlg.resize(300, 100);
		if (dlg.exec() == QDialog::Accepted)
		{
			const QString name = dlg.textValue();
			m_ui.pTabWidget->addTab(name);
		}
	}
	catch (ExceptionPtr pException)
	{
		DetailedMessageBox::critical(tr("Could not add a new menu."), tr("Click 'Show Details...' for more information."), pException);
	}
}

void MainWindow::on_pOpenMenuAction_triggered(void)
{
	try
	{
		OpenListDialog dlg(m_ui.pTabWidget->tabNameList(), false, this);
		if (dlg.exec() == QDialog::Accepted)
			m_ui.pTabWidget->addTab(dlg.tabName());
	}
	catch (ExceptionPtr pException)
	{
		DetailedMessageBox::critical(tr("Could not open menu."), tr("Click 'Show Details...' for more information."), pException);
	}
}

void MainWindow::on_pRenameMenuAction_triggered(void)
{
	const QString oldTabName = m_ui.pTabWidget->tabName();

	try
	{
		QInputDialog dlg;
		dlg.setInputMode(QInputDialog::TextInput);
		dlg.setWindowTitle(tr("Rename Menu"));
		dlg.setLabelText(tr("Enter menu's new name:"));
		dlg.setTextValue(oldTabName);
		dlg.resize(300, 100);
		if (dlg.exec() == QDialog::Accepted)
		{
			const QString newTabName = dlg.textValue();
			m_ui.pTabWidget->setTabName(newTabName);
		}
	}
	catch (ExceptionPtr pException)
	{
		DetailedMessageBox::critical(tr("Could not rename menu '%1'.").arg(oldTabName), tr("Click 'Show Details...' for more information."), pException);
	}
}

void MainWindow::on_pCloseMenuAction_triggered(void)
{
	try
	{
		m_ui.pTabWidget->removeTab(m_ui.pTabWidget->currentIndex());
	}
	catch (ExceptionPtr pException)
	{
		DetailedMessageBox::critical(tr("Unexpected error occurred trying to close menu."), tr("Click 'Show Details...' for more information."), pException);
	}
}

void MainWindow::on_pRecordAddAction_triggered(void)
{
	try
	{
		m_ui.pTabWidget->currentContentWidget()->addItem();
	}
	catch (ExceptionPtr pException)
	{
		DetailedMessageBox::critical(tr("Could not add a new record."), tr("Click 'Show Details...' for more information."), pException);
	}
}

void MainWindow::on_pRecordDeleteAction_triggered(void)
{
	try
	{
		m_ui.pTabWidget->currentContentWidget()->deleteCurrentItem();
	}
	catch (ExceptionPtr pException)
	{
		DetailedMessageBox::critical(tr("Could not delete a record."), tr("Click 'Show Details...' for more information."), pException);
	}
}

void MainWindow::on_pRecordDeleteAllAction_triggered(void)
{
	try
	{
		m_ui.pTabWidget->currentContentWidget()->deleteAllItems();
	}
	catch (ExceptionPtr pException)
	{
		DetailedMessageBox::critical(tr("Could not clear the records list."), tr("Click 'Show Details...' for more information."), pException);
	}
}

void MainWindow::on_pRecordCopyToAction_triggered(void)
{
	try
	{
		OpenListDialog dlg(m_ui.pTabWidget->tabNameList(), true, this);
		if (dlg.exec() == QDialog::Accepted)
			m_ui.pTabWidget->copyToTab(dlg.tabName());
	}
	catch (ExceptionPtr pException)
	{
		DetailedMessageBox::critical(tr("Could not copy a record."), tr("Click 'Show Details...' for more information."), pException);
	}
}

void MainWindow::on_pRecordMoveToAction_triggered(void)
{
	try
	{
		OpenListDialog dlg(m_ui.pTabWidget->tabNameList(), true, this);
		if (dlg.exec() == QDialog::Accepted)
			m_ui.pTabWidget->moveToTab(dlg.tabName());
	}
	catch (ExceptionPtr pException)
	{
		DetailedMessageBox::critical(tr("Could not move a record."), tr("Click 'Show Details...' for more information."), pException);
	}
}

void MainWindow::on_pExportAction_triggered(void)
{
	try
	{
		const QString data = m_ui.pTabWidget->getCurrentMenuCsv();
		if (!data.isEmpty())
		{
			const QString defFolder = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation);
			const QString filePath = QFileDialog::getSaveFileName(this, tr("Export Menu"), defFolder, tr("CSV Files (*.csv)"));
			if (!filePath.isEmpty())
			{
				QFile f(filePath);
				if (f.open(QIODevice::WriteOnly))
				{
					f.write(data.toLocal8Bit());
					f.close();
					QMessageBox::information(this, tr("Export"), tr("Menu export successfully completed!"));
				}
				else
					QMessageBox::warning(this, tr("Export"), tr("Menu export have failed!"));
			}
		}
	}
	catch (ExceptionPtr pException)
	{
		DetailedMessageBox::critical(tr("Could not export a menu."), tr("Click 'Show Details...' for more information."), pException);
	}
}

void MainWindow::on_pAboutAction_triggered(void)
{
	const QString text = tr("<html><strong>Kalorist</strong><br>Version: %1<br><br>An application for creating and managing menus.<br><br>Copyright © 2015-2017 EzSoftware <a href='https://ezsoftware.ru'>https://ezsoftware.ru</a>. All rights reserved!</html>").arg(QApplication::applicationVersion());
	QMessageBox::about(this, tr("About"), text);
}

void MainWindow::on_pAboutQtAction_triggered(void)
{
	QMessageBox::aboutQt(this);
}

void MainWindow::onDataChangeTriggered(void)
{
	const TabContentWidget *pWidget = m_ui.pTabWidget->currentContentWidget();
	m_ui.pStatusBar->showMessage(tr("Total %1 calories in %2 items").arg(pWidget->totalEnergy()).arg(pWidget->itemCount()));
}

void MainWindow::contextMenuEvent(QContextMenuEvent *pEvent)
{
	if (m_ui.pTabWidget->rect().contains(pEvent->pos()))
	{
		QMenu *pMenu = m_ui.pRecordMenu;
		pMenu->exec(pEvent->globalPos());
	}
}
