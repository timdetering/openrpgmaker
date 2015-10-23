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

#include "getrandomnumberdialog.h"
#include "projectdata.h"
#include "variableselector.h"

GetRandomNumberDialog::GetRandomNumberDialog(QWidget *parent) : QDialog(parent)
{
	setupUi(this);
	rand_bMinSideButton->setSpinBox(rand_sbMin);
	rand_bMaxSideButton->setSpinBox(rand_sbMax);
	rand_lblMinVariable->setText(ProjectData::numberVariableIdToString(0));
	rand_lblMaxVariable->setText(ProjectData::numberVariableIdToString(0));
	resize(width(), sizeHint().height());
	setMinimumWidth(width());
	setMaximumWidth(width());
	setMinimumHeight(height());
	setMaximumHeight(height());
}

GetRandomNumberDialog::~GetRandomNumberDialog()
{
}

int GetRandomNumberDialog::getMin()
{
	if (rand_rbSpecificRange->isChecked())
		return rand_sbMin->value();
	return ProjectData::numberVariableIdFromString(rand_lblMinVariable->text());
}

int GetRandomNumberDialog::getMax()
{
	if (rand_rbSpecificRange->isChecked())
		return rand_sbMax->value();
	return ProjectData::numberVariableIdFromString(rand_lblMaxVariable->text());
}

void GetRandomNumberDialog::setRange(int min, int max)
{
	if (rand_rbSpecificRange->isChecked())
	{
		rand_sbMin->setValue(min);
		rand_sbMax->setValue(max);
	}
	else
	{
		rand_lblMinVariable->setText(ProjectData::numberVariableIdToString(min));
		rand_lblMaxVariable->setText(ProjectData::numberVariableIdToString(max));
	}
}

bool GetRandomNumberDialog::isStoredRange()
{
	return rand_rbStoredRange->isChecked();
}

void GetRandomNumberDialog::setStoredRange(bool stored)
{
	if (stored)
		rand_rbStoredRange->setChecked(true);
	else
		rand_rbSpecificRange->setChecked(true);
}

void GetRandomNumberDialog::on_rand_bBrowseMinVariables_clicked()
{
	VariableSelector *variableSelector;
	int varID = ProjectData::numberVariableIdFromString(rand_lblMinVariable->text());
	variableSelector = new VariableSelector(VariableSelector::TYPE_NUMBERS, varID);
	if (variableSelector->exec())
	{
		varID = variableSelector->getVariableID();
		rand_lblMinVariable->setText(ProjectData::numberVariableIdToString(varID));
	}
	delete variableSelector;
}

void GetRandomNumberDialog::on_rand_bBrowseMaxVariables_clicked()
{
	VariableSelector *variableSelector;
	int varID = ProjectData::numberVariableIdFromString(rand_lblMaxVariable->text());
	variableSelector = new VariableSelector(VariableSelector::TYPE_NUMBERS, varID);
	if (variableSelector->exec())
	{
		varID = variableSelector->getVariableID();
		rand_lblMaxVariable->setText(ProjectData::numberVariableIdToString(varID));
	}
	delete variableSelector;
}

void GetRandomNumberDialog::on_rand_rbSpecificRange_toggled(bool checked)
{
	rand_wRange->setEnabled(checked);
	rand_wVariables->setEnabled(!checked);
}
