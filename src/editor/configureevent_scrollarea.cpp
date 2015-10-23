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

#include "configureevent_scrollarea.h"
#include "menuobject.h"
#include "menuscrollarea.h"
#include "projectdata.h"
#include "projectdata_mainmenulayout.h"
#include "projectdata_popupmenulayout.h"
#include "projectdata_titlemenulayout.h"
#include "variableselector.h"

ConfigureEvent_ScrollArea::ConfigureEvent_ScrollArea(int menu, int type, QWidget *parent) : QDialog(parent)
{
	QList<MenuObject*> menuObjects;
	QStringList menuObjectNames;
	menuID = menu;
	menuType = type;
	setupUi(this);
	resize(sizeHint().width(), sizeHint().height());
	setMinimumWidth(width());
	setMaximumWidth(width());
	setMinimumHeight(height());
	setMaximumHeight(height());
	mainMenuRef = NULL;
	popupMenuRef = NULL;
	if (menuType == MenuObject::MENUTYPE_MAINMENU)
	{
		mainMenuRef = new ProjectData::DataReference<MainMenuLayout>(menu, __FILE__, __LINE__);
		menuObjects = (*mainMenuRef)->getObjectsByType(MenuObject::TYPE_SCROLLAREA);
		for (int i = 0; i < menuObjects.size(); ++i)
			menuObjectNames.append(menuObjects[i]->getName());
		menuObjects.clear();
		menuObjectNames.sort();
		cboxScrollArea->addItems(menuObjectNames);
	}
	else if (menuType == MenuObject::MENUTYPE_POPUPMENU)
	{
		popupMenuRef = new ProjectData::DataReference<PopupMenuLayout>(menu, __FILE__, __LINE__);
		menuObjects = (*popupMenuRef)->getObjectsByType(MenuObject::TYPE_SCROLLAREA);
		for (int i = 0; i < menuObjects.size(); ++i)
			menuObjectNames.append(menuObjects[i]->getName());
		menuObjects.clear();
		menuObjectNames.sort();
		cboxScrollArea->addItems(menuObjectNames);
	}
	else if (menuType == MenuObject::MENUTYPE_TITLESCREENMENU)
	{
		titleMenuLayout = ProjectData::titleMenuLayout;
		menuObjects = titleMenuLayout->getObjectsByType(MenuObject::TYPE_SCROLLAREA);
		for (int i = 0; i < menuObjects.size(); ++i)
			menuObjectNames.append(menuObjects[i]->getName());
		menuObjects.clear();
		menuObjectNames.sort();
		cboxScrollArea->addItems(menuObjectNames);
	}
	if (cboxScrollArea->count() == 0)
	{
		frmScrollArea->setEnabled(false);
		cboxScrollArea->addItem("No Selectable Areas or Item Lists To Select");
		frmScrollTo->setEnabled(false);
		buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
	}
	wLocation->setEnabled(false);
	wLocationVariables->setEnabled(false);
	lblXVariable->setText(ProjectData::numberVariableIdToString(0));
	lblYVariable->setText(ProjectData::numberVariableIdToString(0));
	bXSideButton->setSpinBox(sbX);
	bYSideButton->setSpinBox(sbY);
}

ConfigureEvent_ScrollArea::~ConfigureEvent_ScrollArea()
{
	if (mainMenuRef != NULL)
	{
		delete mainMenuRef;
		mainMenuRef = NULL;
	}
	if (popupMenuRef != NULL)
	{
		delete popupMenuRef;
		popupMenuRef = NULL;
	}
	titleMenuLayout = NULL;
}

void ConfigureEvent_ScrollArea::setScrollArea(QString objectName)
{
	int index = cboxScrollArea->findText(objectName);
	if (index != -1)
		cboxScrollArea->setCurrentIndex(index);
}

void ConfigureEvent_ScrollArea::setScrollTo(int scrollTo)
{
	if (scrollTo == CHILD_OBJECT)
		rbChildObject->setChecked(true);
	else if (scrollTo == SPECIFIC_LOCATION)
		rbSpecificLocation->setChecked(true);
	else
		rbStoredLocation->setChecked(true);
}

void ConfigureEvent_ScrollArea::setChildObject(QString objectName)
{
	int index = cboxChildObject->findText(objectName);
	if (index != -1)
		cboxChildObject->setCurrentIndex(index);
}

void ConfigureEvent_ScrollArea::setLocation(int x, int y)
{
	if (rbSpecificLocation->isChecked())
	{
		sbX->setValue(x);
		sbY->setValue(y);
	}
	else
	{
		lblXVariable->setText(ProjectData::numberVariableIdToString(x));
		lblYVariable->setText(ProjectData::numberVariableIdToString(y));
	}
}

