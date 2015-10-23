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

#ifndef DATABASE_CHARACTERS_H
#define DATABASE_CHARACTERS_H

class Character;

#include "ui_database_characters.h"
#include "projectdata.h"

class Database_Characters : public QWidget, public Ui::Database_Characters
{
	Q_OBJECT
	public:
		Database_Characters(QWidget *parent=0);
		~Database_Characters();
		void setupCharacters();
		void shutdownCharacters();
	protected slots:
		void on_lwCharacterList_itemSelectionChanged();
		void on_bArraySize_clicked();
		void on_leName_textChanged(QString text);
		void on_sbMinLevel_valueChanged(int value);
		void on_sbMaxLevel_valueChanged(int value);
		void on_cboxClass_currentIndexChanged(int index);
		void on_bApplyClass_clicked();
		void on_cbDuelWielding_toggled(bool on);
		void on_cbAIControlled_toggled(bool on);
		void on_cbEquipmentLocked_toggled(bool on);
		void on_cbMightyGuard_toggled(bool on);
		void on_cbCriticalHits_toggled(bool on);
		void on_dsbCriticalHitRate_valueChanged(double value);
		void on_dsbCriticalHitMultiplier_valueChanged(double value);
		void on_bSetFace_clicked();
		void on_sbSpriteOpacity_valueChanged(int value);
		void on_cboxSprite_currentIndexChanged(int index);
		void on_gvMaxHP_mouseDoubleClicked();
		void on_gvMaxMP_mouseDoubleClicked();
		void on_gvAttack_mouseDoubleClicked();
		void on_gvDefense_mouseDoubleClicked();
		void on_gvWisdom_mouseDoubleClicked();
		void on_gvAgility_mouseDoubleClicked();
		void on_tbConfigureExperienceCurve_clicked();
		void on_bConfigureBattleCommands_clicked();
		void on_bConfigureBattleEvent_clicked();
		void on_cboxWeapon_currentIndexChanged(int index);
		void on_cboxShield_currentIndexChanged(int index);
		void on_cboxArmor_currentIndexChanged(int index);
		void on_cboxHelmet_currentIndexChanged(int index);
		void on_cboxAccessory_currentIndexChanged(int index);
		void on_cboxUnarmedBattleAnimation_currentIndexChanged(int index);
		void on_bConfigureAI_clicked();
		void on_twSkillList_itemSelectionChanged();
		void on_twSkillList_itemDoubleClicked(QTableWidgetItem *item);
		void on_tbAddSkill_clicked();
		void on_tbDeleteSkill_clicked();
		void on_lwConditionResist_itemClicked(QListWidgetItem *item);
		void on_lwAttributeResist_itemClicked(QListWidgetItem *item);
		void animateSprite();
	private:
		void updateCharacterList();
		void updateCharacterSpriteList();
		void updateItemLists();
		void updateBattleAnimationList();
		void updateSkillsList();
		void updateConditionResistList();
		void updateAttributeResistList();
		QGraphicsPixmapItem *faceItem;
		QGraphicsPixmapItem *spriteItem;
		QTimer *animationTimer;
		QPixmap spriteImage;
		QIcon resistLevelA;
		QIcon resistLevelB;
		QIcon resistLevelC;
		QIcon resistLevelD;
		QIcon resistLevelE;
		ProjectData::DataReference<Character> *charRef;
		QList<int> weaponIDs;
		QList<int> shieldIDs;
		QList<int> armorIDs;
		QList<int> helmetIDs;
		QList<int> accessoryIDs;
		int spriteFrame;
		int spriteFrameDirection;
		int spriteNumFrames;
		bool ignoreEvents;
		bool spritePingPong;
};

#endif // DATABASE_CHARACTERS
