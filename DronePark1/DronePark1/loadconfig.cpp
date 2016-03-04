#include "loadconfig.h""

LoadConfig::LoadConfig(QWidget *parent) : QDialog(parent)
{
	ui.setupUi(this);

	ui.configList->addItem("hello");
	ui.configList->addItem("goodbye");
}

LoadConfig::~LoadConfig()
{

}