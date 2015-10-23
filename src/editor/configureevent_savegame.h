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

#ifndef CONFIGUREEVENT_SAVEGAME_H
#define CONFIGUREEVENT_SAVEGAME_H

#include "ui_configureevent_savegame.h"

class ConfigureEvent_SaveGame : public QDialog, public Ui::ConfigureEvent_SaveGame
{
	Q_OBJECT
	public:
		ConfigureEvent_SaveGame(QWidget *parent=0);
		~ConfigureEvent_SaveGame();
		void setSaveSlotID(int saveSlotID);
		void setIsReferencedSaveSlot(bool referenced);
		int getSaveSlotID();
		bool isReferencedSaveSlot();
	protected slots:
		void on_rbSpecificSaveSlot_toggled(bool checked);
		void on_bBrowseVariables_clicked();
};

#endif // CONFIGUREEVENT_SAVEGAME_H
