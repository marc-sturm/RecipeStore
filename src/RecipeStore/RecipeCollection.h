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
	static QSharedPointer<RecipeSection> parseSection(const QDomNode& node);
	static QSharedPointer<RecipeIngredient> parseIngredient(const QDomNode& node);
	static QSharedPointer<RecipeText> parseText(const QDomNode& node);
};

#endif // RECIPECOLLECTION_H
