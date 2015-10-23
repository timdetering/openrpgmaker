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
 *  Copyright (C) 2010, Justin Davis <tuxdavis@gmail.com>                   *
 ****************************************************************************/

#include <QMessageBox>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cmath>
#include "arraysize.h"
#include "clipboard.h"
#include "database_attributes.h"
#include "projectdata.h"
#include "projectdata_attribute.h"
#include "projectdata_character.h"
#include "projectdata_monster.h"
#include "xmlParser.h"

Database_Attributes::Database_Attributes(QWidget *parent) : QWidget(parent)
{
	setupUi(this);
	bMultiplierASpinButton->setSpinBox(sbMultiplierA);
	bMultiplierBSpinButton->setSpinBox(sbMultiplierB);
	bMultiplierCSpinButton->setSpinBox(sbMultiplierC);
	bMultiplierDSpinButton->setSpinBox(sbMultiplierD);
	bMultiplierESpinButton->setSpinBox(sbMultiplierE);
	attributeRef = NULL;
}

Database_Attributes::~Database_Attributes()
{
	if (attributeRef != NULL)
	{
		delete attributeRef;
		attributeRef = NULL;
	}
}

void Database_Attributes::setupAttributes()
{
	attributeRef = NULL;
	updateAttributeList();
}

void Database_Attributes::shutdownAttributes()
{
	if (attributeRef != NULL)
	{
		delete attributeRef;
		attributeRef = NULL;
	}
}

void Database_Attributes::applyAttributes()
{
	for (int i = 0; i < ProjectData::characterNames.size(); ++i)
	{
		ProjectData::DataReference<Character> *charRef = new ProjectData::DataReference<Character>(i, __FILE__, __LINE__);
		(*charRef)->resizeAttributeResists(ProjectData::attributeNames.size());
		delete charRef;
	}
	for (int i = 0; i < ProjectData::monsterNames.size(); ++i)
	{
		ProjectData::DataReference<Monster> *monsterRef = new ProjectData::DataReference<Monster>(i, __FILE__, __LINE__);
		(*monsterRef)->resizeAttributeResists(ProjectData::attributeNames.size());
		delete monsterRef;
	}
}

void Database_Attributes::on_lwAttributeList_itemSelectionChanged()
{
	int attributeID;
	if (attributeRef != NULL)
	{
		delete attributeRef;
		attributeRef = NULL;
	}
	attributeID = lwAttributeList->currentRow();
	if (attributeID >= 0)
	{
		attributeRef = new ProjectData::DataReference<Attribute>(attributeID, __FILE__, __LINE__);
		leName->setText((*attributeRef)->getName());
		if ((*attributeRef)->getType() == Attribute::TYPE_WEAPON)
			rbTypeWeapon->setChecked(true);
		else
			rbTypeMagic->setChecked(true);
		sbMultiplierA->setValue((*attributeRef)->getMultiplierA());
		sbMultiplierB->setValue((*attributeRef)->getMultiplierB());
		sbMultiplierC->setValue((*attributeRef)->getMultiplierC());
		sbMultiplierD->setValue((*attributeRef)->getMultiplierD());
		sbMultiplierE->setValue((*attributeRef)->getMultiplierE());
	}
}

void Database_Attributes::on_bArraySize_clicked()
{
	int numAttributes = ProjectData::attributeNames.size();
	ArraySize *arraysize = new ArraySize(numAttributes, 1, 5000);
	if (arraysize->exec())
	{
		int newSize = arraysize->getValue();
		if (newSize < numAttributes)
		{
			int confirmdelete = QMessageBox::warning(0, "Delete Data?", QString("If you decrease the array size then\nany unsaved changes will be perminately lost.\nIs it ok to delete data?"), QMessageBox::Yes|QMessageBox::No, QMessageBox::No);
			if (confirmdelete == QMessageBox::Yes)
			{
				if (attributeRef != NULL)
				{
					if (newSize <= attributeRef->getDataID())
					{
						delete attributeRef;
						attributeRef = NULL;
					}
				}
				for (int i = numAttributes - 1; i >= newSize; --i)
					ProjectData::deleteAttribute(i);
			}
		}
		else
		{
			for (int i = numAttributes; i < newSize; ++i)
			{
				Attribute *attribute = new Attribute;
				attribute->setName(QString("Attrib%1").arg(i, 4, 10, QChar('0')));
				ProjectData::addAttribute(attribute);
				delete attribute;
			}
		}
		updateAttributeList();
	}
	delete arraysize;
}

void Database_Attributes::on_leName_textChanged(QString text)
{
	int currentRow = lwAttributeList->currentRow();
	lwAttributeList->item(currentRow)->setText(QString("%1: %2").arg(currentRow, 4, 10, QChar('0')).arg(text));
	ProjectData::attributeNames[currentRow] = text;
	(*attributeRef)->setName(text);
}

void Database_Attributes::on_rbTypeWeapon_toggled(bool on)
{
	if (on)
		(*attributeRef)->setType(Attribute::TYPE_WEAPON);
}

void Database_Attributes::on_rbTypeMagic_toggled(bool on)
{
	if (on)
		(*attributeRef)->setType(Attribute::TYPE_MAGIC);
}

void Database_Attributes::on_sbMultiplierA_valueChanged(int value)
{
	(*attributeRef)->setMultiplierA(value);
}

void Database_Attributes::on_sbMultiplierB_valueChanged(int value)
{
	(*attributeRef)->setMultiplierB(value);
}

void Database_Attributes::on_sbMultiplierC_valueChanged(int value)
{
	(*attributeRef)->setMultiplierC(value);
}

void Database_Attributes::on_sbMultiplierD_valueChanged(int value)
{
	(*attributeRef)->setMultiplierD(value);
}

void Database_Attributes::on_sbMultiplierE_valueChanged(int value)
{
	(*attributeRef)->setMultiplierE(value);
}

void Database_Attributes::updateAttributeList()
{
	QListWidgetItem *item;
	int i, numAttributes = ProjectData::attributeNames.size();
	int currentRow = lwAttributeList->currentRow();
	lwAttributeList->clear();
	for (i = 0; i < numAttributes; ++i)
	{
		item = new QListWidgetItem(QString("%1: %2").arg(i, 4, 10, QChar('0')).arg(ProjectData::attributeNames[i]));
		item->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
		lwAttributeList->addItem(item);
	}
	if (currentRow >= lwAttributeList->count())
		currentRow = lwAttributeList->count() - 1;
	else if (currentRow < 0)
		currentRow = 0;
	if (ProjectData::attributeNames.size() > 0)
	{
		lwAttributeList->setCurrentRow(currentRow);
		lwAttributeList->item(currentRow)->setSelected(true);
	}
}
