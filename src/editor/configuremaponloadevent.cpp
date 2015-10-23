/****************************************************************************
 *  Open RPG Maker is free software: you can redistribute it and/or modify  *
 *  it under the terms of the GNU General Public License as published by    *
 *  the Free Software Foundation, either version 3 of the License, or       *
 *  (at your option) any later version.                                     *
 *                                                                          *
 *  Open RPG Maker is distributed in the hope that it will be useful,       *
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of          *
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the           *
 *  GNU General Public License for more details.                            *
 *                                                                          *
 *  You should have received a copy of the GNU General Public License       *
 *  along with Open RPG Maker. If not, see <http://www.gnu.org/licenses/>.  *
 *                                                                          *
 *  ---                                                                     *
 *  Copyright (C) 2012, Justin Davis <tuxdavis@gmail.com>                   *
 ****************************************************************************/

#include "configuremaponloadevent.h"
#include "projectdata_baseevent.h"

ConfigureMapOnLoadEvent::ConfigureMapOnLoadEvent(int mapID, QWidget *parent) : QDialog(parent)
{
	setupUi(this);
	mapRef = new ProjectData::DataReference<Map>(mapID, __FILE__, __LINE__);
	mapRef->saveToStorage();
	mapRef->backupStorage();
	lwEventCommands->setupView(mapID, 0, (*mapRef)->getOnLoadEvent());
}

ConfigureMapOnLoadEvent::~ConfigureMapOnLoadEvent()
{
	if (mapRef != NULL)
	{
		delete mapRef;
		mapRef = NULL;
	}
}

void ConfigureMapOnLoadEvent::on_buttonBox_clicked(QAbstractButton *button)
{
	int buttonClicked = buttonBox->standardButton(button);
	if (buttonClicked == QDialogButtonBox::Ok)
		mapRef->deleteBackupStorage();
	else if (buttonClicked == QDialogButtonBox::Apply)
		mapRef->backupStorage();
	else if (buttonClicked == QDialogButtonBox::Cancel)
		mapRef->reloadBackupStorage();
}
