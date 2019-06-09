#include "MainWindow.h"
#include "Settings.h"
#include "Exceptions.h"
#include "GUIHelper.h"
#include "Helper.h"
#include <QMessageBox>
#include <QFileDialog>
#include <QDebug>

MainWindow::MainWindow(QWidget* parent)
	: QMainWindow(parent)
	, ui_()
	, recipes_filename_()
	, recipes_()
{
	ui_.setupUi(this);
	setWindowTitle(QApplication::applicationName());
	setWindowState(Qt::WindowMaximized);
	connect(ui_.recipe_selector, SIGNAL(currentItemChanged(QTreeWidgetItem*,QTreeWidgetItem*)), this, SLOT(selectedRecipeChanged(QTreeWidgetItem*,QTreeWidgetItem*)));

	//load last collection if set
	QString last_collection = Settings::string("last_collection");
	if (last_collection!="")
	{
		loadRecipeCollection(last_collection);
	}
}

void MainWindow::loadRecipeCollection(QString filename)
{
	//load
	try
	{
		recipes_ = RecipeCollection::loadFromXml(filename);
		recipes_filename_ = filename;
	}
	catch (Exception& e)
	{
		QMessageBox::critical(this, "Error loading recipe collection", "File parse error in XML file:\n" + e.message() );
		return;
	}

	//update GUI
	updateRecipeTree();

	//set last collection for automatic re-load
	Settings::setString("last_collection", filename);

	//check that all types are valid
	QStringList invalid_types;
	QStringList valid_types = types();
	foreach(const Recipe& recipe, recipes_)
	{
		QString type = recipe.type;
		if (!valid_types.contains(type))
		{
			qDebug() << recipe.name << recipe.type;
			invalid_types << type;
		}
	}
	invalid_types.removeDuplicates();
	if (!invalid_types.isEmpty())
	{
		QMessageBox::warning(this, "Invalid types", "Invalid recipe types found:\n" + invalid_types.join("\n"));
	}

	//check that add units are valid
	QStringList invalid_units;
	QStringList valid_units = units();
	foreach(const Recipe& recipe, recipes_)
	{
		foreach(const RecipePart& part, recipe.parts)
		{
			foreach(const RecipeIngredient& ingr, part.ingredients)
			{
				QString unit = ingr.unit;
				if (!valid_units.contains(unit))
				{
					invalid_units << unit;
				}
			}
		}

		foreach(const RecipeIngredient& ingr, recipe.ingredients)
		{
			QString unit = ingr.unit;
			if (!valid_units.contains(unit))
			{
				invalid_units << unit;
			}
		}
	}
	invalid_units.removeDuplicates();
	invalid_units.removeAll("");
	if (!invalid_units.isEmpty())
	{
		QMessageBox::warning(this, "Invalid units", "Invalid igredient units found:\n" + invalid_types.join("\n"));
	}
}

QString MainWindow::typesFile() const
{
	if (recipes_filename_.isEmpty()) THROW(ProgrammingException, "Cannot use typesFile() function when no recipe collection is loaded!");

	return QFileInfo(recipes_filename_).absolutePath() + QDir::separator() + "types.txt";
}

QStringList MainWindow::types() const
{
	return Helper::loadTextFile(typesFile(), true, '#', true);
}

QString MainWindow::unitsFile() const
{
	if (recipes_filename_.isEmpty()) THROW(ProgrammingException, "Cannot use unitsFile() function when no recipe collection is loaded!");

	return QFileInfo(recipes_filename_).absolutePath() + QDir::separator() + "units.txt";
}

QStringList MainWindow::units() const
{
	return Helper::loadTextFile(unitsFile(), true, '#', true);
}

void MainWindow::on_actionAbout_triggered(bool)
{
	QMessageBox::about(this, "About " + QApplication::applicationName(), QApplication::applicationName() + " " + QApplication::applicationVersion() +"\n\nThis program is free software.\n\nThis program is provided as is with no warranty of any kind, including the warranty of design, merchantability and fitness for a particular purpose.");
}

void MainWindow::on_actionOpen_triggered(bool)
{
	//determine dir
	QString dir = QApplication::applicationDirPath();
	QString last_collection = Settings::string("last_collection");
	if (QFile::exists(last_collection))
	{
		dir = QFileInfo(last_collection).absolutePath();
	}

	QString file = QFileDialog::getOpenFileName(this, "Open recipe collection", dir, "Recipes (*.xml)");
	if (file=="") return;

	loadRecipeCollection(file);
}

void MainWindow::on_actionEditTypes_triggered(bool)
{
	if (recipes_filename_.isEmpty()) return;

	editTextFile(typesFile(), "Edit recipe types", false);
}

void MainWindow::on_actionEditUnits_triggered(bool)
{
	if (recipes_filename_.isEmpty()) return;

	editTextFile(unitsFile(), "Edit units", true);
}

void MainWindow::updateRecipeTree()
{
	ui_.recipe_selector->clear();

	//add type sections
	foreach(QString type, types())
	{
		QTreeWidgetItem* section_item = new QTreeWidgetItem();
		section_item->setText(0, type);
		section_item->setData(0, Qt::UserRole, -1);
		ui_.recipe_selector->addTopLevelItem(section_item);

		//add receipes of the current type
		for (int i=0; i<recipes_.count(); ++i)
		{
			const Recipe& recipe = recipes_[i];

			if (recipe.type==type)
			{
				QTreeWidgetItem* item = new QTreeWidgetItem(section_item);
				item->setText(0, recipe.name);
				item->setData(0, Qt::UserRole, i);
			}
		}
	}

	//expand
	ui_.recipe_selector->expandAll();
}

void MainWindow::editTextFile(QString filename, QString title, bool sort)
{
	//load file
	QStringList entries = Helper::loadTextFile(filename, true, '#', true);

	QTextEdit* widget = new QTextEdit(this);
	widget->setText(entries.join("\n"));
	auto dlg = GUIHelper::createDialog(widget, title, "", true);
	if (dlg->exec()==QDialog::Accepted)
	{
		entries = widget->toPlainText().split("\n");
		for(int i=0; i<entries.count(); ++i)
		{
			entries[i] = entries[i].trimmed();
		}
		entries.removeAll("");
		if (sort) entries.sort();
		Helper::storeTextFile(filename, entries);
	}
}

void MainWindow::selectedRecipeChanged(QTreeWidgetItem* current, QTreeWidgetItem* /*previous*/)
{
	//clear browser
	ui_.browser->clear();

	//recipe type selected > do nothing
	int index = current->data(0, Qt::UserRole).toInt();
	if (index==-1)
	{
		return;
	}


	QString output;
	QTextStream stream(&output);

	//show recipe
	const Recipe& recipe = recipes_[index];
	stream << "<html>\n";
	stream << "  <head>\n";
	stream << "    <style>\n";
	stream << "      td {background-color: #E5E5E5; vertical-align: top;}>\n";
	stream << "    </style>\n";
	stream << "  </head>\n";
	stream << "  <body>\n";
	recipe.toHTML(stream);
	stream << "  </body>\n";
	stream << "</html>\n";
	ui_.browser->setText(output);
}

