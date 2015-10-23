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

#include "configureevent_memorizeposition.h"
#include "projectdata.h"
#include "variableselector.h"

ConfigureEvent_MemorizePosition::ConfigureEvent_MemorizePosition(QWidget *parent) : QDialog(parent)
{
	QString varText = ProjectData::numberVariableIdToString(0);
	setupUi(this);
	resize(sizeHint());
	setMinimumWidth(width());
	setMaximumWidth(width());
	setMinimumHeight(height());
	setMaximumHeight(height());
	lblMapVariable->setText(varText);
	lblXVariable->setText(varText);
	lblYVariable->setText(varText);
	lblLayerVariable->setText(varText);
}

ConfigureEvent_MemorizePosition::~ConfigureEvent_MemorizePosition()
{
}

int ConfigureEvent_MemorizePosition::getMapVariableID()
{
	return ProjectData::numberVariableIdFromString(lblMapVariable->text());
}

void ConfigureEvent_MemorizePosition::setMapVariableID(int value)
{
	lblMapVariable->setText(ProjectData::numberVariableIdToString(value));
}

int ConfigureEvent_MemorizePosition::getXVariableID()
{
	return ProjectData::numberVariableIdFromString(lblXVariable->text());
}

void ConfigureEvent_MemorizePosition::setXVariableID(int value)
{
	lblXVariable->setText(ProjectData::numberVariableIdToString(value));
}

int ConfigureEvent_MemorizePosition::getYVariableID()
{
	return ProjectData::numberVariableIdFromString(lblYVariable->text());
}

void ConfigureEvent_MemorizePosition::setYVariableID(int value)
{
	lblYVariable->setText(ProjectData::numberVariableIdToString(value));
}

int ConfigureEvent_MemorizePosition::getLayerVariableID()
{
	return ProjectData::numberVariableIdFromString(lblLayerVariable->text());
}

void ConfigureEvent_MemorizePosition::setLayerVariableID(int value)
{
	lblLayerVariable->setText(ProjectData::numberVariableIdToString(value));
}

void ConfigureEvent_MemorizePosition::on_bBrowseMapVariables_clicked()
{
	VariableSelector *variableSelector;
	int varID = ProjectData::numberVariableIdFromString(lblMapVariable->text());
	variableSelector = new VariableSelector(VariableSelector::TYPE_NUMBERS, varID);
	if (variableSelector->exec())
	{
		varID = variableSelector->getVariableID();
		lblMapVariable->setText(ProjectData::numberVariableIdToString(varID));
	}
	delete variableSelector;
}

void ConfigureEvent_MemorizePosition::on_bBrowseXVariables_clicked()
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

void ConfigureEvent_MemorizePosition::on_bBrowseYVariables_clicked()
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

void ConfigureEvent_MemorizePosition::on_bBrowseLayerVariables_clicked()
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
