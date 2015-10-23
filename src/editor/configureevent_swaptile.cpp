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

#include "configureevent_swaptile.h"
#include "selectmaplocation.h"
#include "variableselector.h"

ConfigureEvent_SwapTile::ConfigureEvent_SwapTile(int map, QWidget *parent) : QDialog(parent)
{
	setupUi(this);
	mapID = map;
	currentX = 0;
	currentY = 0;
	layer = 0;
	locationStoredInVariables = false;
	lblLocation->setText("(0, 0) layer=0");
	lblTilesetVariable->setText(ProjectData::numberVariableIdToString(0));
	lblTileVariable->setText(ProjectData::numberVariableIdToString(0));
	wTileVariables->setEnabled(false);
	resize(sizeHint().width(), sizeHint().height());
	setMinimumWidth(width());
	setMaximumWidth(width());
	setMinimumHeight(height());
	setMaximumHeight(height());
}

ConfigureEvent_SwapTile::~ConfigureEvent_SwapTile()
{
}

int ConfigureEvent_SwapTile::getXLocation()
{
	return currentX;
}

void ConfigureEvent_SwapTile::setXLocation(int value)
{
	currentX = value;
	if (locationStoredInVariables)
		lblLocation->setText(QString("(variable[%1], variable[%2]) layer=variable[%3]").arg(ProjectData::numberVariableIdToString(currentX)).arg(ProjectData::numberVariableIdToString(currentY)).arg(ProjectData::numberVariableIdToString(layer)));
	else
		lblLocation->setText(QString("(%1, %2) layer=%3").arg(currentX).arg(currentY).arg(layer));
}

int ConfigureEvent_SwapTile::getYLocation()
{
	return currentY;
}

void ConfigureEvent_SwapTile::setYLocation(int value)
{
	currentY = value;
	if (locationStoredInVariables)
		lblLocation->setText(QString("(variable[%1], variable[%2]) layer=variable[%3]").arg(ProjectData::numberVariableIdToString(currentX)).arg(ProjectData::numberVariableIdToString(currentY)).arg(ProjectData::numberVariableIdToString(layer)));
	else
		lblLocation->setText(QString("(%1, %2) layer=%3").arg(currentX).arg(currentY).arg(layer));
}

int ConfigureEvent_SwapTile::getLayer()
{
	return layer;
}

void ConfigureEvent_SwapTile::setLayer(int value)
{
	layer = value;
	if (locationStoredInVariables)
		lblLocation->setText(QString("(variable[%1], variable[%2]) layer=variable[%3]").arg(ProjectData::numberVariableIdToString(currentX)).arg(ProjectData::numberVariableIdToString(currentY)).arg(ProjectData::numberVariableIdToString(layer)));
	else
		lblLocation->setText(QString("(%1, %2) layer=%3").arg(currentX).arg(currentY).arg(layer));
}

int ConfigureEvent_SwapTile::getTilesetID()
{
	if (rbSpecificTile->isChecked())
		return (int)(bTile->getTile() >> 8);
	return ProjectData::numberVariableIdFromString(lblTilesetVariable->text());
}

int ConfigureEvent_SwapTile::getTileID()
{
	if (rbSpecificTile->isChecked())
		return (int)(bTile->getTile() % 256);
	return ProjectData::numberVariableIdFromString(lblTileVariable->text());
}

void ConfigureEvent_SwapTile::setTile(int tilesetID, int tileID)
{
	if (rbSpecificTile->isChecked())
		bTile->setTile(((unsigned short)tilesetID << 8) + (unsigned short)tileID);
	else
	{
		lblTilesetVariable->setText(ProjectData::numberVariableIdToString(tilesetID));
		lblTileVariable->setText(ProjectData::numberVariableIdToString(tileID));
	}
}

bool ConfigureEvent_SwapTile::isLocationStoredInVariables()
{
	return locationStoredInVariables;
}

void ConfigureEvent_SwapTile::setLocationStoredInVariables(bool value)
{
	locationStoredInVariables = value;
	if (locationStoredInVariables)
		lblLocation->setText(QString("(variable[%1], variable[%2]) layer=variable[%3]").arg(ProjectData::numberVariableIdToString(currentX)).arg(ProjectData::numberVariableIdToString(currentY)).arg(ProjectData::numberVariableIdToString(layer)));
	else
		lblLocation->setText(QString("(%1, %2) layer=%3").arg(currentX).arg(currentY).arg(layer));
}

bool ConfigureEvent_SwapTile::isTileReferencedInVariables()
{
	return rbReferencedTile->isChecked();
}

void ConfigureEvent_SwapTile::setTileReferencedInVariables(bool referenced)
{
	if (referenced)
		rbReferencedTile->setChecked(true);
	else
		rbSpecificTile->setChecked(true);
}

void ConfigureEvent_SwapTile::on_bBrowseTileVariables_clicked()
{
	VariableSelector *variableSelector;
	int varID = ProjectData::numberVariableIdFromString(lblTileVariable->text());
	variableSelector = new VariableSelector(VariableSelector::TYPE_NUMBERS, varID);
	if (variableSelector->exec())
	{
		varID = variableSelector->getVariableID();
		lblTileVariable->setText(ProjectData::numberVariableIdToString(varID));
	}
	delete variableSelector;
}

void ConfigureEvent_SwapTile::on_bBrowseTilesetVariables_clicked()
{
	VariableSelector *variableSelector;
	int varID = ProjectData::numberVariableIdFromString(lblTilesetVariable->text());
	variableSelector = new VariableSelector(VariableSelector::TYPE_NUMBERS, varID);
	if (variableSelector->exec())
	{
		varID = variableSelector->getVariableID();
		lblTilesetVariable->setText(ProjectData::numberVariableIdToString(varID));
	}
	delete variableSelector;
}

void ConfigureEvent_SwapTile::on_bSelectLocation_clicked()
{
	SelectMapLocation *selectLocation = new SelectMapLocation(mapID);
	selectLocation->setLocationStoredInVariables(locationStoredInVariables);
	selectLocation->setLocation(currentX, currentY, layer);
	if (selectLocation->exec())
	{
		currentX = selectLocation->getXLocation();
		currentY = selectLocation->getYLocation();
		layer = selectLocation->getLayer();
		locationStoredInVariables = selectLocation->isLocationStoredInVariables();
	}
	delete selectLocation;
	if (locationStoredInVariables)
		lblLocation->setText(QString("(variable[%1], variable[%2]) layer=variable[%3]").arg(ProjectData::numberVariableIdToString(currentX)).arg(ProjectData::numberVariableIdToString(currentY)).arg(ProjectData::numberVariableIdToString(layer)));
	else
		lblLocation->setText(QString("(%1, %2) layer=%3").arg(currentX).arg(currentY).arg(layer));
}

void ConfigureEvent_SwapTile::on_rbSpecificTile_toggled(bool checked)
{
	wTile->setEnabled(checked);
	wTileVariables->setEnabled(!checked);
}
