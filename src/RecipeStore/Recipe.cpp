#include "Recipe.h"

Recipe::Recipe()
{

}

QString Recipe::name() const
{
	return name_;
}

void Recipe::setName(const QString& value)
{
	name_ = value;
}

QString Recipe::amount() const
{
	return amount_;
}

void Recipe::setAmount(const QString& value)
{
	amount_ = value;
}

QString Recipe::type() const
{
	return type_;
}

void Recipe::setType(const QString& value)
{
	type_ = value;
}
