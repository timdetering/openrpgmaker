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

#ifndef DATABASE_MONSTERGROUPS_H
#define DATABASE_MONSTERGROUPS_H

#include "ui_database_monstergroups.h"
#include "projectdata.h"

class Database_MonsterGroups : public QWidget, public Ui::Database_MonsterGroups
{
	Q_OBJECT
	public:
		Database_MonsterGroups(QWidget *parent=0);
		~Database_MonsterGroups();
		void setupMonsterGroups();
		void shutdownMonsterGroups();
};

#endif // DATABASE_MONSTERGROUPS_H
