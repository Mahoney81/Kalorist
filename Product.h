#ifndef PRODUCT_H
#define PRODUCT_H
#include <QtCore>

class Product
{
public:
	QString name;
	int energy100g = 0;
	int totalWeight = 0;
	int portionsCount = 1;
	int portionWeight = 0;

	bool isConsistent(void) const { return totalWeight / qMax(portionsCount, 1) == portionWeight; }
};

using ProductPtr = QSharedPointer<Product>;

#endif // PRODUCT_H
