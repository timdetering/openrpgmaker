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

#include "configureevent_tilesubstitution.h"
#include "variableselector.h"

ConfigureEvent_TileSubstitution::ConfigureEvent_TileSubstitution(QWidget *parent) : QDialog(parent)
{
	QString varText = ProjectData::numberVariableIdToString(0);
	setupUi(this);
	lblTileset1Variable->setText(varText);
	lblTile1Variable->setText(varText);
	lblTileset2Variable->setText(varText);
	lblTile2Variable->setText(varText);
	wTile1Variables->setEnabled(false);
	wTile2Variables->setEnabled(false);
	resize(sizeHint().width(), sizeHint().height());
	setMinimumWidth(width());
	setMaximumWidth(width());
	setMinimumHeight(height());
	setMaximumHeight(height());
}

ConfigureEvent_TileSubstitution::~ConfigureEvent_TileSubstitution()
{
}

int ConfigureEvent_TileSubstitution::getLayer()
{
	if (rbAllLayers->isChecked())
		return ALL;
	else if (rbLowerLayer->isChecked())
		return LOWER;
	else if (rbHeroLayer->isChecked())
		return HERO;
	return UPPER;
}

void ConfigureEvent_TileSubstitution::setLayer(int value)
{
	if (value == ALL)
		rbAllLayers->setChecked(true);
	else if (value == LOWER)
		rbLowerLayer->setChecked(true);
	else if (value == HERO)
		rbHeroLayer->setChecked(true);
	else if (value == UPPER)
		rbUpperLayer->setChecked(true);
}

int ConfigureEvent_TileSubstitution::getTileset1ID()
{
	if (rbSpecificTile1->isChecked())
		return (int)(bTile1->getTile() >> 8);
	return ProjectData::numberVariableIdFromString(lblTileset1Variable->text());
}

int ConfigureEvent_TileSubstitution::getTile1ID()
{
	if (rbSpecificTile1->isChecked())
		return (int)(bTile1->getTile() % 256);
	return ProjectData::numberVariableIdFromString(lblTile1Variable->text());
}

void ConfigureEvent_TileSubstitution::setTile1(int tilesetID, int tileID)
{
	if (rbSpecificTile1->isChecked())
		bTile1->setTile(((unsigned short)tilesetID << 8) + (unsigned short)tileID);
	else
	{
		lblTileset1Variable->setText(ProjectData::numberVariableIdToString(tilesetID));
		lblTile1Variable->setText(ProjectData::numberVariableIdToString(tileID));
	}
}

int ConfigureEvent_TileSubstitution::getTileset2ID()
{
	if (rbSpecificTile2->isChecked())
		return (int)(bTile2->getTile() >> 8);
	return ProjectData::numberVariableIdFromString(lblTileset2Variable->text());
}

int ConfigureEvent_TileSubstitution::getTile2ID()
{
	if (rbSpecificTile2->isChecked())
		return (int)(bTile2->getTile() % 256);
	return ProjectData::numberVariableIdFromString(lblTile2Variable->text());
}

void ConfigureEvent_TileSubstitution::setTile2(int tilesetID, int tileID)
{
	if (rbSpecificTile2->isChecked())
		bTile2->setTile(((unsigned short)tilesetID << 8) + (unsigned short)tileID);
	else
	{
		lblTileset2Variable->setText(ProjectData::numberVariableIdToString(tilesetID));
		lblTile2Variable->setText(ProjectData::numberVariableIdToString(tileID));
	}
}

bool ConfigureEvent_TileSubstitution::isTile1ReferencedInVariables()
{
	return rbReferencedTile1->isChecked();
}

void ConfigureEvent_TileSubstitution::setTile1ReferencedInVariables(bool referenced)
{
	if (referenced)
		rbReferencedTile1->setChecked(true);
	else
		rbSpecificTile1->setChecked(true);
}

bool ConfigureEvent_TileSubstitution::isTile2ReferencedInVariables()
{
	return rbReferencedTile2->isChecked();
}

void ConfigureEvent_TileSubstitution::setTile2ReferencedInVariables(bool referenced)
{
	if (referenced)
		rbReferencedTile2->setChecked(true);
	else
		rbSpecificTile2->setChecked(true);
}

void ConfigureEvent_TileSubstitution::on_bBrowseTile1Variables_clicked()
{
	VariableSelector *variableSelector;
	int varID = ProjectData::numberVariableIdFromString(lblTile1Variable->text());
	variableSelector = new VariableSelector(VariableSelector::TYPE_NUMBERS, varID);
	if (variableSelector->exec())
	{
		varID = variableSelector->getVariableID();
		lblTile1Variable->setText(ProjectData::numberVariableIdToString(varID));
	}
	delete variableSelector;
}

void ConfigureEvent_TileSubstitution::on_bBrowseTile2Variables_clicked()
{
	VariableSelector *variableSelector;
	int varID = ProjectData::numberVariableIdFromString(lblTile2Variable->text());
	variableSelector = new VariableSelector(VariableSelector::TYPE_NUMBERS, varID);
	if (variableSelector->exec())
	{
		varID = variableSelector->getVariableID();
		lblTile2Variable->setText(ProjectData::numberVariableIdToString(varID));
	}
	delete variableSelector;
}

void ConfigureEvent_TileSubstitution::on_bBrowseTileset1Variables_clicked()
{
	VariableSelector *variableSelector;
	int varID = ProjectData::numberVariableIdFromString(lblTileset1Variable->text());
	variableSelector = new VariableSelector(VariableSelector::TYPE_NUMBERS, varID);
	if (variableSelector->exec())
	{
		varID = variableSelector->getVariableID();
		lblTileset1Variable->setText(ProjectData::numberVariableIdToString(varID));
	}
	delete variableSelector;
}

void ConfigureEvent_TileSubstitution::on_bBrowseTileset2Variables_clicked()
{
	VariableSelector *variableSelector;
	int varID = ProjectData::numberVariableIdFromString(lblTileset2Variable->text());
	variableSelector = new VariableSelector(VariableSelector::TYPE_NUMBERS, varID);
	if (variableSelector->exec())
	{
		varID = variableSelector->getVariableID();
		lblTileset2Variable->setText(ProjectData::numberVariableIdToString(varID));
	}
	delete variableSelector;
}

void ConfigureEvent_TileSubstitution::on_rbSpecificTile1_toggled(bool checked)
{
	wTile1->setEnabled(checked);
	wTile1Variables->setEnabled(!checked);
}

void ConfigureEvent_TileSubstitution::on_rbSpecificTile2_toggled(bool checked)
{
	wTile2->setEnabled(checked);
	wTile2Variables->setEnabled(!checked);
}
