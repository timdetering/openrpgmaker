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

#ifndef CONFIGUREEVENT_CHANGEPARTY_H
#define CONFIGUREEVENT_CHANGEPARTY_H

#include "ui_configureevent_changeparty.h"

class ConfigureEvent_ChangeParty : public QDialog, public Ui::ConfigureEvent_ChangeParty
{
	Q_OBJECT
	public:
		ConfigureEvent_ChangeParty(QWidget *parent=0);
		~ConfigureEvent_ChangeParty();
		void setOperation(bool add);
		void setCharacterID(int characterID);
		void setIsReferencedCharacter(bool referenced);
		bool isAddOperation();
		int getCharacterID();
		bool isReferencedCharacter();
	protected slots:
		void on_bBrowseCharacterVariables_clicked();
		void on_rbSpecificCharacter_toggled(bool checked);
};

#endif // CONFIGUREEVENT_CHANGEPARTY_H
