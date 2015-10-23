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

#ifndef CONFIGUREMONSTERBEHAVIORACTION_H
#define CONFIGUREMONSTERBEHAVIORACTION_H

class MonsterAnimation;

#include "projectdata.h"
#include "projectdata_monster.h"
#include "ui_configuremonsterbehavioraction.h"

class ConfigureMonsterBehaviorAction : public QDialog, public Ui::ConfigureMonsterBehaviorAction
{
	Q_OBJECT
	public:
		ConfigureMonsterBehaviorAction(int monsterAnimationID, int hueShift, QWidget *parent=0);
		~ConfigureMonsterBehaviorAction();
		void setupMonsterBehaviorAction();
		void setupMonsterBehaviorAction(const Monster::BehaviorAction &action);
		void getMonsterBehaviorAction(Monster::BehaviorAction *action);
	public slots:
		void on_cboxPrecondition_currentIndexChanged(int index);
		void on_lwPreconditionSwitchID_itemDoubleClicked();
		void on_tbBrowsePreconditionSwitch_clicked();
		void on_cbTurnSwitchOn_toggled(bool on);
		void on_lwTurnSwitchOn_itemDoubleClicked();
		void on_tbBrowseTurnSwitchOn_clicked();
		void on_cbTurnSwitchOff_toggled(bool on);
		void on_lwTurnSwitchOff_itemDoubleClicked();
		void on_tbBrowseTurnSwitchOff_clicked();
		void on_rbBasic_toggled(bool on);
		void on_rbSkill_toggled(bool on);
		void on_rbTransform_toggled(bool on);
		void on_cboxMonsterAnimation_currentIndexChanged();
		void animateMonster();
	private:
		void renderMonsterAnimation();
		void loadMonsterAnimation();
		void updateMonsterAnimationList();
		void updateSkillList();
		void updateMonsterList();
		QGraphicsPixmapItem *monsterItem;
		QTimer *animationTimer;
		QImage monsterImage;
		ProjectData::DataReference<MonsterAnimation> *animRef;
		int monsterFrame;
		int monsterFrameDirection;
		int monsterHueShift;
};

#endif // CONFIGUREMONSTERBEHAVIORACTION_H
