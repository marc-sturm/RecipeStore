#include "RecipeEditor.h"
#include "GUIHelper.h"
#include "Helper.h"
#include "RecipeCollection.h"
#include <QDateTime>

RecipeEditor::RecipeEditor(QWidget *parent, const Recipe &recipe)
	: QDialog(parent)
	, ui_()
	, recipe_(recipe)
{
	ui_.setupUi(this);
	GUIHelper::styleSplitter(ui_.splitter);
	setWindowTitle("Edit '" + recipe.name + "'");
	setWindowState(Qt::WindowMaximized);

	//connect stuff
	connect(ui_.store_btn, SIGNAL(clicked()), this, SLOT(accept()));
	connect(ui_.editor, SIGNAL(textChanged()), this, SLOT(recipeTextChanged()));
	connect(&timer_, SIGNAL(timeout()), this, SLOT(convertXmlToHtml()));

	//set up timer
	timer_.setInterval(1000);
	timer_.setSingleShot(true);

	//load recipe XML
	QString xml;
	QTextStream stream(&xml);
	recipe_.toXML(stream, "");
	ui_.editor->setText(xml);
}

const Recipe& RecipeEditor::recipe() const
{
	return recipe_;
}

void RecipeEditor::recipeTextChanged()
{
	//disable store button until validation is performed.
	ui_.store_btn->setEnabled(false);

	//reset timer
	timer_.start();
}

void RecipeEditor::convertXmlToHtml()
{
	//check that XML is valid
	QString tmp_file = Helper::tempFileName(".xml");
	try
	{
		QString xml = RecipeCollection::xmlHeader() + ui_.editor->toPlainText() + RecipeCollection::xmlFooter();
		Helper::storeTextFile(tmp_file, xml.split("\n"));
		RecipeCollection::validate(tmp_file);
	}
	catch (Exception& e)
	{
		ui_.html->setHtml("<b>XML validation error:</b><br>\n" + e.message().replace("\n", "<br>"));
		return;
	}

	//valid > convert to HTML
	try
	{
		RecipeCollection collection;
		collection.load(tmp_file, false);
		QString html;
		QTextStream stream(&html);
		recipe_ = collection[0];
		recipe_.toHTML(stream);
		ui_.html->setHtml(RecipeCollection::htmlHeader() + html + RecipeCollection::htmlFooter());
		ui_.store_btn->setEnabled(true);
	}
	catch (Exception& e)
	{
		ui_.html->setHtml("<b>XML loading error:</b><br>\n" + e.message().replace("\n", "<br>"));
		return;
	}
}
