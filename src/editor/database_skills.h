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

#ifndef DATABASE_SKILLS_H
#define DATABASE_SKILLS_H

class Skill;

#include "ui_database_skills.h"
#include "projectdata.h"
#include "projectdata_skill.h"

class Database_Skills : public QWidget, public Ui::Database_Skills
{
	Q_OBJECT
	public:
		Database_Skills(QWidget *parent=0);
		~Database_Skills();
		void setupSkills();
		void shutdownSkills();
	protected slots:
		void on_lwSkillList_itemSelectionChanged();
		void on_bArraySize_clicked();
		void on_leName_textChanged(QString text);
		void on_cboxType_currentIndexChanged(int index);
		void on_bConfigureTypes_clicked();
		void on_rbNumber_toggled(bool on);
		void on_normal_leDescription_textChanged(QString text);
		void on_normal_cboxTarget_currentIndexChanged(int index);
		void on_normal_bPlayBattleAnimationPreview_clicked();
		void on_normal_bConfigureBattleAnimationPreview_clicked();
		void on_normal_bSetAnimation_clicked();
		void on_normal_hsAttackInfluence_valueChanged(int value);
		void on_normal_hsWisdomInfluence_valueChanged(int value);
		void on_normal_hsVariance_valueChanged(int value);
		void on_switch_leDescription_textChanged(QString text);
		void on_switch_bBrowseSoundEffect_clicked();
		void on_switch_lwSwitch_itemDoubleClicked();
		void on_switch_bBrowseSwitches_clicked();
		void on_escapeTeleport_leDescription_textChanged(QString text);
		void on_escapeTeleport_bBrowseSoundEffect_clicked();
	private:
		void updateSkillList();
		void updateTypeList();
		void normal_updateAttributeList();
		void normal_updateConditionList();
		void applySkillData();
		ProjectData::DataReference<Skill> *skillRef;
		Skill::NormalData normalData;
		Skill::EscapeTeleportData escapeTeleportData;
		Skill::SwitchData switchData;
		bool ignoreEvents;
};

#endif // DATABASE_SKILLS_H
