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

#ifndef DATABASE_CLASSES_H
#define DATABASE_CLASSES_H

class Class;

#include "ui_database_classes.h"
#include "projectdata.h"

class Database_Classes : public QWidget, public Ui::Database_Classes
{
	Q_OBJECT
	public:
		Database_Classes(QWidget *parent=0);
		~Database_Classes();
		void setupClasses();
		void shutdownClasses();
	protected slots:
		void on_lwClassList_itemSelectionChanged();
		void on_bArraySize_clicked();
		void on_leName_textChanged(QString text);
		void on_cboxSprite_currentIndexChanged(int index);
		void on_cbDuelWielding_toggled(bool on);
		void on_cbEquipmentLocked_toggled(bool on);
		void on_cbAIControlled_toggled(bool on);
		void on_cbMightyGuard_toggled(bool on);
		void on_gvMaxHP_mouseDoubleClicked();
		void on_gvMaxMP_mouseDoubleClicked();
		void on_gvAttack_mouseDoubleClicked();
		void on_gvDefense_mouseDoubleClicked();
		void on_gvWisdom_mouseDoubleClicked();
		void on_gvAgility_mouseDoubleClicked();
		void on_tbConfigureExperienceCurve_clicked();
		void on_gvExperienceGraph_mouseDoubleClicked();
		void on_cboxCommand1_currentIndexChanged(int index);
		void on_cboxCommand2_currentIndexChanged(int index);
		void on_cboxCommand3_currentIndexChanged(int index);
		void on_cboxCommand4_currentIndexChanged(int index);
		void on_cboxCommand5_currentIndexChanged(int index);
		void on_cboxCommand6_currentIndexChanged(int index);
		void on_bConfigureCommands_clicked();
		void on_twSkillList_itemSelectionChanged();
		void on_twSkillList_itemDoubleClicked(QTableWidgetItem *item);
		void on_tbDeleteSkill_clicked();
		void on_tbAddSkill_clicked();
		void on_lwConditionResist_itemClicked(QListWidgetItem *item);
		void on_lwAttributeResist_itemClicked(QListWidgetItem *item);
		void on_sbSpriteOpacity_valueChanged(int value);
		void animateSprite();
	private:
		int getCommand1();
		void setCommand1(int value);
		int getCommand2();
		void setCommand2(int value);
		int getCommand3();
		void setCommand3(int value);
		int getCommand4();
		void setCommand4(int value);
		int getCommand5();
		void setCommand5(int value);
		int getCommand6();
		void setCommand6(int value);
		void updateClassList();
		void updateCharacterSpriteList();
		void updateExperienceCurve();
		void updateSkillsList();
		void updateConditionResistList();
		void updateAttributeResistList();
		void updateCommand1List();
		void updateCommand2List();
		void updateCommand3List();
		void updateCommand4List();
		void updateCommand5List();
		void updateCommand6List();
		QList<int> command1List;
		QList<int> command2List;
		QList<int> command3List;
		QList<int> command4List;
		QList<int> command5List;
		QList<int> command6List;
		QGraphicsPixmapItem *spriteItem;
		QTimer *animationTimer;
		QPixmap spriteImage;
		QIcon resistLevelA;
		QIcon resistLevelB;
		QIcon resistLevelC;
		QIcon resistLevelD;
		QIcon resistLevelE;
		ProjectData::DataReference<Class> *classRef;
		int spriteFrame;
		int spriteFrameDirection;
		int spriteNumFrames;
		bool spritePingPong;
		bool ignoreEvents;
};

#endif // DATABASE_CLASSES_H
