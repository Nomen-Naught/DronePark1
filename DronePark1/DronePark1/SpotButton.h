#pragma once
#include <QtWidgets/QPushButton>

class SpotButton : public QPushButton
{
	Q_OBJECT

public:
	SpotButton(QWidget *parent = 0);
	~SpotButton();

public slots:
	void updateTicketed(int id, bool value);
	void updateEmpty(int id, bool value);

private:
	bool currentTicketed;
	bool currentEmpty;

	void updateText();
};