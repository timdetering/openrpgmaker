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

#ifndef CONFIGUREEVENT_CHANGECLASS_H
#define CONFIGUREEVENT_CHANGECLASS_H

#include "ui_configureevent_changeclass.h"

class ConfigureEvent_ChangeClass : public QDialog, public Ui::ConfigureEvent_ChangeClass
{
	Q_OBJECT
	public:
		enum LearnedSkills {LEARNED_NONE=0, LEARNED_ADDUPTONEWDELETEOLD, LEARNED_ADDUPTONEWKEEPOLD};
		enum BaseStatChange {CHANGE_NONE=0, CHANGE_HALVESTATS, CHANGE_TOLEVEL1OFCLASS, CHANGE_TOCURRENTLEVELOFCLASS};
		ConfigureEvent_ChangeClass(QWidget *parent=0);
		~ConfigureEvent_ChangeClass();
		void setCharacterID(int characterID);
		void setIsReferencedCharacter(bool referenced);
		void setClassID(int classID);
		void setIsReferencedClass(bool referenced);
		void setRetainCurrentLevel(bool retainCurrentLevel);
		void setBaseStatChange(int statChange);
		void setLearnedSkills(int learnedSkills);
		void setShowLevelUpMessage(bool showMessage);
		int getCharacterID();
		bool isReferencedCharacter();
		int getClassID();
		bool isReferencedClass();
		bool retainCurrentLevel();
		int getBaseStatChange();
		int getLearnedSkills();
		bool showLevelUpMessage();
	protected slots:
		void on_bBrowseCharacterVariables_clicked();
		void on_bBrowseClassVariables_clicked();
		void on_rbSpecificCharacter_toggled(bool checked);
		void on_rbSpecificClass_toggled(bool checked);
};

#endif // CONFIGUREEVENT_CHANGECLASS_H
