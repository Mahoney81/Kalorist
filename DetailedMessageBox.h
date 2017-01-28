#ifndef MESSAGEBOX_H
#define MESSAGEBOX_H
#include <QtWidgets>
#include "Exception.h"

namespace DetailedMessageBox
{
void critical(const QString& text, const QString& informativeText = {}, ExceptionPtr pException = {});
}

#endif // MESSAGEBOX_H
