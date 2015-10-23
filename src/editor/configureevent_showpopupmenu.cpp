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

#include "configureevent_showpopupmenu.h"
#include "projectdata.h"
#include "variableselector.h"

ConfigureEvent_ShowPopupMenu::ConfigureEvent_ShowPopupMenu(QWidget *parent) : QDialog(parent)
{
	QString varText = ProjectData::numberVariableIdToString(0);
	setupUi(this);
	resize(sizeHint().width(), sizeHint().height());
	setMinimumWidth(width());
	setMaximumWidth(width());
	setMinimumHeight(height());
	setMaximumHeight(height());
	cboxMenus->addItems(ProjectData::popupMenuLayoutNames.values());
	if (cboxMenus->count() == 0)
	{
		frmMenu->setEnabled(false);
		frmLocation->setEnabled(false);
		buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
		cboxMenus->addItem("No Popup Menus To Open");
	}
	lblVariable->setText(varText);
	lblXVariable->setText(varText);
	lblYVariable->setText(varText);
	wVariable->setEnabled(false);
	wCoordinateVariables->setEnabled(false);
	bXSideButton->setSpinBox(sbX);
	bYSideButton->setSpinBox(sbY);
}

ConfigureEvent_ShowPopupMenu::~ConfigureEvent_ShowPopupMenu()
{
}

void ConfigureEvent_ShowPopupMenu::setMenuID(int menuID)
{
	if (rbSpecificMenu->isChecked())
	{
		int index = ProjectData::popupMenuLayoutNames.keys().indexOf(menuID);
		if (index != -1)
			cboxMenus->setCurrentIndex(index);
	}
	else
		lblVariable->setText(ProjectData::numberVariableIdToString(menuID));
}

void ConfigureEvent_ShowPopupMenu::setIsReferencedMenu(bool referenced)
{
	if (referenced)
		rbReferencedMenu->setChecked(true);
	else
		rbSpecificMenu->setChecked(true);
}

void ConfigureEvent_ShowPopupMenu::setCoordinates(int x, int y)
{
	if (rbSpecificCoordinates->isChecked())
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

void ConfigureEvent_ShowPopupMenu::setIsStoredCoordinates(bool stored)
{
	if (stored)
		rbStoredCoordinates->setChecked(true);
	else
		rbSpecificCoordinates->setChecked(true);
}

int ConfigureEvent_ShowPopupMenu::getMenuID()
{
	if (rbSpecificMenu->isChecked())
		return ProjectData::popupMenuLayoutNames.keys()[cboxMenus->currentIndex()];
	return ProjectData::numberVariableIdFromString(lblVariable->text());
}

bool ConfigureEvent_ShowPopupMenu::isReferencedMenu()
{
	return rbReferencedMenu->isChecked();
}

int ConfigureEvent_ShowPopupMenu::getXCoordinate()
{
	if (rbSpecificCoordinates->isChecked())
		return sbX->value();
	return ProjectData::numberVariableIdFromString(lblXVariable->text());
}

int ConfigureEvent_ShowPopupMenu::getYCoordinate()
{
	if (rbSpecificCoordinates->isChecked())
		return sbY->value();
	return ProjectData::numberVariableIdFromString(lblYVariable->text());
}

bool ConfigureEvent_ShowPopupMenu::isStoredCoordinates()
{
	return rbStoredCoordinates->isChecked();
}

void ConfigureEvent_ShowPopupMenu::on_bBrowseVariables_clicked()
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

void ConfigureEvent_ShowPopupMenu::on_bBrowseXVariables_clicked()
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

void ConfigureEvent_ShowPopupMenu::on_bBrowseYVariables_clicked()
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

void ConfigureEvent_ShowPopupMenu::on_rbSpecificCoordinates_toggled(bool checked)
{
	wCoordinates->setEnabled(checked);
	wCoordinateVariables->setEnabled(!checked);
}

void ConfigureEvent_ShowPopupMenu::on_rbSpecificMenu_toggled(bool checked)
{
	cboxMenus->setEnabled(checked);
	wVariable->setEnabled(!checked);
}
