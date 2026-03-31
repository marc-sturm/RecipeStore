#ifndef RECIPEEDITOR_H
#define RECIPEEDITOR_H

#include <QDialog>
#include <QTimer>
#include "ui_RecipeEditor.h"
#include "Recipe.h"

class RecipeEditor : public QDialog
{
	Q_OBJECT

public:
	RecipeEditor(QWidget* parent, const Recipe& recipe);
	const Recipe& recipe() const;

private slots:
	void recipeTextChanged();
	void convertXmlToHtml();
private:
	Ui::RecipeEditor ui_;
	Recipe recipe_;
	QTimer timer_;
};

#endif // RECIPEEDITOR_H
