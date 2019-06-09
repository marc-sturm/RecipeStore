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

	//Load recipe list from XML file
	static RecipeCollection loadFromXml(QString filename);

protected:
	static Recipe parseRecipe(const QDomNode& node);
	static RecipePart parsePart(const QDomNode& node);
	static RecipeIngredient parseIngredient(const QDomNode& node);
};

#endif // RECIPECOLLECTION_H
