#include "stdafx.h"
#include "Application.h"
#include "MainWindow.h"

Application::Application(int argc, char* argv[]) : QApplication(argc, argv)
{
	setApplicationName("Kalorist");
	//setApplicationDisplayName(QObject::tr("Kalorist"));
	setApplicationVersion("0.4.1");
	setOrganizationName("EzSoftware");
	setOrganizationDomain("ezsoftware.ru");
	setAttribute(Qt::AA_UseHighDpiPixmaps);
	setWindowIcon(QIcon(":/icons/bananas-icon"));

	static QTranslator qtTranslator;
	qtTranslator.load(":/translations/qtbase_" + QLocale::system().name());
	if(!installTranslator(&qtTranslator))
		qDebug() << "Could not install Qt translator";

	static QTranslator appTranslator;
	appTranslator.load(":/translations/kalorist_" + QLocale::system().name());
	if(!installTranslator(&appTranslator))
		qDebug() << "Could not install app translator";
}

int Application::execute(void)
{
	try
	{
		MainWindow wnd;
		wnd.show();

		return exec();
	}
	catch(ExceptionPtr pException)
	{
		DetailedMessageBox::critical(tr("Oops... The application has crashed."),
							 tr("Something went wrong... Restart the app. If the problem persists, please contact the developer.<br><br>Click 'Show Details...' for more information."),
							 pException);
		return 1;
	}
}
