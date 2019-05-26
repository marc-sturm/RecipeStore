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
	//check XML is valid
	QString xml_errors = XmlHelper::isValidXml(filename, ":/Resources/receipes.xsd");
	if (!xml_errors.isEmpty())
	{
		THROW(FileParseException, xml_errors);
	}

	//load
	qDebug() << "TODO: " << __FILE__ << __LINE__;

	return RecipeCollection();
}
