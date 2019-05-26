#ifndef SEARCHPANEL_H
#define SEARCHPANEL_H

#include <QWidget>

namespace Ui {
class SearchPanel;
}

class SearchPanel : public QWidget
{
	Q_OBJECT

public:
	explicit SearchPanel(QWidget *parent = 0);
	~SearchPanel();

private:
	Ui::SearchPanel *ui;
};

#endif // SEARCHPANEL_H
