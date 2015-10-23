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

#include "getnumberdialog.h"
#include "projectdata.h"
#include "variableselector.h"

GetNumberDialog::GetNumberDialog(QString title, int minValue, int maxValue, QWidget *parent) : QDialog(parent)
{
	setupUi(this);
	setWindowTitle(title);
	lblTitle->setText(title);
	sbValue->setRange(minValue, maxValue);
	bValueSideButton->setSpinBox(sbValue);
	lblVariable->setText(ProjectData::numberVariableIdToString(0));
	resize(width(), sizeHint().height());
	setMinimumWidth(width());
	setMaximumWidth(width());
	setMinimumHeight(height());
	setMaximumHeight(height());
}

GetNumberDialog::~GetNumberDialog()
{
}

int GetNumberDialog::getValue()
{
	if (rbSpecificValue->isChecked())
		return sbValue->value();
	return ProjectData::numberVariableIdFromString(lblVariable->text());
}

void GetNumberDialog::setValue(int value)
{
	if (rbSpecificValue->isChecked())
		sbValue->setValue(value);
	else
		lblVariable->setText(ProjectData::numberVariableIdToString(value));
}

bool GetNumberDialog::isValueStoredInVariable()
{
	return rbStoredValue->isChecked();
}

void GetNumberDialog::setValueStoredInVariable(bool stored)
{
	if (stored)
		rbStoredValue->setChecked(true);
	else
		rbSpecificValue->setChecked(true);
}

void GetNumberDialog::on_bBrowseVariables_clicked()
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

void GetNumberDialog::on_rbSpecificValue_toggled(bool checked)
{
	wValue->setEnabled(checked);
	wVariable->setEnabled(!checked);
}
