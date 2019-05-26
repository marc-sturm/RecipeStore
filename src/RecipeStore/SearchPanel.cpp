#include "SearchPanel.h"
#include "ui_SearchPanel.h"

SearchPanel::SearchPanel(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::SearchPanel)
{
	ui->setupUi(this);
}

SearchPanel::~SearchPanel()
{
	delete ui;
}
