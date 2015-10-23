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

#ifndef CONFIGUREEVENT_SCROLLAREA_H
#define CONFIGUREEVENT_SCROLLAREA_H

#include "ui_configureevent_scrollarea.h"
#include "projectdata.h"

class ConfigureEvent_ScrollArea : public QDialog, public Ui::ConfigureEvent_ScrollArea
{
	Q_OBJECT
	public:
		enum ScrollTo {CHILD_OBJECT=0, SPECIFIC_LOCATION, STORED_LOCATION};
		ConfigureEvent_ScrollArea(int menu, int type, QWidget *parent=0);
		~ConfigureEvent_ScrollArea();
		void setScrollArea(QString objectName);
		void setScrollTo(int scrollTo);
		void setChildObject(QString objectName);
		void setLocation(int x, int y);
		QString getScrollArea();
		int getScrollTo();
		QString getChildObject();
		int getXLocation();
		int getYLocation();
	protected slots:
		void on_bBrowseXVariables_clicked();
		void on_bBrowseYVariables_clicked();
		void on_cboxScrollArea_currentIndexChanged();
		void on_rbChildObject_toggled(bool checked);
		void on_rbSpecificLocation_toggled(bool checked);
		void on_rbStoredLocation_toggled(bool checked);
	private:
		ProjectData::DataReference<MainMenuLayout> *mainMenuRef;
		ProjectData::DataReference<PopupMenuLayout> *popupMenuRef;
		TitleMenuLayout *titleMenuLayout;
		int menuID;
		int menuType;
};

#endif // CONFIGUREEVENT_SCROLLAREA_H
