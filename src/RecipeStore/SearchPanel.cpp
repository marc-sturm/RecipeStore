#include "SearchPanel.h"
#include <QDebug>

SearchPanel::SearchPanel(QWidget *parent) :
	QWidget(parent),
	ui_()
{
	ui_.setupUi(this);
	connect(ui_.recipe_text, SIGNAL(textChanged(QString)), this, SLOT(textChanged(QString)));
	connect(&timer_, SIGNAL(timeout()), this, SLOT(textChangeDone()));

	timer_.setSingleShot(true);
	timer_.setInterval(500);
}

void SearchPanel::textChanged(QString text)
{
	search_text_ = text;
	timer_.start();
}

void SearchPanel::textChangeDone()
{
	emit searchTextChanged(search_text_);
}
