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

#ifndef MENUOBJECTDELEGATE_H
#define MENUOBJECTDELEGATE_H

#include <QStyledItemDelegate>
#include <QWidget>

class MenuFrame;
class MenuImage;
class MenuItemTable;
class MenuObject;
class MenuSelectableArea;
class MenuScrollArea;
class MenuText;

class MenuObjectDelegate : public QStyledItemDelegate
{
	Q_OBJECT
	public:
		MenuObjectDelegate(QObject *parent=0);
		~MenuObjectDelegate();
		void setCurrentObject(MenuObject *menuObject);
		QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;
		void setEditorData(QWidget *editor, const QModelIndex &index) const;
		void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;
		void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const;
	private:
		union
		{
			MenuFrame *frame;
			MenuImage *image;
			MenuItemTable *itemList;
			MenuScrollArea *scrollArea;
			MenuSelectableArea *selectableArea;
			MenuText *text;
		} object;
		int objectType;
};

#endif // MENUOBJECTDELEGATE_H
