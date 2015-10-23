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

#include <QMenu>
#include <QFileInfoList>
#include <QDir>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include "loading.h"
#include "projectdata.h"
#include "tilebrowser.h"
#include "xmlParser.h"

TileBrowser::TileBrowser(int numtiles, QDialog *parent) : QDialog(parent)
{
	setupUi(this);
	QTreeWidgetItem *item;
	Loading *loading = new Loading("Locating tiles");
	loading->show();
	item = tileList->topLevelItem(0);
	item->setFlags(Qt::ItemIsEnabled);
	item->setText(1, "DIR");
	item->setText(2, "");
	if (item->text(0) == tr("Project Data"))
		item->setText(3, "local::/images/tiles");
	else
		item->setText(3, "global::/images/tiles");
	setupDirectoryTree(item);
	item = tileList->topLevelItem(1);
	item->setFlags(Qt::ItemIsEnabled);
	item->setText(1, "DIR");
	item->setText(2, "");
	if (item->text(0) == tr("Project Data"))
		item->setText(3, "local::/images/tiles");
	else
		item->setText(3, "global::/images/tiles");
	setupDirectoryTree(item);
	availableTiles = numtiles;
	statusBar->setText(tr("You have 0/%1 tiles selected.").arg(availableTiles));
	QHeaderView *header = tileList->header();
	header->resizeSection(0, 240);
	header->resizeSection(1, 53);
	header->resizeSection(2, 118);
	loading->doneLoading();
	delete loading;
}

QStringList TileBrowser::getTileLocations()
{
	QList<QTreeWidgetItem *> selectedItems = tileList->selectedItems();
	QStringList tileLocations;
	int numSelectedItems = selectedItems.size();
	for (int i = 0; i < numSelectedItems; ++i)
		tileLocations.append(selectedItems[i]->text(3));
	return tileLocations;
}

void TileBrowser::on_aSelectAllImages_triggered()
{
	selectTiles(tempDir, false);
}

void TileBrowser::on_aSelectAllImagesRecursively_triggered()
{
	selectTiles(tempDir, true);
}

void TileBrowser::on_filterSelector_currentIndexChanged(int index)
{
	applyFilter(tileList->topLevelItem(0), index);
	applyFilter(tileList->topLevelItem(1), index);
}

void TileBrowser::on_tileList_itemExpanded(QTreeWidgetItem *item)
{
	item->setIcon(0, QIcon(QString::fromUtf8(":/icons/folder_open.png")));
}

void TileBrowser::on_tileList_itemCollapsed(QTreeWidgetItem *item)
{
	item->setIcon(0, QIcon(QString::fromUtf8(":/icons/folder.png")));
}

void TileBrowser::on_tileList_itemSelectionChanged()
{
	int numSelectedTiles = tileList->selectedItems().size();
	if (numSelectedTiles <= availableTiles)
	{
		buttonBox->button(QDialogButtonBox::Ok)->setEnabled(numSelectedTiles > 0);
		statusBar->setText(tr("You have %1/%2 tiles currently selected.").arg(numSelectedTiles).arg(availableTiles));
	}
	else
	{
		buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
		statusBar->setText(tr("You have <font color=\"#FF0000\">%1/%2</font> tiles currently selected.").arg(numSelectedTiles).arg(availableTiles));
	}
}

void TileBrowser::on_tileList_customContextMenuRequested(const QPoint &pos)
{
	tempDir = tileList->itemAt(pos);
	if (tempDir and tempDir->text(1) == "DIR")
	{
		QMenu *popupMenu = new QMenu(this);
		popupMenu->addAction(aSelectAllImages);
		popupMenu->addAction(aSelectAllImagesRecursively);
		popupMenu->exec(tileList->mapToGlobal(pos));
	}
}

