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

public slots:
	void on_actionAbout_triggered(bool);
	void on_actionOpen_triggered(bool);
	void updateRecipeTree();

private:
	Ui::MainWindow ui_;
	RecipeCollection recipes_;
};
#endif // MainWindow_H
