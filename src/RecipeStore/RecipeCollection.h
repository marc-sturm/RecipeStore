#ifndef RECIPECOLLECTION_H
#define RECIPECOLLECTION_H

#include <QList>
#include <QDomDocument>
#include "Recipe.h"

//Recipe colection
class RecipeCollection
	: public QList<Recipe>
{
public:
	RecipeCollection();

	//Load recipes from XML file
	static RecipeCollection load(QString filename);
	//Store recipes to XML file
	void store(QString filename) const;

	//Sort by recipe name
	void sort();

protected:
	static Recipe parseRecipe(const QDomNode& node);
	static QSharedPointer<RecipeSection> parseSection(const QDomNode& node);
	static QSharedPointer<RecipeIngredient> parseIngredient(const QDomNode& node);
	static QSharedPointer<RecipeText> parseText(const QDomNode& node);
};

#endif // RECIPECOLLECTION_H
