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

#ifndef DATABASE_H
#define DATABASE_H

class QGraphicsPixmapItem;
class QMouseEvent;

#include "ui_database.h"

class Database : public QDialog, public Ui::Database
{
	Q_OBJECT
	public:
		Database(QWidget *parent=0);
		~Database();
		void setupDatabase();
		void shutdownDatabase();
	protected slots:
		void on_bTilesets_toggled(bool on);
		void on_bTerrain_toggled(bool on);
		void on_bCharacters_toggled(bool on);
		void on_bClasses_toggled(bool on);
		void on_bCharacterSprites_toggled(bool on);
		void on_bSkills_toggled(bool on);
		void on_bItems_toggled(bool on);
		void on_bAttributes_toggled(bool on);
		void on_bConditions_toggled(bool on);
		void on_bStrings_toggled(bool on);
		void on_bSystem_toggled(bool on);
		void on_bCommonEvents_toggled(bool on);
		void on_bMonsterAnimations_toggled(bool on);
		void on_bMonsters_toggled(bool on);
		void on_bMonsterGroups_toggled(bool on);
		void on_bBattleAnimations_toggled(bool on);
		void on_bGlobalAnimations_toggled(bool on);
		void on_bScreenLayouts_toggled(bool on);
		void on_buttonBox_clicked(QAbstractButton *button);
	private:
		enum DatabaseTabs
		{
			TAB_TILESETS=0,
			TAB_TERRAIN=1,
			TAB_CHARACTERS=2,
			TAB_CHARACTERSPRITES=3,
			TAB_ITEMS=4,
			TAB_ATTRIBUTES=5,
			TAB_STRINGS=6,
			TAB_MONSTERS=7,
			TAB_CLASSES=8,
			TAB_SKILLS=9,
			TAB_CONDITIONS=10,
			TAB_SYSTEM=11,
			TAB_COMMONEVENTS=12,
			TAB_MONSTERGROUPS=13,
			TAB_BATTLEANIMATIONS=14,
			TAB_GLOBALANIMATIONS=15,
			TAB_SCREENLAYOUTS=16,
			TAB_MONSTERANIMATIONS=17
		};
		void changeCurrentIndex(int index);
		int currentIndex;
};

#endif // DATABASE_H
