#ifndef RECIPE_H
#define RECIPE_H

#include <QString>

class Recipe
{
public:
	Recipe();

	QString name() const;
	void setName(const QString& value);

	QString amount() const;
	void setAmount(const QString& value);

	QString type() const;
	void setType(const QString& value);

protected:
	QString name_;
	QString amount_;
	QString type_;
};

#endif // RECIPE_H
