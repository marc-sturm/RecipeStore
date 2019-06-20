#ifndef SEARCHPANEL_H
#define SEARCHPANEL_H

#include <QWidget>
#include <QTimer>
#include "ui_SearchPanel.h"

namespace Ui {
class SearchPanel;
}

class SearchPanel
	: public QWidget
{
	Q_OBJECT

public:
	SearchPanel(QWidget *parent = 0);

signals:
	void searchTextChanged(QString text);

protected slots:
	void textChanged(QString text);
	void textChangeDone();

private:
	Ui::SearchPanel ui_;
	QTimer timer_;
	QString search_text_;
};

#endif // SEARCHPANEL_H
