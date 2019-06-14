#ifndef RECIPE_H
#define RECIPE_H

#include <QString>
#include <QList>
#include <QTextStream>

//Base class for recipe parts
class RecipePart
{
public:
	virtual ~RecipePart();
	virtual void toHTML(QTextStream& stream) const = 0;
};

class RecipeIngredient
	: public RecipePart
{
public:
	virtual ~RecipeIngredient();

	QString name;
	QString amount;
	QString unit;
	QString text;

	void toHTML(QTextStream& stream) const override;
};

class RecipeSection
	: public RecipePart
{
public:
	virtual ~RecipeSection();

	QString text;

	void toHTML(QTextStream& stream) const override;
};

class RecipeText
	: public RecipePart
{
public:
	virtual ~RecipeText();

	QString text;

	void toHTML(QTextStream& stream) const override;
};

class Recipe
{
public:
	QString name;
	QString amount;
	QString type;

	QList<RecipePart*> parts;

	void toHTML(QTextStream& stream, int recipe_nr=-1) const;
};

#endif // RECIPE_H
