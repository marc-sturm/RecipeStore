#ifndef MainWindow_H
#define MainWindow_H

#include "ui_MainWindow.h"
#include <QMainWindow>
#include "RecipeCollection.h"


class MainWindow
		: public QMainWindow
{
	Q_OBJECT

public:
	MainWindow(QWidget* parent = 0);
	void loadRecipeCollection(QString filename);
	QString typesFile() const;
	QStringList types() const;
	QString unitsFile() const;
	QStringList units() const;

public slots:
	void on_actionAbout_triggered(bool);
	void on_actionOpen_triggered(bool);
	void on_actionEditTypes_triggered(bool);
	void on_actionEditUnits_triggered(bool);
	void updateRecipeTree();
	void editTextFile(QString filename, QString title, bool sort);
	void selectedRecipeChanged(QTreeWidgetItem* current ,QTreeWidgetItem* previous);

private:
	Ui::MainWindow ui_;
	QString recipes_filename_;
	RecipeCollection recipes_;
};
#endif // MainWindow_H
