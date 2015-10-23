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

#ifndef CONFIGUREEVENT_CHANGECHARACTERCONDITION_H
#define CONFIGUREEVENT_CHANGECHARACTERCONDITION_H

#include "ui_configureevent_changecharactercondition.h"

class ConfigureEvent_ChangeCharacterCondition : public QDialog, public Ui::ConfigureEvent_ChangeCharacterCondition
{
	Q_OBJECT
	public:
		enum Target {ENTIRE_PARTY=0, SPECIFIC_CHARACTER, REFERENCED_CHARACTER};
		ConfigureEvent_ChangeCharacterCondition(QWidget *parent=0);
		~ConfigureEvent_ChangeCharacterCondition();
		void setTarget(int target);
		void setCharacterID(int characterID);
		void setOperation(bool add);
		void setConditionID(int conditionID);
		void setIsReferencedCondition(bool referenced);
		int getTarget();
		int getCharacterID();
		bool isAddOperation();
		int getConditionID();
		bool isReferencedCondition();
	protected slots:
		void on_bBrowseCharacterVariables_clicked();
		void on_bBrowseConditionVariables_clicked();
		void on_rbReferencedCharacter_toggled(bool checked);
		void on_rbSpecificCharacter_toggled(bool checked);
		void on_rbSpecificCondition_toggled(bool checked);
};

#endif // CONFIGUREEVENT_CHANGECHARACTERCONDITION_H
