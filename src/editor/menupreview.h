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

#ifndef MENUPREVIEW_H
#define MENUPREVIEW_H

#include "ui_menupreview.h"

class MainMenuLayout;
class PopupMenuLayout;
class TitleMenuLayout;
class MenuObject;

class MenuPreview : public QDialog, public Ui::MenuPreview
{
	Q_OBJECT
	public:
		enum MenuType
		{
			MENUTYPE_MAINMENU=0,
			MENUTYPE_POPUPMENU,
			MENUTYPE_TITLESCREENMENU
		};
		MenuPreview(QWidget *parent=0);
		~MenuPreview();
		void setupPreview(int menuID, int menuType);
	protected slots:
		void on_buttonBox_clicked(QAbstractButton *button);
	private:
		void renderObject(MenuObject *object, QPainter &painter);
		QPixmap menuPixmap;
};

#endif // MENUPREVIEW_H
