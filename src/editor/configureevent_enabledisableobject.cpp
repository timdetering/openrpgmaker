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

#include "configureevent_enabledisableobject.h"
#include "projectdata.h"
#include "projectdata_mainmenulayout.h"
#include "projectdata_popupmenulayout.h"
#include "projectdata_titlemenulayout.h"

ConfigureEvent_EnableDisableObject::ConfigureEvent_EnableDisableObject(int menuID, int menuType, QWidget *parent) : QDialog(parent)
{
	QList<MenuObject*> menuObjects;
	QStringList objectNames;
	StorageFile storageFile;
	QString storageLocation;
	setupUi(this);
	if (menuType == MenuObject::MENUTYPE_MAINMENU)
	{
		ProjectData::DataReference<MainMenuLayout> *layoutRef = new ProjectData::DataReference<MainMenuLayout>(menuID, __FILE__, __LINE__);
		menuObjects = (*layoutRef)->getObjectsByType(MenuObject::TYPE_FRAME);
		menuObjects += (*layoutRef)->getObjectsByType(MenuObject::TYPE_TEXT);
		menuObjects += (*layoutRef)->getObjectsByType(MenuObject::TYPE_IMAGE);
		menuObjects += (*layoutRef)->getObjectsByType(MenuObject::TYPE_SELECTABLEAREA);
		menuObjects += (*layoutRef)->getObjectsByType(MenuObject::TYPE_SCROLLAREA);
		menuObjects += (*layoutRef)->getObjectsByType(MenuObject::TYPE_ITEMLIST);
		for (int i = 0; i < menuObjects.size(); ++i)
			objectNames.append(menuObjects[i]->getName());
		objectNames.sort();
		cboxMenuObject->addItems(objectNames);
		menuObjects.clear();
		delete layoutRef;
	}
	else if (menuType == MenuObject::MENUTYPE_POPUPMENU)
	{
		ProjectData::DataReference<PopupMenuLayout> *layoutRef = new ProjectData::DataReference<PopupMenuLayout>(menuID, __FILE__, __LINE__);
		menuObjects = (*layoutRef)->getObjectsByType(MenuObject::TYPE_FRAME);
		menuObjects += (*layoutRef)->getObjectsByType(MenuObject::TYPE_TEXT);
		menuObjects += (*layoutRef)->getObjectsByType(MenuObject::TYPE_IMAGE);
		menuObjects += (*layoutRef)->getObjectsByType(MenuObject::TYPE_SELECTABLEAREA);
		menuObjects += (*layoutRef)->getObjectsByType(MenuObject::TYPE_SCROLLAREA);
		menuObjects += (*layoutRef)->getObjectsByType(MenuObject::TYPE_ITEMLIST);
		for (int i = 0; i < menuObjects.size(); ++i)
			objectNames.append(menuObjects[i]->getName());
		objectNames.sort();
		cboxMenuObject->addItems(objectNames);
		menuObjects.clear();
		delete layoutRef;
	}
	else if (menuType == MenuObject::MENUTYPE_TITLESCREENMENU)
	{
		TitleMenuLayout *layout = ProjectData::titleMenuLayout;
		menuObjects = layout->getObjectsByType(MenuObject::TYPE_FRAME);
		menuObjects += layout->getObjectsByType(MenuObject::TYPE_TEXT);
		menuObjects += layout->getObjectsByType(MenuObject::TYPE_IMAGE);
		menuObjects += layout->getObjectsByType(MenuObject::TYPE_SELECTABLEAREA);
		menuObjects += layout->getObjectsByType(MenuObject::TYPE_SCROLLAREA);
		menuObjects += layout->getObjectsByType(MenuObject::TYPE_ITEMLIST);
		for (int i = 0; i < menuObjects.size(); ++i)
			objectNames.append(menuObjects[i]->getName());
		objectNames.sort();
		cboxMenuObject->addItems(objectNames);
		menuObjects.clear();
		layout = NULL;
	}
	if (cboxMenuObject->count() == 0)
	{
		cboxMenuObject->addItem("No Selectable Areas or Item Lists To Select");
		buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
	}
	resize(sizeHint().width(), sizeHint().height());
	setMinimumWidth(width());
	setMaximumWidth(width());
	setMinimumHeight(height());
	setMaximumHeight(height());
}

ConfigureEvent_EnableDisableObject::~ConfigureEvent_EnableDisableObject()
{
}

void ConfigureEvent_EnableDisableObject::setMenuObject(QString objectName)
{
	int index = cboxMenuObject->findText(objectName);
	if (index != -1)
		cboxMenuObject->setCurrentIndex(index);
}

void ConfigureEvent_EnableDisableObject::setEnable(bool enable)
{
	if (enable)
		rbEnable->setChecked(true);
	else
		rbDisable->setChecked(true);
}

QString ConfigureEvent_EnableDisableObject::getMenuObject()
{
	return cboxMenuObject->currentText();
}

bool ConfigureEvent_EnableDisableObject::enable()
{
	return rbEnable->isChecked();
}
