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

#ifndef TILEBROWSER_H
#define TILEBROWSER_H

#include "ui_tilebrowser.h"


class TileBrowser : public QDialog, public Ui::TileBrowser
{
	Q_OBJECT
	public:
		enum
		{
			ALL = 0,
			BMP = 1,
			GIF = 2,
			JPEG = 3,
			PNG = 4,
			PNM = 5,
			XPM = 6,
			TIFF = 7
		}; // Filters
		TileBrowser(int numtiles, QDialog *parent = 0);
		QStringList getTileLocations();
	private slots:
		void on_aSelectAllImages_triggered();
		void on_aSelectAllImagesRecursively_triggered();
		void on_filterSelector_currentIndexChanged(int index);
		void on_tileList_itemExpanded(QTreeWidgetItem *item);
		void on_tileList_itemCollapsed(QTreeWidgetItem *item);
		void on_tileList_itemSelectionChanged();
		void on_tileList_customContextMenuRequested(const QPoint &pos);
	private:
		void setupDirectoryTree(QTreeWidgetItem *directoryItem);
		void applyFilter(QTreeWidgetItem *directory, int filter);
		void selectTiles(QTreeWidgetItem *directory, bool recursive);
		QTreeWidgetItem *tempDir;
		int availableTiles;
};

#endif // TILEBROWSER_H
