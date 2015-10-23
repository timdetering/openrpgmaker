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

#ifndef CONFIGUREEVENT_CHANGEBATTLECOMMANDS_H
#define CONFIGUREEVENT_CHANGEBATTLECOMMANDS_H

#include "ui_configureevent_changebattlecommands.h"

class ConfigureEvent_ChangeBattleCommands : public QDialog, public Ui::ConfigureEvent_ChangeBattleCommands
{
	Q_OBJECT
	public:
		ConfigureEvent_ChangeBattleCommands(QWidget *parent=0);
		~ConfigureEvent_ChangeBattleCommands();
		void setCharacterID(int characterID);
		void setIsReferencedCharacter(bool referenced);
		void setCommandID(int commandID);
		void setIsReferencedCommand(bool referenced);
		void setOperation(bool add);
		int getCharacterID();
		bool isReferencedCharacter();
		int getCommandID();
		bool isReferencedCommand();
		bool isAddOperation();
	protected slots:
		void on_bBrowseCharacterVariables_clicked();
		void on_bBrowseCommandVariables_clicked();
		void on_rbSpecificCharacter_toggled(bool checked);
		void on_rbSpecificCommand_toggled(bool checked);
};

#endif // CONFIGUREEVENT_CHANGEBATTLECOMMANDS_H
