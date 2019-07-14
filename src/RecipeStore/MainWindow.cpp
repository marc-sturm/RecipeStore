#include "MainWindow.h"
#include "Settings.h"
#include "Exceptions.h"
#include "GUIHelper.h"
#include "Helper.h"
#include <QMessageBox>
#include <QFileDialog>
#include <QDebug>
#include <QPrinter>
#include <QPrintDialog>

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
	connect(ui_.search_panel, SIGNAL(searchTextChanged(QString)), this, SLOT(applySearchTerms(QString)));

	//load last collection if set
	QString last_collection = Settings::string("last_collection");
	if (last_collection!="")
	{
		loadRecipeCollection(last_collection);
	}
}

MainWindow::~MainWindow()
{
	if (!recipes_filename_.isEmpty())
	{
		recipes_.store(recipes_filename_);
	}
}

void MainWindow::loadRecipeCollection(QString filename)
{
	//load
	try
	{
		recipes_ = RecipeCollection::load(filename);
		recipes_.sort();
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
		foreach(const QSharedPointer<RecipePart>& part, recipe.parts)
		{
			const RecipeIngredient* ingr = dynamic_cast<const RecipeIngredient*>(part.data());
			if (ingr!=nullptr)
			{
				QString unit = ingr->unit;
				if (!valid_units.contains(unit))
				{
					invalid_units << unit;
				}
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

	QString file = QFileDialog::getOpenFileName(this, "Open recipe collection", dir, "Recipes (*.xml);;All files (*.*)");
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

void MainWindow::on_actionExportHTML_triggered(bool)
{
	//determine file name
	QFileInfo file_info(recipes_filename_);
	QString filename = file_info.absoluteFilePath();
	filename = filename.left(filename.size()-file_info.suffix().size()); //remove extension
	filename += "html";

	//open stream
	QFile file(filename);
	if (!file.open(QFile::WriteOnly | QFile::Truncate))
	{
		QMessageBox::critical(this, "HTML export error", "Could not open file for writing '" + filename + "'!");
		return;
	}
	QTextStream stream(&file);
	stream.setCodec("UTF-8");

	//add HTML header
	stream << "<html>\n";
	stream << "  <head>\n";
	stream << "    <title>Kochbuch</title>\n";
	stream << "    <meta charset='utf-8'/>\n";
	stream << "    <style>\n";
	stream << "      td {background-color: #E5E5E5; vertical-align: top;}>\n";
	stream << "    </style>\n";
	stream << "  </head>\n";

	//add script
	stream << "  <script language='javascript'>\n";
	stream << "    function toggle(i) {\n";
	stream << "      var ele = document.getElementById(\"toggle\" + i);\n";
	stream << "      var text = document.getElementById(\"displayText\");\n";
	stream << "      if(ele.style.display == \"block\") {\n";
	stream << "        ele.style.display = \"none\";\n";
	stream << "        text.innerHTML = \"show\";\n";
	stream << "      } else {\n";
	stream << "        ele.style.display = \"block\";\n";
	stream << "        text.innerHTML = \"hide\";\n";
	stream << "      }\n";
	stream << "    }\n";
	stream << "  </script>\n";

	//add body
	stream << "  <body style='margin:5;'>\n";
	foreach(QString type, types())
	{
		//add type
		stream << "	<font style='font-size: 1.2em; font-weight: bold;'>- " << type << " -</font>\n";
		stream << "	<br>\n";

		//add receipes of the current type
		for (int i=0; i<recipes_.count(); ++i)
		{
			const Recipe& recipe = recipes_[i];

			if (recipe.type==type)
			{
				recipe.toHTML(stream, i);
				stream << "  <br>\n";
			}
		}
		stream << "  <br>\n";
	}

	//close open tags
	stream << "  </body>\n";
	stream << "</html>\n";

	//show success message
	QMessageBox::information(this, "HTML export", "HTML export successfully written to:\n"+filename);
}

void MainWindow::on_actionPrint_triggered(bool)
{
	//check that recipe is selected
	if (ui_.browser->toPlainText().trimmed().isEmpty()) return;

	//create printer
	QPrinter printer(QPrinter::HighResolution);
	printer.setFullPage(true);

	//show dialog
	QPrintDialog dlg(&printer, this);
	if (dlg.exec() == QDialog::Accepted)
	{
		ui_.browser->print(&printer);
	}
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

void MainWindow::applySearchTerms(QString search_text)
{
	//expand
	ui_.recipe_selector->expandAll();

	//init
	QList<QTreeWidgetItem*> shown_items;
	QStringList search_terms = search_text.simplified().split(" ");

	//iterate through types
	for(int t=0; t<ui_.recipe_selector->topLevelItemCount(); ++t)
	{
		QTreeWidgetItem* type_item = ui_.recipe_selector->topLevelItem(t);

		//iterate through recipes
		for(int c=0; c<type_item->childCount(); ++c)
		{
			QTreeWidgetItem* recipe_item = type_item->child(c);
			int index = recipe_item->data(0, Qt::UserRole).toInt();
			const Recipe& recipe = recipes_[index];

			//check if recipe matches
			if (recipe.matchesSearchTerms(search_terms))
			{
				recipe_item->setHidden(false);
				shown_items << recipe_item;
			}
			else
			{
				recipe_item->setHidden(true);
			}
		}
	}

	//select first entry
	if (shown_items.isEmpty())
	{
		ui_.recipe_selector->clearSelection();
		ui_.browser->clear();
	}
	else
	{
		ui_.recipe_selector->setCurrentItem(shown_items[0]);
	}
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

	//nothing selected > do nothing
	if (current==nullptr) return;

	//recipe type selected > do nothing
	int index = current->data(0, Qt::UserRole).toInt();
	if (index==-1) return;

	QString output;
	QTextStream stream(&output);

	//show recipe
	const Recipe& recipe = recipes_[index];
	stream << "<html>\n";
	stream << "  <head>\n";
	stream << "  <meta charset='utf-8'/>\n";
	stream << "    <style>\n";
	stream << "      td {background-color: #E5E5E5; vertical-align: top;}>\n";
	stream << "    </style>\n";
	stream << "  </head>\n";
	stream << "  <body style='margin:5;'>\n";
	recipe.toHTML(stream);
	stream << "  </body>\n";
	stream << "</html>\n";
	ui_.browser->setText(output);
}

