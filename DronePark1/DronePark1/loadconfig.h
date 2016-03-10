#pragma once
#ifndef LOADCONFIG_H
#define LOADCONFIG_H

#include <QtWidgets/QDialog>
#include "ui_loadconfig.h"
#include "DatabaseController.h"

class LoadConfig : public QDialog
{
	Q_OBJECT

public:
	LoadConfig(std::list<Config*>*, QWidget *parent = 0);
	~LoadConfig();

	void accept();

signals:
	void acceptedConfig(int id);

private:
	Ui::LoadConfig ui;

};

#endif // LOADCONFIG_H