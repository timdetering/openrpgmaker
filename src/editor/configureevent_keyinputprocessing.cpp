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

#include "configureevent_keyinputprocessing.h"
#include "projectdata.h"
#include "variableselector.h"

ConfigureEvent_KeyInputProcessing::ConfigureEvent_KeyInputProcessing(QWidget *parent) : QDialog(parent)
{
	setupUi(this);
	resize(sizeHint().width(), sizeHint().height());
	setMinimumWidth(width());
	setMaximumWidth(width());
	setMinimumHeight(height());
	setMaximumHeight(height());
	wStoreTimeTaken->setEnabled(false);
	wTimeVariable->setEnabled(false);
	lblVariable->setText(ProjectData::numberVariableIdToString(0));
	lblTimeVariable->setText(ProjectData::numberVariableIdToString(0));
}

ConfigureEvent_KeyInputProcessing::~ConfigureEvent_KeyInputProcessing()
{
}

void ConfigureEvent_KeyInputProcessing::setVariableID(int variableID)
{
	lblVariable->setText(ProjectData::numberVariableIdToString(variableID));
}

void ConfigureEvent_KeyInputProcessing::setWaitForKeyPress(bool wait)
{
	cbWait->setChecked(wait);
}

void ConfigureEvent_KeyInputProcessing::setStoreTimeTaken(bool storeTimeTaken)
{
	cbStoreTimeTaken->setChecked(storeTimeTaken);
}

void ConfigureEvent_KeyInputProcessing::setTimeTakenVariableID(int variableID)
{
	lblTimeVariable->setText(ProjectData::numberVariableIdToString(variableID));
}

int ConfigureEvent_KeyInputProcessing::getVariableID()
{
	return ProjectData::numberVariableIdFromString(lblVariable->text());
}

bool ConfigureEvent_KeyInputProcessing::waitForKeyPress()
{
	return cbWait->isChecked();
}

bool ConfigureEvent_KeyInputProcessing::storeTimeTaken()
{
	return cbStoreTimeTaken->isChecked();
}

int ConfigureEvent_KeyInputProcessing::getTimeTakenVariableID()
{
	return ProjectData::numberVariableIdFromString(lblTimeVariable->text());
}

void ConfigureEvent_KeyInputProcessing::on_bBrowseTimeVariables_clicked()
{
	VariableSelector *variableSelector;
	int varID = ProjectData::numberVariableIdFromString(lblTimeVariable->text());
	variableSelector = new VariableSelector(VariableSelector::TYPE_NUMBERS, varID);
	if (variableSelector->exec())
	{
		varID = variableSelector->getVariableID();
		lblTimeVariable->setText(ProjectData::numberVariableIdToString(varID));
	}
	delete variableSelector;
}

void ConfigureEvent_KeyInputProcessing::on_bBrowseVariables_clicked()
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

void ConfigureEvent_KeyInputProcessing::on_cbStoreTimeTaken_toggled(bool checked)
{
	wTimeVariable->setEnabled(checked);
}

void ConfigureEvent_KeyInputProcessing::on_cbWait_toggled(bool checked)
{
	wStoreTimeTaken->setEnabled(checked);
}
