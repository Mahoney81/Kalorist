#ifndef EXCEPTION_H
#define EXCEPTION_H

#include <QtCore>

class Exception
{
public:
	explicit Exception(const QString& message) : m_message(message) { }
	virtual ~Exception(void) {}

	virtual const QString& what(void) { return m_message; }

private:
	const QString m_message;
};

using ExceptionPtr = QSharedPointer<Exception>;

#endif // EXCEPTION_H
