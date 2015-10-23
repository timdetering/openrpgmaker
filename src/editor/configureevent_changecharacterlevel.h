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

#ifndef CONFIGUREEVENT_CHANGECHARACTERLEVEL_H
#define CONFIGUREEVENT_CHANGECHARACTERLEVEL_H

#include "ui_configureevent_changecharacterlevel.h"

class ConfigureEvent_ChangeCharacterLevel : public QDialog, public Ui::ConfigureEvent_ChangeCharacterLevel
{
	Q_OBJECT
	public:
		enum Target {ENTIRE_PARTY=0, SPECIFIC_CHARACTER, REFERENCED_CHARACTER};
		ConfigureEvent_ChangeCharacterLevel(QWidget *parent=0);
		~ConfigureEvent_ChangeCharacterLevel();
		void setTarget(int target);
		void setCharacterID(int characterID);
		void setOperation(bool add);
		void setAmount(int amount);
		void setIsStoredAmount(bool storedAmount);
		void setShowLevelUpMessage(bool showMessage);
		int getTarget();
		int getCharacterID();
		bool isAddOperation();
		int getAmount();
		bool isStoredAmount();
		bool showLevelUpMessage();
	protected slots:
		void on_bBrowseAmountVariables_clicked();
		void on_bBrowseCharacterVariables_clicked();
		void on_rbAddLevels_toggled(bool checked);
		void on_rbReferencedCharacter_toggled(bool checked);
		void on_rbSpecificAmount_toggled(bool checked);
		void on_rbSpecificCharacter_toggled(bool checked);
};

#endif // CONFIGUREEVENT_CHANGECHARACTERLEVEL_H