QString ConfigureEvent_ScrollArea::getScrollArea()
{
	return cboxScrollArea->currentText();
}

int ConfigureEvent_ScrollArea::getScrollTo()
{
	if (rbChildObject->isChecked())
		return CHILD_OBJECT;
	else if (rbSpecificLocation->isChecked())
		return SPECIFIC_LOCATION;
	return STORED_LOCATION;
}

QString ConfigureEvent_ScrollArea::getChildObject()
{
	return cboxChildObject->currentText();
}

int ConfigureEvent_ScrollArea::getXLocation()
{
	if (rbSpecificLocation->isChecked())
		return sbX->value();
	return ProjectData::numberVariableIdFromString(lblXVariable->text());
}

int ConfigureEvent_ScrollArea::getYLocation()
{
	if (rbSpecificLocation->isChecked())
		return sbY->value();
	return ProjectData::numberVariableIdFromString(lblYVariable->text());
}

void ConfigureEvent_ScrollArea::on_bBrowseXVariables_clicked()
{
	VariableSelector *variableSelector;
	int varID = ProjectData::numberVariableIdFromString(lblXVariable->text());
	variableSelector = new VariableSelector(VariableSelector::TYPE_NUMBERS, varID);
	if (variableSelector->exec())
	{
		varID = variableSelector->getVariableID();
		lblXVariable->setText(ProjectData::numberVariableIdToString(varID));
	}
	delete variableSelector;
}

void ConfigureEvent_ScrollArea::on_bBrowseYVariables_clicked()
{
	VariableSelector *variableSelector;
	int varID = ProjectData::numberVariableIdFromString(lblYVariable->text());
	variableSelector = new VariableSelector(VariableSelector::TYPE_NUMBERS, varID);
	if (variableSelector->exec())
	{
		varID = variableSelector->getVariableID();
		lblYVariable->setText(ProjectData::numberVariableIdToString(varID));
	}
	delete variableSelector;
}

void ConfigureEvent_ScrollArea::on_cboxScrollArea_currentIndexChanged()
{
	QList<MenuObject*> menuObjects;
	QStringList menuObjectNames;
	if (cboxScrollArea->isEnabled())
	{
		if (menuType == MenuObject::MENUTYPE_MAINMENU)
		{
			MenuScrollArea *scrollArea = (MenuScrollArea*)(*mainMenuRef)->getObjectByName(cboxScrollArea->currentText());
			menuObjects = scrollArea->getChildObjects();
			for (int i = 0; i < menuObjects.size(); ++i)
				menuObjectNames.append(menuObjects[i]->getName());
			menuObjects.clear();
			menuObjectNames.sort();
			cboxChildObject->addItems(menuObjectNames);
			sbX->setMaximum(scrollArea->getInternalWidth() - scrollArea->getWidth());
			sbY->setMaximum(scrollArea->getInternalHeight() - scrollArea->getHeight());
		}
		else if (menuType == MenuObject::MENUTYPE_POPUPMENU)
		{
			MenuScrollArea *scrollArea = (MenuScrollArea*)(*popupMenuRef)->getObjectByName(cboxScrollArea->currentText());
			menuObjects = scrollArea->getChildObjects();
			for (int i = 0; i < menuObjects.size(); ++i)
				menuObjectNames.append(menuObjects[i]->getName());
			menuObjects.clear();
			menuObjectNames.sort();
			cboxChildObject->addItems(menuObjectNames);
			sbX->setMaximum(scrollArea->getInternalWidth() - scrollArea->getWidth());
			sbY->setMaximum(scrollArea->getInternalHeight() - scrollArea->getHeight());
		}
		else if (menuType == MenuObject::MENUTYPE_TITLESCREENMENU)
		{
			MenuScrollArea *scrollArea = (MenuScrollArea*)titleMenuLayout->getObjectByName(cboxScrollArea->currentText());
			menuObjects = scrollArea->getChildObjects();
			for (int i = 0; i < menuObjects.size(); ++i)
				menuObjectNames.append(menuObjects[i]->getName());
			menuObjects.clear();
			menuObjectNames.sort();
			cboxChildObject->addItems(menuObjectNames);
			sbX->setMaximum(scrollArea->getInternalWidth() - scrollArea->getWidth());
			sbY->setMaximum(scrollArea->getInternalHeight() - scrollArea->getHeight());
		}
	}
}

void ConfigureEvent_ScrollArea::on_rbChildObject_toggled(bool checked)
{
	cboxChildObject->setEnabled(checked);
}

void ConfigureEvent_ScrollArea::on_rbSpecificLocation_toggled(bool checked)
{
	wLocation->setEnabled(checked);
}

void ConfigureEvent_ScrollArea::on_rbStoredLocation_toggled(bool checked)
{
	wLocationVariables->setEnabled(checked);
}
