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

#ifndef DATABASE_COMMONEVENTS_H
#define DATABASE_COMMONEVENTS_H

#include "ui_database_commonevents.h"
#include "projectdata.h"

class Database_CommonEvents : public QWidget, public Ui::Database_CommonEvents
{
	Q_OBJECT
	public:
		Database_CommonEvents(QWidget *parent=0);
		~Database_CommonEvents();
		void setupCommonEvents();
		void shutdownCommonEvents();
};

#endif // DATABASE_COMMONEVENTS_H
