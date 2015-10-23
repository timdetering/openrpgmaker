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

#ifndef DATABASE_ITEMS_H
#define DATABASE_ITEMS_H

class Item;

#include "ui_database_items.h"
#include "projectdata.h"

class Database_Items : public QWidget, public Ui::Database_Items
{
	Q_OBJECT
	public:
		Database_Items(QWidget *parent=0);
		~Database_Items();
		void setupItems();
		void shutdownItems();
	protected slots:
		void on_lwItemList_itemSelectionChanged();
		void on_bArraySize_clicked();
		void on_leName_textChanged(QString text);
		void on_cboxType_currentIndexChanged(int index);
		void on_weapon_rbUsableByCharacter_toggled();
		void on_weapon_twCharacterClassList_cellClicked(int row);
		void on_weapon_bSetAnimation_clicked();
		void on_weapon_cboxBattleAnimation_currentIndexChanged(int index);
		void on_weapon_rbInflictCondition_toggled(bool on);
		void on_weapon_rbRemoveCondition_toggled(bool on);
		void on_medicine_rbUsableByCharacter_toggled();
		void on_medicine_twCharacterClassList_cellClicked(int row);
		void on_skillBook_rbUsableByCharacter_toggled();
		void on_skillBook_twCharacterClassList_cellClicked(int row);
		void on_seed_rbUsableByCharacter_toggled();
		void on_seed_twCharacterClassList_cellClicked(int row);
		void on_skillScroll_rbUsableByCharacter_toggled();
		void on_skillScroll_twCharacterClassList_cellClicked(int row);
		void on_switch_lwSwitch_itemDoubleClicked();
		void on_switch_tbBrowseSwitches_clicked();
		void on_equipment_rbUsableByCharacter_toggled();
		void on_equipment_twCharacterClassList_cellClicked(int row);
		void on_equipment_rbResistCondition_toggled(bool on);
		void animateBattleCharacter();
	private:
		void updateItemList();
		void commonItem_clearData();
		void equipment_clearData();
		void event_clearData();
		void medicine_clearData();
		void seed_clearData();
		void skillBook_clearData();
		void skillScroll_clearData();
		void switch_clearData();
		void weapon_clearData();
		void weapon_updateCharacterClassList();
		void weapon_updateSkillList();
		void weapon_updateBattleAnimationList();
		void weapon_updateAttributeList();
		void weapon_updateConditionList();
		void medicine_updateCharacterClassList();
		void medicine_updateConditionList();
		void skillBook_updateCharacterClassList();
		void skillBook_updateSkillList();
		void seed_updateCharacterClassList();
		void skillScroll_updateCharacterClassList();
		void skillScroll_updateSkillList();
		void equipment_updateSkillList();
		void equipment_updateCharacterClassList();
		void equipment_updateAttributeList();
		void equipment_updateConditionList();
		void updateCurrentItem();
		QGraphicsPixmapItem *animationItem;
		QTimer *battleCharacterTimer;
		QImage battleCharacterImage;
		QImage weaponImage;
		ProjectData::DataReference<Item> *itemRef;
		ProjectData::DataReference<BattleAnimation> *animRef;
		bool ignoreEvents;
};

#endif // DATABASE_ITEMS
