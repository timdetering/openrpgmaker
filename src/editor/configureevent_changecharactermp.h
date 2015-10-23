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

#ifndef CONFIGUREEVENT_CHANGECHARACTERMP_H
#define CONFIGUREEVENT_CHANGECHARACTERMP_H

#include "ui_configureevent_changecharactermp.h"

class ConfigureEvent_ChangeCharacterMP : public QDialog, public Ui::ConfigureEvent_ChangeCharacterMP
{
	Q_OBJECT
	public:
		enum Target {ENTIRE_PARTY=0, SPECIFIC_CHARACTER, REFERENCED_CHARACTER};
		ConfigureEvent_ChangeCharacterMP(QWidget *parent=0);
		~ConfigureEvent_ChangeCharacterMP();
		void setTarget(int target);
		void setCharacterID(int characterID);
		void setOperation(bool increase);
		void setAmount(int amount);
		void setIsStoredAmount(bool storedAmount);
		int getTarget();
		int getCharacterID();
		bool isIncreaseOperation();
		int getAmount();
		bool isStoredAmount();
	protected slots:
		void on_bBrowseAmountVariables_clicked();
		void on_bBrowseCharacterVariables_clicked();
		void on_rbReferencedCharacter_toggled(bool checked);
		void on_rbSpecificAmount_toggled(bool checked);
		void on_rbSpecificCharacter_toggled(bool checked);
};

#endif // CONFIGUREEVENT_CHANGECHARACTERMP_H
