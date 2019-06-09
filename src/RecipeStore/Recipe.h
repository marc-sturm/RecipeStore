#ifndef RECIPE_H
#define RECIPE_H

#include <QString>
#include <QList>
#include <QTextStream>


struct RecipeIngredient
{
	QString name;
	QString amount;
	QString unit;

	void toHTML(QTextStream& stream) const;
};

struct RecipePart
{
	QString name;

	QList<RecipeIngredient> ingredients;

	void toHTML(QTextStream& stream) const;
};

struct Recipe
{
	QString name;
	QString amount;
	QString type;

	QList<RecipePart> parts;

	QList<RecipeIngredient> ingredients;

	void toHTML(QTextStream& stream) const;
};

#endif // RECIPE_H
