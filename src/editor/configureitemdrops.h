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

#ifndef CONFIGUREITEMDROPS_H
#define CONFIGUREITEMDROPS_H

class Monster;

#include "ui_configureitemdrops.h"

class ConfigureItemDrops : public QDialog, public Ui::ConfigureItemDrops
{
	Q_OBJECT
	public:
		ConfigureItemDrops(QWidget *parent=0);
		~ConfigureItemDrops();
		void setupItemDrops(int monsterID);
		void getItemDrops(int monsterID);
	protected slots:
		void on_bDeleteItem_clicked();
		void on_bAddItem_clicked();
};

#endif // CONFIGUREITEMDROPS_H
