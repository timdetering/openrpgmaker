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

#include <QGraphicsRectItem>
#include <QMouseEvent>
#include "projectdata.h"
#include "projectdata_map.h"
#include "selectmaplocation.h"
#include "variableselector.h"

SelectMapLocation::SelectMapLocation(int mapID, QWidget *parent) : QDialog(parent)
{
	QString varText = ProjectData::numberVariableIdToString(0);
	setupUi(this);
	resize(sizeHint().width(), sizeHint().height());
	setMinimumWidth(width());
	setMaximumWidth(width());
	setMinimumHeight(height());
	setMaximumHeight(height());
	lblXVariable->setText(varText);
	lblYVariable->setText(varText);
	lblLayerVariable->setText(varText);
	wLocation->setupMapLocation(mapID);
	setLocation(0, 0, 0);
}

SelectMapLocation::~SelectMapLocation()
{
}

void SelectMapLocation::setLocation(int x, int y, int layer)
{
	if (rbSpecificLocation->isChecked())
		wLocation->setLocation(x, y, layer);
	else
	{
		lblXVariable->setText(ProjectData::numberVariableIdToString(x));
		lblYVariable->setText(ProjectData::numberVariableIdToString(y));
		lblLayerVariable->setText(ProjectData::numberVariableIdToString(layer));
	}
}

int SelectMapLocation::getXLocation()
{
	if (rbSpecificLocation->isChecked())
		return wLocation->getXLocation();
	return ProjectData::numberVariableIdFromString(lblXVariable->text());
}

int SelectMapLocation::getYLocation()
{
	if (rbSpecificLocation->isChecked())
		return wLocation->getYLocation();
	return ProjectData::numberVariableIdFromString(lblYVariable->text());
}

int SelectMapLocation::getLayer()
{
	if (rbSpecificLocation->isChecked())
		return wLocation->getLayer();
	return ProjectData::numberVariableIdFromString(lblLayerVariable->text());
}

void SelectMapLocation::setLocationStoredInVariables(bool stored)
{
	if (stored)
		rbStoredLocation->setChecked(true);
	else
		rbSpecificLocation->setChecked(true);
}

bool SelectMapLocation::isLocationStoredInVariables()
{
	return rbStoredLocation->isChecked();
}

void SelectMapLocation::on_bBrowseXVariables_clicked()
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

void SelectMapLocation::on_bBrowseYVariables_clicked()
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

void SelectMapLocation::on_bBrowseLayerVariables_clicked()
{
	VariableSelector *variableSelector;
	int varID = ProjectData::numberVariableIdFromString(lblLayerVariable->text());
	variableSelector = new VariableSelector(VariableSelector::TYPE_NUMBERS, varID);
	if (variableSelector->exec())
	{
		varID = variableSelector->getVariableID();
		lblLayerVariable->setText(ProjectData::numberVariableIdToString(varID));
	}
	delete variableSelector;
}

void SelectMapLocation::on_rbSpecificLocation_toggled(bool checked)
{
	if (checked)
		stackedWidget->setCurrentWidget(wLocation);
	else
		stackedWidget->setCurrentWidget(wVariables);
}
