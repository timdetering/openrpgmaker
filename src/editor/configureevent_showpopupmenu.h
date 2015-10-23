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

#ifndef CONFIGUREEVENT_SHOWPOPUPMENU_H
#define CONFIGUREEVENT_SHOWPOPUPMENU_H

#include "ui_configureevent_showpopupmenu.h"

class ConfigureEvent_ShowPopupMenu : public QDialog, public Ui::ConfigureEvent_ShowPopupMenu
{
	Q_OBJECT
	public:
		ConfigureEvent_ShowPopupMenu(QWidget *parent=0);
		~ConfigureEvent_ShowPopupMenu();
		void setMenuID(int menuID);
		void setIsReferencedMenu(bool referenced);
		void setCoordinates(int x, int y);
		void setIsStoredCoordinates(bool stored);
		int getMenuID();
		bool isReferencedMenu();
		int getXCoordinate();
		int getYCoordinate();
		bool isStoredCoordinates();
	protected slots:
		void on_bBrowseVariables_clicked();
		void on_bBrowseXVariables_clicked();
		void on_bBrowseYVariables_clicked();
		void on_rbSpecificCoordinates_toggled(bool checked);
		void on_rbSpecificMenu_toggled(bool checked);
};

#endif // CONFIGUREEVENT_SHOWPOPUPMENU_H
