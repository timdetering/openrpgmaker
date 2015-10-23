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

#include "configureevent_changebordertile.h"
#include "tilebutton.h"
#include "variableselector.h"

ConfigureEvent_ChangeBorderTile::ConfigureEvent_ChangeBorderTile(QWidget *parent) : QDialog(parent)
{
	QString varText = ProjectData::numberVariableIdToString(0);
	setupUi(this);
	lblTilesetVariable->setText(varText);
	lblTileVariable->setText(varText);
	wTileVariables->setEnabled(false);
	resize(sizeHint().width(), sizeHint().height());
	setMinimumWidth(width());
	setMaximumWidth(width());
	setMinimumHeight(height());
	setMaximumHeight(height());
}

ConfigureEvent_ChangeBorderTile::~ConfigureEvent_ChangeBorderTile()
{
}

int ConfigureEvent_ChangeBorderTile::getTilesetID()
{
	if (rbSpecificTile->isChecked())
		return (int)((bTile->getTile() >> 8) % 256);
	return ProjectData::numberVariableIdFromString(lblTilesetVariable->text());
}

int ConfigureEvent_ChangeBorderTile::getTileID()
{
	if (rbSpecificTile->isChecked())
		return (int)(bTile->getTile() % 256);
	return ProjectData::numberVariableIdFromString(lblTileVariable->text());
}

void ConfigureEvent_ChangeBorderTile::setTile(int tilesetID, int tileID)
{
	if (rbSpecificTile->isChecked())
		bTile->setTile(((unsigned short)tilesetID << 8) + (unsigned short)tileID);
	else
	{
		lblTilesetVariable->setText(ProjectData::numberVariableIdToString(tilesetID));
		lblTileVariable->setText(ProjectData::numberVariableIdToString(tileID));
	}
}

bool ConfigureEvent_ChangeBorderTile::isTileReferencedInVariables()
{
	return rbReferencedTile->isChecked();
}

void ConfigureEvent_ChangeBorderTile::setTileReferencedInVariables(bool tileReferencedInVariables)
{
	if (tileReferencedInVariables)
		rbReferencedTile->setChecked(true);
	else
		rbSpecificTile->setChecked(true);
}

void ConfigureEvent_ChangeBorderTile::on_bBrowseTileVariables_clicked()
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

void ConfigureEvent_ChangeBorderTile::on_bBrowseTilesetVariables_clicked()
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

void ConfigureEvent_ChangeBorderTile::on_rbSpecificTile_toggled(bool checked)
{
	wTile->setEnabled(checked);
	wTileVariables->setEnabled(!checked);
}
