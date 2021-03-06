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
	~MainWindow();
	void loadRecipeCollection(QString filename);

public slots:
	void on_actionAbout_triggered(bool);
	void on_actionOpen_triggered(bool);
	void on_actionEditTypes_triggered(bool);
	void on_actionEditUnits_triggered(bool);
	void on_actionExportHTML_triggered(bool);
	void on_actionPrint_triggered(bool);
	void on_actionImportChefkoch_triggered(bool);
	void updateRecipeTree();
	void applySearchTerms(QString search_text);
	void editTextFile(QString filename, QString title, bool sort);
	void selectedRecipeChanged(QTreeWidgetItem* current ,QTreeWidgetItem* previous);

private:
	Ui::MainWindow ui_;
	QString recipes_filename_;
	RecipeCollection recipes_;
	int recipe_selected_;
};
#endif // MainWindow_H
