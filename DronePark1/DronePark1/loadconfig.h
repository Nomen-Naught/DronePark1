#pragma once
#ifndef LOADCONFIG_H
#define LOADCONFIG_H

#include <QtWidgets/QDialog>
#include "ui_loadconfig.h"

class LoadConfig : public QDialog
{
	Q_OBJECT

public:
	LoadConfig(QWidget *parent = 0);
	~LoadConfig();

private:
	Ui::LoadConfig ui;
};

#endif // LOADCONFIG_H