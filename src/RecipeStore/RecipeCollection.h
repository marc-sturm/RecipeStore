#ifndef RECIPECOLLECTION_H
#define RECIPECOLLECTION_H

#include <QList>
#include <QString>
#include "Recipe.h"

//Recipe colection
class RecipeCollection
	: public QList<Recipe>
{
public:
	RecipeCollection();

	//Load recipe list from XML file
	static RecipeCollection loadFromXml(QString filename);
};

#endif // RECIPECOLLECTION_H
