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
 *  Copyright (C) 2013, Justin Davis <tuxdavis@gmail.com>                   *
 ****************************************************************************/

#ifndef MENUNAVIGATIONLIST_H
#define MENUNAVIGATIONLIST_H

class Font;

#include <QListWidget>
#include <QPoint>

class MenuNavigationList : public QListWidget
{
	public:
		MenuNavigationList(QWidget *parent=0);
		~MenuNavigationList();
		void setFont(Font *f);
		void addMenuObjects(QStringList objectNames);
		void enableMenuObject(QString name);
		void disableMenuObject(QString name);
	protected:
		void mousePressEvent(QMouseEvent *event);
		void mouseMoveEvent(QMouseEvent *event);
	private:
		Font *font;
		QPoint dragStartPos;
};

#endif // MENUNAVIGATIONLIST_H
