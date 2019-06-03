#include "RecipeCollection.h"
#include "XmlHelper.h"
#include "Exceptions.h"
#include <QMessageBox>
#include <QDomDocument>

RecipeCollection::RecipeCollection()
	: QList<Recipe>()
{
}

RecipeCollection RecipeCollection::loadFromXml(QString filename)
{
	RecipeCollection output;

	//check XML is valid
	QString xml_errors = XmlHelper::isValidXml(filename, ":/Resources/receipes.xsd");
	if (!xml_errors.isEmpty())
	{
		THROW(FileParseException, xml_errors);
	}

	//load XML
	QFile file(filename);
	if (!file.open(QIODevice::ReadOnly ))
	{
		THROW(FileParseException, "Could not open file '" + filename + "'!");
	}
	QDomDocument dom;
	dom.setContent(&file);
	file.close();


	//parse XML
	QDomElement root = dom.documentElement();
	QDomNodeList recipe_nodes = root.elementsByTagName("recipe");
	for (int i=0; i<recipe_nodes.count(); ++i)
	{
		QDomNode recipe_node = recipe_nodes.at(i);

		Recipe recipe;
		//attributes
		recipe.setName(recipe_node.attributes().namedItem("name").toAttr().value());
		recipe.setAmount(recipe_node.attributes().namedItem("amount").toAttr().value());
		recipe.setType(recipe_node.attributes().namedItem("type").toAttr().value());
		//parts
		//TODO
		//ingredients
		//TODO
		output << recipe;
	}

	return output;
}
