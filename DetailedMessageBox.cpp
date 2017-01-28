#include "stdafx.h"
#include "DetailedMessageBox.h"

namespace DetailedMessageBox
{
void critical(const QString& text, const QString& informativeText, ExceptionPtr pException)
{
	QMessageBox msgBox;
	msgBox.setIcon(QMessageBox::Critical);
	msgBox.setText(text);
	if(!informativeText.isEmpty())
		msgBox.setInformativeText(informativeText);
	if(pException)
	{
		QString errorText = pException->what().trimmed();
		if (errorText.isEmpty())
			errorText = QObject::tr("Unknown error occurred!");
		msgBox.setDetailedText(errorText);
	}
	msgBox.setStandardButtons(QMessageBox::Ok);
	msgBox.exec();
}
}
