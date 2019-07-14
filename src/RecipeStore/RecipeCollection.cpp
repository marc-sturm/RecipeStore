#include "RecipeCollection.h"
#include "Helper.h"
#include "XmlHelper.h"
#include "Exceptions.h"
#include <QMessageBox>

RecipeCollection::RecipeCollection()
	: QList<Recipe>()
{
}

RecipeCollection RecipeCollection::load(QString filename)
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

	return output;
}

void RecipeCollection::store(QString filename) const
{
	//store
	auto file = Helper::openFileForWriting(filename);
	QTextStream stream(file.data());
	stream.setCodec("UTF-8");

	stream << "<?xml version=\"1.0\" encoding=\"UTF-8\" ?>\n";
	stream << "<recipecollection>\n";
	foreach(const Recipe& recipe, *this)
	{
		recipe.toXML(stream);
	}
	stream << "</recipecollection>\n";

	file->close();

	//check that it's well-formed
	QString xml_errors = XmlHelper::isValidXml(filename, ":/Resources/receipes.xsd");
	if (!xml_errors.isEmpty())
	{
		THROW(FileParseException, xml_errors);
	}
}

void RecipeCollection::sort()
{
	std::sort(begin(), end(), [](const Recipe& a, const Recipe& b)
	{
		return a.name < b.name;
	});
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
			if (name=="section")
			{
				output.parts << parseSection(child);
				handled = true;
			}
			else if (name=="ingr")
			{
				output.parts << parseIngredient(child);
				handled = true;
			}
			else if (name=="text")
			{
				output.parts << parseText(child);
				handled = true;
			}
		}

		//not handled > error
		if (!handled)
		{
			THROW(ProgrammingException, "Recipe child '" + name + "' in XML not handled!");
		}
	}

	return output;
}

QSharedPointer<RecipeSection> RecipeCollection::parseSection(const QDomNode& node)
{
	QSharedPointer<RecipeSection> output(new RecipeSection());

	output->text = node.toElement().text();

	return output;
}

QSharedPointer<RecipeIngredient> RecipeCollection::parseIngredient(const QDomNode& node)
{
	QSharedPointer<RecipeIngredient> output(new RecipeIngredient());

	output->name = node.attributes().namedItem("name").toAttr().value();
	output->amount = node.attributes().namedItem("amount").toAttr().value();
	output->unit = node.attributes().namedItem("unit").toAttr().value();
	output->text = node.toElement().text();

	return output;
}

QSharedPointer<RecipeText> RecipeCollection::parseText(const QDomNode& node)
{
	QSharedPointer<RecipeText> output(new RecipeText());

	output->text = node.toElement().text();

	return output;
}
