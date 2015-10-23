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

#include "getpartymemberdialog.h"
#include "projectdata.h"
#include "variableselector.h"

GetPartyMemberDialog::GetPartyMemberDialog(QWidget *parent) : QDialog(parent)
{
	setupUi(this);
	lblVariable->setText(ProjectData::numberVariableIdToString(0));
	resize(width(), sizeHint().height());
	setMinimumWidth(width());
	setMaximumWidth(width());
	setMinimumHeight(height());
	setMaximumHeight(height());
}

GetPartyMemberDialog::~GetPartyMemberDialog()
{
}

int GetPartyMemberDialog::getID()
{
	if (rbPartyMember1->isChecked())
		return 0;
	else if (rbPartyMember2->isChecked())
		return 1;
	else if (rbPartyMember3->isChecked())
		return 2;
	else if (rbPartyMember4->isChecked())
		return 3;
	return ProjectData::numberVariableIdFromString(lblVariable->text());
}

void GetPartyMemberDialog::setID(int id)
{
	if (rbVariable->isChecked())
		lblVariable->setText(ProjectData::numberVariableIdToString(id));
	else if (id == 0)
		rbPartyMember1->setChecked(true);
	else if (id == 1)
		rbPartyMember2->setChecked(true);
	else if (id == 2)
		rbPartyMember3->setChecked(true);
	else if (id == 3)
		rbPartyMember4->setChecked(true);
}

bool GetPartyMemberDialog::isIDStoredInVariable()
{
	return rbVariable->isChecked();
}

void GetPartyMemberDialog::setIDStoredInVariable(bool stored)
{
	if (stored)
		rbVariable->setChecked(true);
}

void GetPartyMemberDialog::on_bBrowseVariables_clicked()
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

void GetPartyMemberDialog::on_rbVariable_toggled(bool checked)
{
	wVariable->setEnabled(checked);
}
