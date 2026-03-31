#ifndef RECIPECOLLECTION_H
#define RECIPECOLLECTION_H

#include <QList>
#include <QDomDocument>
#include "Recipe.h"
#include "Helper.h"
#include "Settings.h"

//Recipe colection
class RecipeCollection
	: public QList<Recipe>
{
public:
	RecipeCollection();

	//Load recipes from XML file
	void load(QString filename, bool validate_file=true);
	//Store recipes to XML file
	void store(QString filename) const;

	//Sort by recipe name
	void sort();

	//Validates the recipe
	static void validate(QString filename);
	//Returns the valid recipe types
	static QStringList validTypes();
	//Returns the valid ingredient units
	static QStringList validUnits();

	//XML header/footer
	static QString xmlHeader() { return "<?xml version=\"1.0\" encoding=\"UTF-8\" ?>\n<recipecollection>\n"; }
	static QString xmlFooter() { return "</recipecollection>\n"; }
	//HTML header/footer
	static QString htmlHeader();
	static QString htmlFooter();

protected:
	static Recipe parseRecipe(const QDomNode& node);
	static QSharedPointer<RecipeSection> parseSection(const QDomNode& node);
	static QSharedPointer<RecipeIngredient> parseIngredient(const QDomNode& node);
	static QSharedPointer<RecipeText> parseText(const QDomNode& node);

};

#endif // RECIPECOLLECTION_H
