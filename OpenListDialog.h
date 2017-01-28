#ifndef OPENLISTDIALOG_H
#define OPENLISTDIALOG_H

#include <QDialog>
#include "ui_OpenListDialog.h"

class OpenListDialog : public QDialog
{
	Q_OBJECT

public:
	OpenListDialog(const QStringList& openedLists, const bool isOnlyOpened, QWidget *pParent = 0);

	const QString& tabName(void) const { return m_tabName; }

private slots:
	void on_pButtonBox_accepted(void);
	void on_pButtonBox_rejected(void) { reject(); }

private:
	Ui::OpenListDialog m_ui;
	QString m_tabName;
};

#endif // OPENLISTDIALOG_H
