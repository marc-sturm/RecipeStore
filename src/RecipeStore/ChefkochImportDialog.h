#ifndef CHEFKOCHIMPORTDIALOG_H
#define CHEFKOCHIMPORTDIALOG_H

#include <QDialog>
#include "ui_ChefkochImportDialog.h"
#include "Recipe.h"

class ChefkochImportDialog
	: public QDialog
{
	Q_OBJECT

public:
	ChefkochImportDialog(QWidget *parent, QStringList types);
	const Recipe& recipe() const;

private slots:
	void okButtonPressed();

private:
	Ui::ChefkochImportDialog ui_;
	Recipe recipe_;
};

#endif // CHEFKOCHIMPORTDIALOG_H
