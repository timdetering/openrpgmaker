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
#include "configureitemdrops.h"
#include "itemdroppeddelegate.h"
#include "projectdata.h"
#include "projectdata_monster.h"

ConfigureItemDrops::ConfigureItemDrops(QWidget *parent) : QDialog(parent)
{
	setupUi(this);
	twItemsDropped->setItemDelegate(new ItemDroppedDelegate(0, 1));
	twItemsDropped->horizontalHeader()->setResizeMode(0, QHeaderView::Stretch);
	twItemsDropped->horizontalHeader()->setResizeMode(1, QHeaderView::ResizeToContents);
	twItemsDropped->verticalHeader()->setResizeMode(QHeaderView::ResizeToContents);
	resize(sizeHint());
	setMinimumWidth(width());
	setMaximumWidth(width());
	setMinimumHeight(height());
	setMaximumHeight(height());
}

ConfigureItemDrops::~ConfigureItemDrops()
{
}

void ConfigureItemDrops::setupItemDrops(int monsterID)
{
	ProjectData::DataReference<Monster> *monsterRef = new ProjectData::DataReference<Monster>(monsterID, __FILE__, __LINE__);
	QTableWidgetItem *item;
	int numItemsDropped = (*monsterRef)->numItemsDropped();
	twItemsDropped->setRowCount(numItemsDropped);
	for (int i = 0; i < numItemsDropped; ++i)
	{
		Monster::ItemDrop itemDropped;
		(*monsterRef)->getItemDropped(i, &itemDropped);
		item = new QTableWidgetItem;
		item->setData(Qt::DisplayRole, ProjectData::itemNames[itemDropped.itemID]);
		item->setData(Qt::UserRole, itemDropped.itemID);
		item->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEditable|Qt::ItemIsEnabled);
		item->setTextAlignment(Qt::AlignLeft|Qt::AlignVCenter);
		twItemsDropped->setItem(i, 0, item);
		item = new QTableWidgetItem;
		item->setData(Qt::DisplayRole, QString::number(itemDropped.priority));
		item->setData(Qt::UserRole, itemDropped.priority);
		item->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEditable|Qt::ItemIsEnabled);
		item->setTextAlignment(Qt::AlignRight|Qt::AlignVCenter);
		twItemsDropped->setItem(i, 1, item);
	}
	delete monsterRef;
}

void ConfigureItemDrops::getItemDrops(int monsterID)
{
	ProjectData::DataReference<Monster> *monsterRef = new ProjectData::DataReference<Monster>(monsterID, __FILE__, __LINE__);
	Monster::ItemDrop itemDropped;
	int oldNumber = (*monsterRef)->numItemsDropped();
	int newNumber = twItemsDropped->rowCount();
	if (oldNumber < newNumber)
	{
		for (int i = 0; i < newNumber; ++i)
		{
			itemDropped.itemID = twItemsDropped->item(i, 0)->data(Qt::UserRole).toInt();
			itemDropped.priority = twItemsDropped->item(i, 1)->data(Qt::UserRole).toInt();
			if (i < oldNumber)
				(*monsterRef)->setItemDropped(i, itemDropped);
			else
				(*monsterRef)->addItemDropped(itemDropped);
		}
	}
	else
	{
		for (int i = 0; i < oldNumber; ++i)
		{
			if (i < newNumber)
			{
				itemDropped.itemID = twItemsDropped->item(i, 0)->data(Qt::UserRole).toInt();
				itemDropped.priority = twItemsDropped->item(i, 1)->data(Qt::UserRole).toInt();
				(*monsterRef)->setItemDropped(i, itemDropped);
			}
			else
				(*monsterRef)->deleteItemDropped(newNumber);
		}
	}
	delete monsterRef;
}

void ConfigureItemDrops::on_bDeleteItem_clicked()
{
	twItemsDropped->removeRow(twItemsDropped->currentRow());
}

void ConfigureItemDrops::on_bAddItem_clicked()
{
	QTableWidgetItem *item;
	int itemNumber = twItemsDropped->rowCount();
	twItemsDropped->setRowCount(itemNumber + 1);
	item = new QTableWidgetItem;
	item->setData(Qt::DisplayRole, ProjectData::itemNames[0]);
	item->setData(Qt::UserRole, 0);
	item->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEditable|Qt::ItemIsEnabled);
	item->setTextAlignment(Qt::AlignLeft|Qt::AlignVCenter);
	twItemsDropped->setItem(itemNumber, 0, item);
	item = new QTableWidgetItem;
	item->setData(Qt::DisplayRole, QString::number(50));
	item->setData(Qt::UserRole, 50);
	item->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEditable|Qt::ItemIsEnabled);
	item->setTextAlignment(Qt::AlignRight|Qt::AlignVCenter);
	twItemsDropped->setItem(itemNumber, 1, item);
}
