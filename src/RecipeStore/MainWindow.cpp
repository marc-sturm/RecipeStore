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
	catch (FileParseException e)
	{
		QMessageBox::critical(this, "Error loading recipe collection", "File parse error in XML file:\n" + e.message() );
		return;
	}

	//update GUI
	updateRecipeTree();

	//set last collection for automatic re-load
	Settings::setString("last_collection", filename);
}

QString MainWindow::typesFile() const
{
	if (recipes_filename_.isEmpty()) THROW(ProgrammingException, "Cannot use typesFile() function when no recipe collection is loaded!");

	return QFileInfo(recipes_filename_).absolutePath() + QDir::separator() + "types.txt";
}

QString MainWindow::unitsFile() const
{
	if (recipes_filename_.isEmpty()) THROW(ProgrammingException, "Cannot use unitsFile() function when no recipe collection is loaded!");

	return QFileInfo(recipes_filename_).absolutePath() + QDir::separator() + "units.txt";
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

	editTextFile(typesFile(), "Edit recipe types");
}

void MainWindow::on_actionEditUnits_triggered(bool)
{
	if (recipes_filename_.isEmpty()) return;

	editTextFile(unitsFile(), "Edit units");
}

void MainWindow::updateRecipeTree()
{
	qDebug() << "TODO: " << __FILE__ << __LINE__;
}

void MainWindow::editTextFile(QString filename, QString title)
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
		entries.sort();
		Helper::storeTextFile(filename, entries);
	}
}

