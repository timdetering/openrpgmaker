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

#ifndef MENUEVENTEDITOR_H
#define MENUEVENTEDITOR_H

class MainMenuLayout;
class PopupMenuLayout;
class TitleMenuLayout;

#include "ui_menueventeditor.h"
#include "projectdata.h"

class MenuEventEditor : public QDialog, public Ui::MenuEventEditor
{
	Q_OBJECT
	public:
		enum MenuEventType
		{
			MENUEVENT_ONACCEPT=0,
			MENUEVENT_ONCANCEL,
			MENUEVENT_ONLOAD,
			MENUEVENT_ONLEAVE,
			MENUEVENT_UPARROW,
			MENUEVENT_DOWNARROW,
			MENUEVENT_LEFTARROW,
			MENUEVENT_RIGHTARROW
		};
		enum ItemListEventType
		{
			ITEMLIST_SELECTIONCHANGED=0,
			ITEMLIST_ONACCEPT,
			ITEMLIST_ONCANCEL
		};
		enum SelectableAreaEventType
		{
			SELECTABLEAREA_ONHIGHLIGHT=0,
			SELECTABLEAREA_ONLEFTARROW,
			SELECTABLEAREA_ONRIGHTARROW,
			SELECTABLEAREA_ONUPARROW,
			SELECTABLEAREA_ONDOWNARROW,
			SELECTABLEAREA_ONSELECTED,
			SELECTABLEAREA_ONCANCEL
		};
		MenuEventEditor(int id, int type, QWidget *parent=0);
		~MenuEventEditor();
	protected slots:
		void on_twObjects_itemSelectionChanged();
	private:
		void setupEventTree();
		ProjectData::DataReference<MainMenuLayout> *mainRef;
		ProjectData::DataReference<PopupMenuLayout> *popupRef;
		TitleMenuLayout *titleMenuLayout;
		int menuID;
};

#endif // MENUEVENTEDITOR_H
