#include "RecipeCollection.h"
#include "XmlHelper.h"
#include "Exceptions.h"
#include <QMessageBox>

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
		output << parseRecipe(recipe_nodes.at(i));
	}


	//sort recipes by name
	std::sort(output.begin(), output.end(), [](const Recipe& a, const Recipe& b) { return a.name < b.name; });

	return output;
}

Recipe RecipeCollection::parseRecipe(const QDomNode& node)
{
	Recipe output;

	//attributes
	output.name = node.attributes().namedItem("name").toAttr().value();
	output.amount = node.attributes().namedItem("amount").toAttr().value();
	output.type = node.attributes().namedItem("type").toAttr().value();

	//process children
	QDomNodeList child_nodes = node.childNodes();
	for (int i=0; i<child_nodes.count(); ++i)
	{
		QDomNode child = child_nodes.at(i);
		QDomNode::NodeType type = child.nodeType();
		QString name = child.nodeName();
		bool handled = false;

		//parts + ingredients
		if (type==QDomNode::ElementNode)
		{
			if (name=="part")
			{
				output.parts << parsePart(child);
				handled = true;
			}
			else if (name=="ingr")
			{
				output.ingredients << parseIngredient(child);
				handled = true;
			}
		}

		//text
		if (type==QDomNode::TextNode)
		{
			QDomText text = child.toText();
			//TODO
			handled = true;
		}

		//not handled > error
		if (!handled)
		{
			THROW(ProgrammingException, "Recipe child '" + name + "' in XML not handled!");
		}
	}

	return output;
}

RecipePart RecipeCollection::parsePart(const QDomNode& node)
{
	RecipePart output;

	//attributes
	output.name = node.attributes().namedItem("name").toAttr().value();

	//process children
	QDomNodeList child_nodes = node.childNodes();
	for (int i=0; i<child_nodes.count(); ++i)
	{
		QDomNode child = child_nodes.at(i);
		QDomNode::NodeType type = child.nodeType();
		QString name = child.nodeName();
		bool handled = false;

		//parts + ingredients
		if (type==QDomNode::ElementNode)
		{
			if (name=="ingr")
			{
				output.ingredients << parseIngredient(child);
				handled = true;
			}
		}

		//text
		if (type==QDomNode::TextNode)
		{
			QDomText text = child.toText();
			//TODO
			handled = true;
		}

		//not handled > error
		if (!handled)
		{
			THROW(ProgrammingException, "Part child '" + name + "' in XML not handled!");
		}
	}
	return output;
}

RecipeIngredient RecipeCollection::parseIngredient(const QDomNode& node)
{
	RecipeIngredient output;

	output.name = node.attributes().namedItem("name").toAttr().value();
	output.amount = node.attributes().namedItem("amount").toAttr().value();
	output.unit = node.attributes().namedItem("unit").toAttr().value();

	return output;
}
