#ifndef RECIPE_H
#define RECIPE_H

#include <QString>
#include <QList>


struct RecipeIngredient
{
	QString name;
	QString amount;
	QString unit;
};

struct RecipePart
{
	QString name;

	QList<RecipeIngredient> ingredients;
};

struct Recipe
{
	QString name;
	QString amount;
	QString type;

	QList<RecipePart> parts;

	QList<RecipeIngredient> ingredients;
};

#endif // RECIPE_H
