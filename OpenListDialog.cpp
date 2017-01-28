#include "stdafx.h"
#include "OpenListDialog.h"

OpenListDialog::OpenListDialog(const QStringList& openedLists, const bool isOnlyOpened, QWidget *pParent) : QDialog(pParent)
{
	m_ui.setupUi(this);

	if (!isOnlyOpened)
	{
		const QString folderPath = QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation);
		const QDir dir(folderPath);
		const QFileInfoList fileInfoList = dir.entryInfoList({ "*.xml" });
		for (const QFileInfo& fi : fileInfoList)
		{
			const QString name = fi.baseName();
			if (!openedLists.contains(name))
				m_ui.pListWidget->addItem(QString(name).replace("_", " "));
		}
	}
	else
	{
		setWindowTitle(tr("Select Destination Menu"));
		for (const QString name : openedLists)
			m_ui.pListWidget->addItem(QString(name).replace("_", " "));
	}
	m_ui.pListWidget->sortItems();
}

void OpenListDialog::on_pButtonBox_accepted(void)
{
	const QListWidgetItem *pItem = m_ui.pListWidget->currentItem();
	if (pItem)
	{
		m_tabName = pItem->text();
		accept();
	}
}