void TileBrowser::setupDirectoryTree(QTreeWidgetItem *directoryItem)
{
	QDir directory(ProjectData::getAbsoluteResourcePath(directoryItem->text(3)));
	QFileInfoList files;
	QStringList fileFilters;
	int numfiles, numimages = 0, numdirectories = 0;
	fileFilters << "*.bmp" << "*.BMP";
	fileFilters << "*.gif" << "*.GIF";
	fileFilters << "*.jpg" << "*.JPG";
	fileFilters << "*.jpeg" << "*.JPEG";
	fileFilters << "*.png" << "*.PNG";
	fileFilters << "*.pbm" << "*.PBM";
	fileFilters << "*.pgm" << "*.PGM";
	fileFilters << "*.ppm" << "*.PPM";
	fileFilters << "*.xpm" << "*.XPM";
	fileFilters << "*.tiff" << "*.TIFF";
	directory.setFilter(QDir::AllDirs|QDir::Files|QDir::NoSymLinks|QDir::NoDotAndDotDot|QDir::Readable);
	directory.setNameFilters(fileFilters);
	directory.setSorting(QDir::Name|QDir::DirsFirst|QDir::IgnoreCase);
	files = directory.entryInfoList();
	numfiles = files.size();
	for (int i = 0; i < numfiles; ++i)
	{
		QCoreApplication::processEvents();
		if (files[i].isDir())
		{
			QTreeWidgetItem *newDirItem = new QTreeWidgetItem();
			newDirItem->setIcon(0, QIcon(QString::fromUtf8(":/icons/folder.png")));
			newDirItem->setText(0, files[i].fileName());
			newDirItem->setText(1, "DIR");
			newDirItem->setText(3, directoryItem->text(3) + QString("/") + files[i].fileName());
			newDirItem->setFlags(Qt::ItemIsEnabled);
			setupDirectoryTree(newDirItem);
			if (newDirItem->childCount() > 0)
				directoryItem->addChild(newDirItem);
			++numdirectories;
		}
		else
		{
			QPixmap icon(files[i].filePath());
			QString tempString;
			int width = icon.width();
			int height = icon.height();
			QTreeWidgetItem *newFileItem = new QTreeWidgetItem();
			newFileItem->setIcon(0, icon.copy(0, 0, 32, 32));
			newFileItem->setText(0, files[i].fileName());
			QString fileType = files[i].suffix().toLower();
			if (fileType == "bmp")
				newFileItem->setText(1, "BMP");
			else if (fileType == "gif")
				newFileItem->setText(1, "GIF");
			else if (fileType == "jpg" or fileType == "jpeg")
				newFileItem->setText(1, "JPEG");
			else if (fileType == "png")
				newFileItem->setText(1, "PNG");
			else if (fileType == "pbm" or fileType == "pgm" or fileType == "ppm")
				newFileItem->setText(1, "PNM");
			else if (fileType == "xpm")
				newFileItem->setText(1, "XPM");
			else if (fileType == "tiff")
				newFileItem->setText(1, "TIFF");
			else
				newFileItem->setText(1, "DELETE");
			newFileItem->setText(3, directoryItem->text(3) + QString("/") + files[i].fileName());
			directoryItem->addChild(newFileItem);
			tempString = "";
			if (width > 32)
				tempString += "Animated ";
			if (height == 160)
				tempString += "Auto Tile";
			else if (height == 32)
				tempString += "Single Tile";
			else
				tempString = "DELETE";
			newFileItem->setText(2, tempString);
			if (newFileItem->text(1) == "DELETE" || newFileItem->text(2) == "DELETE")
			{
				directoryItem->removeChild(newFileItem);
				--numimages;
			}
			++numimages;
		}
	}
	directoryItem->setText(2, tr("%1 Images, %2 Folders").arg(numimages).arg(numdirectories));
}

void TileBrowser::applyFilter(QTreeWidgetItem *directory, int filter)
{
	QTreeWidgetItem *item;
	int numchildren = directory->childCount();
	int i;
	for (i = 0; i < numchildren; ++i)
	{
		item = directory->child(i);
		if (item->text(1) == "DIR")
			applyFilter(item, filter);
		else if (filter == ALL)
			item->setHidden(false);
		else if (filter == BMP)
			item->setHidden(item->text(1) != "BMP");
		else if (filter == GIF)
			item->setHidden(item->text(1) != "GIF");
		else if (filter == JPEG)
			item->setHidden(item->text(1) != "JPEG");
		else if (filter == PNG)
			item->setHidden(item->text(1) != "PNG");
		else if (filter == PNM)
			item->setHidden(item->text(1) != "PNM");
		else if (filter == XPM)
			item->setHidden(item->text(1) != "XPM");
		else if (filter == TIFF)
			item->setHidden(item->text(1) != "TIFF");
	}
}

void TileBrowser::selectTiles(QTreeWidgetItem *directory, bool recursive)
{
	int numchildren = directory->childCount();
	for (int i = 0; i < numchildren; ++i)
	{
		QTreeWidgetItem *item = directory->child(i);
		if (item and item->text(1) != "DIR")
			item->setSelected(true);
		else if (recursive)
			selectTiles(item, true);
	}
}
