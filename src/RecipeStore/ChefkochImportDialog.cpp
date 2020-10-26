#include "ChefkochImportDialog.h"
#include "Exceptions.h"
#include "RecipeCollection.h"
#include "Helper.h"
#include <QMessageBox>
#include <QDebug>

ChefkochImportDialog::ChefkochImportDialog(QWidget* parent)
	: QDialog(parent)
	, ui_()
{
	ui_.setupUi(this);
	connect(ui_.ok_btn, &QPushButton::clicked, this, &ChefkochImportDialog::okButtonPressed);

	ui_.type->addItem("[select]");
	ui_.type->addItems(RecipeCollection::validTypes());
	ui_.type->setCurrentIndex(0);
}

const Recipe& ChefkochImportDialog::recipe() const
{
	return recipe_;
}

void ChefkochImportDialog::okButtonPressed()
{
	try
	{
		//title
		QString title = ui_.title->text().trimmed();
		if (title.isEmpty()) THROW(ArgumentException, "Title must not be empty!");
		recipe_.name = title;

		//amount
		QString amount = ui_.amount->text().trimmed();
		if (amount.isEmpty()) THROW(ArgumentException, "Amount must not be empty!");
		recipe_.amount = amount;

		//type
		QString type = ui_.type->currentText().trimmed();
		if (type=="[select]") THROW(ArgumentException, "Type must be selected!");
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
				QString amount = parts2[0].trimmed();
				if (amount=="½") amount = "0.5";
				if (amount=="¼") amount = "0.25";
				if (amount=="¾") amount = "0.75";
				part->amount = amount;
				if (parts2.count()==2)
				{
					part->unit = parts2[1].trimmed();
				}

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

		//validate
		QString tmp_file = Helper::tempFileName(".xml");
		RecipeCollection tmp_collection;
		tmp_collection << recipe_;
		tmp_collection.store(tmp_file);
		RecipeCollection::validate(tmp_file);
	}
	catch(Exception& e)
	{
		QMessageBox::warning(this, "Invalid recipe", e.message());
		return;
	}

	//accept and close dialog
	accept();
}
