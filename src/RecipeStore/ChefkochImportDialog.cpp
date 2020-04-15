#include "ChefkochImportDialog.h"
#include <QMessageBox>
#include <QDebug>

ChefkochImportDialog::ChefkochImportDialog(QWidget* parent, QStringList types)
	: QDialog(parent)
	, ui_()
{
	ui_.setupUi(this);
	connect(ui_.ok_btn, &QPushButton::clicked, this, &ChefkochImportDialog::okButtonPressed);

	ui_.type->addItem("[select]");
	ui_.type->addItems(types);
	ui_.type->setCurrentIndex(0);
}

const Recipe& ChefkochImportDialog::recipe() const
{
	return recipe_;
}

void ChefkochImportDialog::okButtonPressed()
{
	//title
	QString title = ui_.title->text().trimmed();
	if (title.isEmpty())
	{
		QMessageBox::warning(this, "Invalid recipe", "Title must not be empty!");
		return;
	}
	recipe_.name = title;

	//amount
	QString amount = ui_.amount->text().trimmed();
	if (amount.isEmpty())
	{
		QMessageBox::warning(this, "Invalid recipe", "Amount must not be empty!");
		return;
	}
	recipe_.amount = amount;

	//type
	QString type = ui_.type->currentText().trimmed();
	if (type=="[select]")
	{
		QMessageBox::warning(this, "Invalid recipe", "Type must be selected!");
		return;
	}
	recipe_.type = type;

	//parse text
	QStringList lines = ui_.text->toPlainText().split("\n");
	int first_tab = -1;
	int last_tab = -1;
	int start_desc = -1;
	int end_desc = -1;
	for (int i=0; i<lines.count(); ++i)
	{
		if (lines[i].contains("\t"))
		{
			if (first_tab==-1) first_tab = i;
			else last_tab = i;
		}
		if (lines[i].trimmed()=="Zubereitung") start_desc = i+1;
		if (lines[i].trimmed()=="Rezept von") end_desc = i-1;
	}

	//ingredients and section
	for (int i=first_tab; i<=last_tab; ++i)
	{
		QString line = lines[i];
		if (line.contains("\t"))
		{
			QSharedPointer<RecipeIngredient> part(new RecipeIngredient());

			QStringList parts = line.split("\t");
			part->name = parts[1].trimmed();

			QStringList parts2 = parts[0].trimmed().split(" ");
			part->amount = parts2[0].trimmed();
			part->unit = parts2[1].trimmed();

			recipe_.parts << part;
		}
		else
		{
			 QSharedPointer<RecipeSection> part(new RecipeSection());

			 part->text = line.trimmed();

			 recipe_.parts << part;
		}
	}

	//description
	QStringList desc;
	for (int i=start_desc; i<=end_desc; ++i)
	{
		desc << lines[i].trimmed();
	}
	QSharedPointer<RecipeText> part(new RecipeText());
	part->text = desc.join("\n");
	recipe_.parts << part;


	//accept and close dialog
	accept();
}
