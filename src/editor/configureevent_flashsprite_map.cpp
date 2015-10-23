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

#include "configureevent_flashsprite_map.h"
#include "projectdata.h"
#include "projectdata_map.h"
#include "projectdata_mapevent.h"
#include "variableselector.h"

ConfigureEvent_FlashSprite_Map::ConfigureEvent_FlashSprite_Map(int mapID, QWidget *parent) : QDialog(parent)
{
	ProjectData::DataReference<Map> *mapRef = new ProjectData::DataReference<Map>(mapID, __FILE__, __LINE__);
	setupUi(this);
	cboxTarget->addItem("Hero");
	typeList.append(HERO);
	idList.append(-1);
	for (int i = 0; i < ProjectData::vehicleNames.size(); ++i)
	{
		vehicleIndexes.append(cboxTarget->count());
		cboxTarget->addItem(ProjectData::vehicleNames[i]);
		typeList.append(VEHICLE);
		idList.append(i);
	}
	cboxTarget->addItem("This Event");
	typeList.append(THIS_EVENT);
	idList.append(-1);
	for (int i = 0; i < (*mapRef)->numEvents(); ++i)
	{
		eventIndexes.append(cboxTarget->count());
		cboxTarget->addItem((*mapRef)->getEvent(i)->getName());
		typeList.append(EVENT);
		idList.append(i);
	}
	bOpacitySideButton->setSpinBox(sbOpacity);
	bDelaySideButton->setSpinBox(sbDelay);
	lblOpacityVariable->setText(ProjectData::numberVariableIdToString(0));
	lblDelayVariable->setText(ProjectData::numberVariableIdToString(0));
	wOpacityVariable->setEnabled(false);
	wDelayVariable->setEnabled(false);
	frmDelay->setEnabled(false);
	resize(sizeHint().width(), sizeHint().height());
	setMinimumWidth(width());
	setMaximumWidth(width());
	setMinimumHeight(height());
	setMaximumHeight(height());
	delete mapRef;
}

ConfigureEvent_FlashSprite_Map::~ConfigureEvent_FlashSprite_Map()
{
}

int ConfigureEvent_FlashSprite_Map::getTargetType()
{
	return typeList[cboxTarget->currentIndex()];
}

void ConfigureEvent_FlashSprite_Map::setTargetType(int type)
{
	bool foundOne = false;
	for (int i = 0; i < typeList.size() && !foundOne; ++i)
	{
		if (typeList[i] == type)
		{
			cboxTarget->setCurrentIndex(i);
			foundOne = true;
		}
	}
}

int ConfigureEvent_FlashSprite_Map::getTargetID()
{
	return idList[cboxTarget->currentIndex()];
}

void ConfigureEvent_FlashSprite_Map::setTargetID(int value)
{
	int currentType = typeList[cboxTarget->currentIndex()];
	if (currentType == VEHICLE)
		cboxTarget->setCurrentIndex(vehicleIndexes[value]);
	else if (currentType == EVENT)
		cboxTarget->setCurrentIndex(eventIndexes[value]);
}

int ConfigureEvent_FlashSprite_Map::getFlash()
{
	return cboxFlash->currentIndex();
}

void ConfigureEvent_FlashSprite_Map::setFlash(int value)
{
	cboxFlash->setCurrentIndex(value);
}

QColor ConfigureEvent_FlashSprite_Map::getColor()
{
	return bColor->getColor();
}

void ConfigureEvent_FlashSprite_Map::setColor(QColor color)
{
	bColor->setColor(color);
}

int ConfigureEvent_FlashSprite_Map::getOpacity()
{
	if (rbSpecificOpacity->isChecked())
		return sbOpacity->value();
	else
		return ProjectData::numberVariableIdFromString(lblOpacityVariable->text());
}

void ConfigureEvent_FlashSprite_Map::setOpacity(int value)
{
	if (rbSpecificOpacity->isChecked())
		sbOpacity->setValue(value);
	else
		lblOpacityVariable->setText(ProjectData::numberVariableIdToString(value));
}

