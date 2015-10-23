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

#ifndef IMAGEBROWSER_H
#define IMAGEBROWSER_H

#include "ui_imagebrowser.h"

class ImageBrowser : public QDialog, public Ui::ImageBrowser
{
	Q_OBJECT
	public:
		enum BlockedFilter {BlockNone=0u, BlockBMP=1u, BlockGIF=2u, BlockJPEG=4u, BlockPBM=8u, BlockPCX=16u, BlockPGM=32u, BlockPNG=64u, BlockPNM=128u, BlockPPM=256u, BlockTGA=512u, BlockTIFF=1024u, BlockXPM=2048u, BlockALL=4095u};
		ImageBrowser(QWidget *parent=0);
		~ImageBrowser();
		void setupBrowser(QString startSubFolder, unsigned short blockFilters=BlockNone, QList<QSize> sizeRestrictions=QList<QSize>());
		QString getFileLocation();
	protected slots:
		void on_twFiles_itemCollapsed(QTreeWidgetItem *item);
		void on_twFiles_itemExpanded(QTreeWidgetItem *item);
		void on_twFiles_currentItemChanged(QTreeWidgetItem *item);
		void on_twFiles_itemActivated(QTreeWidgetItem *item);
		void on_cboxFilter_currentIndexChanged(int index);
	private:
		void setupDirectoryTree(QTreeWidgetItem *directoryItem, QList<QSize> sizeRestrictions);
		void applyFilter(QTreeWidgetItem *directory, QRegExp filter);
		unsigned short blockedFilters;
};

#endif // IMAGEBROWSER_H
