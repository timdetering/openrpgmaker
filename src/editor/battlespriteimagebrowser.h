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

#ifndef BATTLESPRITEIMAGEBROWSER_H
#define BATTLESPRITEIMAGEBROWSER_H

#include "ui_battlespriteimagebrowser.h"

class BattleSpriteImageBrowser : public QDialog, public Ui::BattleSpriteImageBrowser
{
	Q_OBJECT
	public:
		BattleSpriteImageBrowser(QString folder, QString imageLocation="", int numFrames=1, QWidget *parent=0);
		~BattleSpriteImageBrowser();
		QString getFileLocation();
		int getNumFrames();
	protected slots:
		void on_twFiles_itemCollapsed(QTreeWidgetItem *item);
		void on_twFiles_itemExpanded(QTreeWidgetItem *item);
		void on_twFiles_currentItemChanged(QTreeWidgetItem *item);
		void on_twFiles_itemActivated(QTreeWidgetItem *item);
	private:
		void setupDirectoryTree(QTreeWidgetItem *directoryItem, QString selectedImage);
};

#endif // BATTLESPRITEIMAGEBROWSER_H