int ConfigureEvent_FlashSprite_Map::getDelayBetweenFlashes()
{
	if (rbSpecificDelay->isChecked())
		return sbDelay->value();
	else
		return ProjectData::numberVariableIdFromString(lblDelayVariable->text());
}

void ConfigureEvent_FlashSprite_Map::setDelayBetweenFlashes(int value)
{
	if (rbSpecificDelay->isChecked())
		sbDelay->setValue(value);
	else
		lblDelayVariable->setText(ProjectData::numberVariableIdToString(value));
}

bool ConfigureEvent_FlashSprite_Map::isOpacityStoredInVariable()
{
	return rbStoredOpacity->isChecked();
}

void ConfigureEvent_FlashSprite_Map::setOpacityStoredInVariable(bool stored)
{
	if (stored)
		rbStoredOpacity->setChecked(true);
	else
		rbSpecificOpacity->setChecked(true);
}

bool ConfigureEvent_FlashSprite_Map::isDelayStoredInVariable()
{
	return rbStoredDelay->isChecked();
}

void ConfigureEvent_FlashSprite_Map::setDelayStoredInVariable(bool stored)
{
	if (stored)
		rbStoredDelay->setChecked(true);
	else
		rbSpecificDelay->setChecked(true);
}

bool ConfigureEvent_FlashSprite_Map::getHaltOtherProcesses()
{
	return cbHaltOtherProcesses->isChecked();
}

void ConfigureEvent_FlashSprite_Map::setHaltOtherProcesses(bool value)
{
	cbHaltOtherProcesses->setChecked(value);
}

void ConfigureEvent_FlashSprite_Map::on_bBrowseDelayVariables_clicked()
{
	VariableSelector *variableSelector;
	int varID = ProjectData::numberVariableIdFromString(lblDelayVariable->text());
	variableSelector = new VariableSelector(VariableSelector::TYPE_NUMBERS, varID);
	if (variableSelector->exec())
	{
		varID = variableSelector->getVariableID();
		lblDelayVariable->setText(ProjectData::numberVariableIdToString(varID));
	}
	delete variableSelector;
}

void ConfigureEvent_FlashSprite_Map::on_bBrowseOpacityVariables_clicked()
{
	VariableSelector *variableSelector;
	int varID = ProjectData::numberVariableIdFromString(lblOpacityVariable->text());
	variableSelector = new VariableSelector(VariableSelector::TYPE_NUMBERS, varID);
	if (variableSelector->exec())
	{
		varID = variableSelector->getVariableID();
		lblOpacityVariable->setText(ProjectData::numberVariableIdToString(varID));
	}
	delete variableSelector;
}

void ConfigureEvent_FlashSprite_Map::on_cboxFlash_currentIndexChanged(int index)
{
	if (index == ONCE)
	{
		cbHaltOtherProcesses->setEnabled(true);
		frmColor->setEnabled(true);
		frmOpacity->setEnabled(true);
		frmDelay->setEnabled(false);
	}
	else if (index == BEGIN)
	{
		cbHaltOtherProcesses->setEnabled(false);
		frmColor->setEnabled(true);
		frmOpacity->setEnabled(true);
		frmDelay->setEnabled(true);
	}
	else if (index == STOP)
	{
		cbHaltOtherProcesses->setEnabled(false);
		frmColor->setEnabled(false);
		frmOpacity->setEnabled(false);
		frmDelay->setEnabled(false);
	}
}

void ConfigureEvent_FlashSprite_Map::on_rbSpecificDelay_toggled(bool checked)
{
	wDelay->setEnabled(checked);
	wDelayVariable->setEnabled(!checked);
}

void ConfigureEvent_FlashSprite_Map::on_rbSpecificOpacity_toggled(bool checked)
{
	wOpacity->setEnabled(checked);
	wOpacityVariable->setEnabled(!checked);
}
