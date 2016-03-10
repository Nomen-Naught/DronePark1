#include "loadconfig.h"
#include <QListWidgetItem>

LoadConfig::LoadConfig(std::list<Config*>* configs, QWidget *parent) : QDialog(parent)
{
	ui.setupUi(this);

	QListWidgetItem* item;
	QString lotName;

	//We need to hook up our Spots with our database observers and the gui
	for (std::list<Config*>::const_iterator iterator = configs->begin(),
		end = configs->end();
		iterator != end;
		++iterator)
	{
		item = new QListWidgetItem();
		lotName = (*iterator)->getCurrentLot()->getCity() + ": " + (*iterator)->getCurrentLot()->getName();
		QVariant config_id((*iterator)->getId());

		item->setText(lotName);
		item->setData(Qt::UserRole, config_id);

		ui.configList->addItem(item);
	}

}

void LoadConfig::accept()
{
	int selectedId;
	QVariant data;

	//Bail if nothing is selected
	if (ui.configList->currentItem() == NULL)
	{
		return;
	}

	//Grab the id associated with the list object
	data = ui.configList->currentItem()->data(Qt::UserRole);
	selectedId = data.toInt();

	//Let main controller know that we have an ID to load!
	emit acceptedConfig(selectedId);

	//Call superclass accept
	QDialog::accept();

	return;
}

LoadConfig::~LoadConfig()
{

}