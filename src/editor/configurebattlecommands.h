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

#ifndef CONFIGUREBATTLECOMMANDS_H
#define CONFIGUREBATTLECOMMANDS_H

#include "projectdata.h"
#include "ui_configurebattlecommands.h"

class ConfigureBattleCommands : public QDialog, public Ui::ConfigureBattleCommands
{
	Q_OBJECT
	public:
		ConfigureBattleCommands(QWidget *parent=0);
		~ConfigureBattleCommands();
	protected slots:
		void on_lwCommandList_itemSelectionChanged();
		void on_bArraySize_clicked();
		void on_leName_textChanged(QString text);
		void on_cboxType_currentIndexChanged(int index);
		void on_buttonBox_clicked(QAbstractButton *button);
	private:
		void updateCommandList();
		QList<ProjectData::BattleCommand*> battleCommands;
		bool ignoreEvents;
};

#endif // CONFIGUREBATTLECOMMANDS
