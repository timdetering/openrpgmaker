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

#include "configureevent_changevehiclegraphic.h"
#include "projectdata.h"
#include "projectdata_vehicle.h"
#include "storagefile.h"
#include "variableselector.h"

ConfigureEvent_ChangeVehicleGraphic::ConfigureEvent_ChangeVehicleGraphic(QWidget *parent) : QDialog(parent)
{
	setupUi(this);
	resize(sizeHint().width(), sizeHint().height());
	setMinimumWidth(width());
	setMaximumWidth(width());
	setMinimumHeight(height());
	setMaximumHeight(height());
	cboxVehicles->addItems(ProjectData::vehicleNames);
	cboxSprites->addItems(ProjectData::characterSpriteNames);
	lblVehicleVariable->setText(ProjectData::numberVariableIdToString(0));
	lblSpriteVariable->setText(ProjectData::numberVariableIdToString(0));
	wVehicleVariable->setEnabled(false);
	wSpriteVariable->setEnabled(false);
}

ConfigureEvent_ChangeVehicleGraphic::~ConfigureEvent_ChangeVehicleGraphic()
{
}

void ConfigureEvent_ChangeVehicleGraphic::setVehicleID(int vehicleID)
{
	if (rbSpecificVehicle->isChecked())
		cboxVehicles->setCurrentIndex(vehicleID);
	else
		lblVehicleVariable->setText(ProjectData::numberVariableIdToString(vehicleID));
}

void ConfigureEvent_ChangeVehicleGraphic::setSpriteID(int spriteID)
{
	if (rbSpecificSprite->isChecked())
		cboxSprites->setCurrentIndex(spriteID);
	else
		lblSpriteVariable->setText(ProjectData::numberVariableIdToString(spriteID));
}

void ConfigureEvent_ChangeVehicleGraphic::setIsReferencedVehicle(bool referenced)
{
	if (referenced)
		rbReferencedVehicle->setChecked(true);
	else
		rbSpecificVehicle->setChecked(true);
}

void ConfigureEvent_ChangeVehicleGraphic::setIsReferencedSprite(bool referenced)
{
	if (referenced)
		rbReferencedSprite->setChecked(true);
	else
		rbSpecificSprite->setChecked(true);
}

int ConfigureEvent_ChangeVehicleGraphic::getVehicleID()
{
	if (rbSpecificVehicle->isChecked())
		return cboxVehicles->currentIndex();
	return ProjectData::numberVariableIdFromString(lblVehicleVariable->text());
}

int ConfigureEvent_ChangeVehicleGraphic::getSpriteID()
{
	if (rbSpecificSprite->isChecked())
		return cboxSprites->currentIndex();
	return ProjectData::numberVariableIdFromString(lblSpriteVariable->text());
}

bool ConfigureEvent_ChangeVehicleGraphic::isReferencedVehicle()
{
	return rbReferencedVehicle->isChecked();
}

bool ConfigureEvent_ChangeVehicleGraphic::isReferencedSprite()
{
	return rbReferencedSprite->isChecked();
}

void ConfigureEvent_ChangeVehicleGraphic::on_bBrowseVehicleVariables_clicked()
{
	VariableSelector *variableSelector;
	int varID = ProjectData::numberVariableIdFromString(lblVehicleVariable->text());
	variableSelector = new VariableSelector(VariableSelector::TYPE_NUMBERS, varID);
	if (variableSelector->exec())
	{
		varID = variableSelector->getVariableID();
		lblVehicleVariable->setText(ProjectData::numberVariableIdToString(varID));
	}
	delete variableSelector;
}

void ConfigureEvent_ChangeVehicleGraphic::on_rbSpecificVehicle_toggled(bool checked)
{
	cboxVehicles->setEnabled(checked);
	wVehicleVariable->setEnabled(!checked);
}

void ConfigureEvent_ChangeVehicleGraphic::on_rbSpecificSprite_toggled(bool checked)
{
	cboxSprites->setEnabled(checked);
	wSpriteVariable->setEnabled(!checked);
}

void ConfigureEvent_ChangeVehicleGraphic::on_bBrowseSpriteVariables_clicked()
{
	VariableSelector *variableSelector;
	int varID = ProjectData::numberVariableIdFromString(lblSpriteVariable->text());
	variableSelector = new VariableSelector(VariableSelector::TYPE_NUMBERS, varID);
	if (variableSelector->exec())
	{
		varID = variableSelector->getVariableID();
		lblSpriteVariable->setText(ProjectData::numberVariableIdToString(varID));
	}
	delete variableSelector;
}
