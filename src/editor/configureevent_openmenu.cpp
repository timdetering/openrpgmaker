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

#include "configureevent_openmenu.h"
#include "projectdata.h"
#include "variableselector.h"

ConfigureEvent_OpenMenu::ConfigureEvent_OpenMenu(QWidget *parent) : QDialog(parent)
{
	setupUi(this);
	resize(sizeHint().width(), sizeHint().height());
	setMinimumWidth(width());
	setMaximumWidth(width());
	setMinimumHeight(height());
	setMaximumHeight(height());
	lblVariable->setText(ProjectData::numberVariableIdToString(0));
	wVariable->setEnabled(false);
	cboxMenus->addItems(ProjectData::mainMenuLayoutNames.values());
}

ConfigureEvent_OpenMenu::~ConfigureEvent_OpenMenu()
{
}

void ConfigureEvent_OpenMenu::setMenuID(int menuID)
{
	if (rbSpecificMenu->isChecked())
	{
		int index = ProjectData::mainMenuLayoutNames.keys().indexOf(menuID);
		if (index != -1)
			cboxMenus->setCurrentIndex(index);
	}
	else
		lblVariable->setText(ProjectData::numberVariableIdToString(menuID));
}

void ConfigureEvent_OpenMenu::setIsReferencedMenu(bool referenced)
{
	if (referenced)
		rbReferencedMenu->setChecked(true);
	else
		rbSpecificMenu->setChecked(true);
}

int ConfigureEvent_OpenMenu::getMenuID()
{
	if (rbSpecificMenu->isChecked())
		return ProjectData::mainMenuLayoutNames.keys()[cboxMenus->currentIndex()];
	return ProjectData::numberVariableIdFromString(lblVariable->text());
}

bool ConfigureEvent_OpenMenu::isReferencedMenu()
{
	return rbReferencedMenu->isChecked();
}

void ConfigureEvent_OpenMenu::on_bBrowseVariables_clicked()
{
	VariableSelector *variableSelector;
	int varID = ProjectData::numberVariableIdFromString(lblVariable->text());
	variableSelector = new VariableSelector(VariableSelector::TYPE_NUMBERS, varID);
	if (variableSelector->exec())
	{
		varID = variableSelector->getVariableID();
		lblVariable->setText(ProjectData::numberVariableIdToString(varID));
	}
	delete variableSelector;
}

void ConfigureEvent_OpenMenu::on_rbSpecificMenu_toggled(bool checked)
{
	cboxMenus->setEnabled(checked);
	wVariable->setEnabled(!checked);
}
