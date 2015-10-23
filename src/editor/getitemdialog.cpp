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

#include "getitemdialog.h"
#include "projectdata.h"
#include "variableselector.h"

GetItemDialog::GetItemDialog(QString title, QString type, QStringList items, QWidget *parent) : QDialog(parent)
{
	setupUi(this);
	setWindowTitle(title);
	lblType->setText(type);
	rbSpecificItem->setText(QString("Specific %1:").arg(type));
	rbReferencedItem->setText(QString("%1 Referenced in Variable:").arg(type));
	cboxItem->addItems(items);
	resize(width(), sizeHint().height());
	setMinimumWidth(width());
	setMaximumWidth(width());
	setMinimumHeight(height());
	setMaximumHeight(height());
}

GetItemDialog::~GetItemDialog()
{
}

int GetItemDialog::getItemID()
{
	if (rbSpecificItem->isChecked())
		return cboxItem->currentIndex();
	return ProjectData::numberVariableIdFromString(lblVariable->text());
}

void GetItemDialog::setItemID(int value)
{
	if (rbSpecificItem->isChecked())
		cboxItem->setCurrentIndex(value);
	else
		lblVariable->setText(ProjectData::numberVariableIdToString(value));
}

bool GetItemDialog::isItemReferencedInVariable()
{
	return rbReferencedItem->isChecked();
}

void GetItemDialog::setItemReferencedInVariable(bool referenced)
{
	if (referenced)
		rbReferencedItem->setChecked(true);
	else
		rbSpecificItem->setChecked(true);
}

void GetItemDialog::on_bBrowseVariables_clicked()
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

void GetItemDialog::on_rbSpecificItem_toggled(bool checked)
{
	cboxItem->setEnabled(checked);
	wVariable->setEnabled(checked);
}
