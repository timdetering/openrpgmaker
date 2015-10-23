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

#include "configureevent_changeselectedarea.h"
#include "menuobject.h"
#include "projectdata.h"
#include "projectdata_mainmenulayout.h"
#include "projectdata_popupmenulayout.h"
#include "projectdata_titlemenulayout.h"
#include "storagefile.h"

ConfigureEvent_ChangeSelectedArea::ConfigureEvent_ChangeSelectedArea(int menuID, int menuType, QWidget *parent) : QDialog(parent)
{
	QList<MenuObject*> menuObjects;
	setupUi(this);
	if (menuType == MenuObject::MENUTYPE_MAINMENU)
	{
		ProjectData::DataReference<MainMenuLayout> *layoutRef = new ProjectData::DataReference<MainMenuLayout>(menuID, __FILE__, __LINE__);
		menuObjects = (*layoutRef)->getObjectsByType(MenuObject::TYPE_SELECTABLEAREA) + (*layoutRef)->getObjectsByType(MenuObject::TYPE_ITEMLIST);
		for (int i = 0; i < menuObjects.size(); ++i)
			cboxMenuObject->addItem(menuObjects[i]->getName());
		menuObjects.clear();
		delete layoutRef;
	}
	else if (menuType == MenuObject::MENUTYPE_POPUPMENU)
	{
		ProjectData::DataReference<PopupMenuLayout> *layoutRef = new ProjectData::DataReference<PopupMenuLayout>(menuID, __FILE__, __LINE__);
		menuObjects = (*layoutRef)->getObjectsByType(MenuObject::TYPE_SELECTABLEAREA) + (*layoutRef)->getObjectsByType(MenuObject::TYPE_ITEMLIST);
		for (int i = 0; i < menuObjects.size(); ++i)
			cboxMenuObject->addItem(menuObjects[i]->getName());
		menuObjects.clear();
		delete layoutRef;
	}
	else if (menuType == MenuObject::MENUTYPE_TITLESCREENMENU)
	{
		TitleMenuLayout *layout = ProjectData::titleMenuLayout;
		menuObjects = layout->getObjectsByType(MenuObject::TYPE_SELECTABLEAREA) + layout->getObjectsByType(MenuObject::TYPE_ITEMLIST);
		for (int i = 0; i < menuObjects.size(); ++i)
			cboxMenuObject->addItem(menuObjects[i]->getName());
		menuObjects.clear();
		layout = NULL;
	}
	if (cboxMenuObject->count() == 0)
	{
		cboxMenuObject->addItem("No Selectable Areas or Item Lists To Select");
		buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
	}
	resize(width(), sizeHint().height());
	setMinimumWidth(width());
	setMaximumWidth(width());
	setMinimumHeight(height());
	setMaximumHeight(height());
}

ConfigureEvent_ChangeSelectedArea::~ConfigureEvent_ChangeSelectedArea()
{
}

void ConfigureEvent_ChangeSelectedArea::setMenuObject(QString object)
{
	int index = cboxMenuObject->findText(object);
	if (index != -1)
		cboxMenuObject->setCurrentIndex(index);
}

QString ConfigureEvent_ChangeSelectedArea::getMenuObject()
{
	return cboxMenuObject->currentText();
}
