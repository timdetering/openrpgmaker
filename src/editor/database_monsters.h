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

#ifndef DATABASE_MONSTERS_H
#define DATABASE_MONSTERS_H

class Monster;
class MonsterAnimation;

#include "ui_database_monsters.h"
#include "projectdata.h"

class Database_Monsters : public QWidget, public Ui::Database_Monsters
{
	Q_OBJECT
	public:
		Database_Monsters(QWidget *parent=0);
		~Database_Monsters();
		void setupMonsters();
		void shutdownMonsters();
	protected slots:
		void on_lwMonsterList_itemSelectionChanged();
		void on_bArraySize_clicked();
		void on_leName_textChanged(QString text);
		void on_sbMaxHP_valueChanged(int value);
		void on_sbMaxMP_valueChanged(int value);
		void on_sbAttack_valueChanged(int value);
		void on_sbDefense_valueChanged(int value);
		void on_sbWisdom_valueChanged(int value);
		void on_sbAgility_valueChanged(int value);
		void on_cboxMonsterAnimation_currentIndexChanged(int value);
		void on_cbTransparent_toggled(bool on);
		void on_cbFlying_toggled(bool on);
		void on_sldrHueShift_valueChanged(int value);
		void on_sbExperience_valueChanged(int value);
		void on_sbMoney_valueChanged(int value);
		void on_sbItemProbability_valueChanged(int value);
		void on_bConfigureItems_clicked();
		void on_cbCriticalHits_toggled(bool on);
		void on_dsbCriticalHitRate_valueChanged(double value);
		void on_dsbCriticalHitMultiplier_valueChanged(double value);
		void on_cbAttacksOftenMiss_toggled(bool on);
		void on_bConfigureBattleEvent_clicked();
		void on_bConfigureAI_clicked();
		void on_lwConditionResist_itemClicked(QListWidgetItem *item);
		void on_lwAttributeResist_itemClicked(QListWidgetItem *item);
		void on_twBehavior_itemDoubleClicked();
		void on_tbDeleteBehavior_clicked();
		void on_tbAddBehavior_clicked();
		void animateMonster();
	private:
		void updateMonsterImage();
		void renderMonsterImage();
		void updateMonsterList();
		void updateMonsterAnimationList();
		void updateBehaviorList();
		void updateConditionResistList();
		void updateAttributeResistList();
		QGraphicsPixmapItem *monsterItem;
		QGraphicsPixmapItem *hueShiftItem1;
		QGraphicsPixmapItem *hueShiftItem2;
		QTimer *animationTimer;
		QImage originalMonsterImage;
		QImage hueShiftedMonsterImage;
		QIcon resistLevelA;
		QIcon resistLevelB;
		QIcon resistLevelC;
		QIcon resistLevelD;
		QIcon resistLevelE;
		ProjectData::DataReference<Monster> *monsterRef;
		ProjectData::DataReference<MonsterAnimation> *animRef;
		int monsterFrame;
		int monsterFrameDirection;
		bool ignoreEvents;
		bool pingPongAnimation;
};

#endif // DATABASE_MONSTERS_H
