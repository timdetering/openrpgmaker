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

#include "configureevent_savegame.h"
#include "projectdata.h"
#include "variableselector.h"

ConfigureEvent_SaveGame::ConfigureEvent_SaveGame(QWidget *parent) : QDialog(parent)
{
	setupUi(this);
	resize(sizeHint().width(), sizeHint().height());
	setMinimumWidth(width());
	setMaximumWidth(width());
	setMinimumHeight(height());
	setMaximumHeight(height());
	bSaveSlotSideButton->setSpinBox(sbSaveSlot);
	wVariable->setEnabled(false);
	lblVariable->setText(ProjectData::numberVariableIdToString(0));
}

ConfigureEvent_SaveGame::~ConfigureEvent_SaveGame()
{
}

void ConfigureEvent_SaveGame::setSaveSlotID(int saveSlotID)
{
	if (rbSpecificSaveSlot->isChecked())
		sbSaveSlot->setValue(saveSlotID);
	else
		lblVariable->setText(ProjectData::numberVariableIdToString(saveSlotID));
}

void ConfigureEvent_SaveGame::setIsReferencedSaveSlot(bool referenced)
{
	if (referenced)
		rbReferencedSaveSlot->setChecked(true);
	else
		rbSpecificSaveSlot->setChecked(true);
}

int ConfigureEvent_SaveGame::getSaveSlotID()
{
	if (rbSpecificSaveSlot->isChecked())
		return sbSaveSlot->value();
	return ProjectData::numberVariableIdFromString(lblVariable->text());
}

bool ConfigureEvent_SaveGame::isReferencedSaveSlot()
{
	return rbReferencedSaveSlot->isChecked();
}

void ConfigureEvent_SaveGame::on_rbSpecificSaveSlot_toggled(bool checked)
{
	wSaveSlot->setEnabled(checked);
	wVariable->setEnabled(!checked);
}

void ConfigureEvent_SaveGame::on_bBrowseVariables_clicked()
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
