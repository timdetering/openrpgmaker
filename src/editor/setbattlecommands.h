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

#ifndef SETBATTLECOMMANDS_H
#define SETBATTLECOMMANDS_H

#include "ui_setbattlecommands.h"

class SetBattleCommands : public QDialog, public Ui::SetBattleCommands
{
	Q_OBJECT
	public:
		SetBattleCommands(QWidget *parent=0);
		~SetBattleCommands();
		int getCommand1();
		void setCommand1(int value);
		int getCommand2();
		void setCommand2(int value);
		int getCommand3();
		void setCommand3(int value);
		int getCommand4();
		void setCommand4(int value);
		int getCommand5();
		void setCommand5(int value);
		int getCommand6();
		void setCommand6(int value);
	protected slots:
		void on_cboxCommand1_currentIndexChanged(int index);
		void on_cboxCommand2_currentIndexChanged(int index);
		void on_cboxCommand3_currentIndexChanged(int index);
		void on_cboxCommand4_currentIndexChanged(int index);
		void on_cboxCommand5_currentIndexChanged(int index);
		void on_cboxCommand6_currentIndexChanged(int index);
		void on_bConfigureCommands_clicked();
	private:
		void updateCommand1List();
		void updateCommand2List();
		void updateCommand3List();
		void updateCommand4List();
		void updateCommand5List();
		void updateCommand6List();
		QList<int> command1List;
		QList<int> command2List;
		QList<int> command3List;
		QList<int> command4List;
		QList<int> command5List;
		QList<int> command6List;
		bool ignoreEvents;
};

#endif // SETBATTLECOMMANDS_H
