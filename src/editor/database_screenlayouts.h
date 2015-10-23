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
 *  Copyright (C) 2011, Justin Davis <tuxdavis@gmail.com>                   *
 ****************************************************************************/

#ifndef DATABASE_SCREENLAYOUTS_H
#define DATABASE_SCREENLAYOUTS_H

#include "ui_database_screenlayouts.h"

class Database_ScreenLayouts : public QWidget, public Ui::Database_ScreenLayouts
{
	Q_OBJECT
	public:
		Database_ScreenLayouts(QWidget *parent=0);
		~Database_ScreenLayouts();
		void setupScreenLayouts();
		void shutdownScreenLayouts();
	protected slots:
		void on_aDeleteMenu_triggered();
		void on_aNewMainMenu_triggered();
		void on_aNewPopupMenu_triggered();
		void on_leMenuName_textEdited(const QString &text);
		void on_twLayouts_customContextMenuRequested(const QPoint &pos);
		void on_twLayouts_itemSelectionChanged();
	private:
		QTreeWidgetItem *contextMenuItem;
		bool ignoreEvents;
};

#endif // DATABASE_SCREENLAYOUTS_H
