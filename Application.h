#ifndef APPLICATION_H
#define APPLICATION_H
#include <QtWidgets>

class Application : public QApplication
{
	Q_OBJECT
public:
	Application(int argc, char *argv[]);

	int execute(void);
};

#endif // APPLICATION_H
