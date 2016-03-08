#pragma once
#include <QLabel>

class StatusLabel : public QLabel
{
	Q_OBJECT

public:
	StatusLabel(QWidget *parent = 0);
	~StatusLabel();

public slots:
	void updateStatus(bool);
};