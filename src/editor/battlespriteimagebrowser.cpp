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

#include <QDir>
#include <QFileInfo>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include "battlespriteimagebrowser.h"
#include "loading.h"
#include "projectdata.h"

BattleSpriteImageBrowser::BattleSpriteImageBrowser(QString folder, QString imageLocation, int numFrames, QWidget *parent) : QDialog(parent)
{
	QTreeWidgetItem *item;
	Loading *loading = new Loading;
	loading->show();
	setupUi(this);
	item = twFiles->topLevelItem(0);
	item->setFlags(Qt::ItemIsEnabled);
	item->setText(1, "dir");
	if (item->text(0) == "Global Data")
		item->setText(2, ProjectData::datadir + "/images/" + folder);
	else
		item->setText(2, ProjectData::projectLocation + "/images/" + folder);
	item->setIcon(0, QIcon(":/icons/folder.png"));
	QCoreApplication::processEvents();
	setupDirectoryTree(item, imageLocation);
	QCoreApplication::processEvents();
	item = twFiles->topLevelItem(1);
	item->setFlags(Qt::ItemIsEnabled);
	item->setText(1, "dir");
	if (item->text(0) == "Global Data")
		item->setText(2, ProjectData::datadir + "/images/" + folder);
	else
		item->setText(2, ProjectData::projectLocation + "/images/" + folder);
	item->setIcon(0, QIcon(":/icons/folder.png"));
	QCoreApplication::processEvents();
	setupDirectoryTree(item, imageLocation);
	loading->doneLoading();
	delete loading;
	bNumFramesSideButton->setSpinBox(sbNumFrames);
	sbNumFrames->setValue(numFrames);
}

BattleSpriteImageBrowser::~BattleSpriteImageBrowser()
{
}

QString BattleSpriteImageBrowser::getFileLocation()
{
	QTreeWidgetItem *item = twFiles->currentItem();
	if (item->text(1) == "img")
		return item->text(2);
	else
		return "";
}

int BattleSpriteImageBrowser::getNumFrames()
{
	return sbNumFrames->value();
}

void BattleSpriteImageBrowser::on_twFiles_itemCollapsed(QTreeWidgetItem *item)
{
	if (item->text(1) == "dir")
		item->setIcon(0, QIcon(":/icons/folder.png"));
}

void BattleSpriteImageBrowser::on_twFiles_itemExpanded(QTreeWidgetItem *item)
{
	if (item->text(1) == "dir")
		item->setIcon(0, QIcon(":/icons/folder_open.png"));
}

void BattleSpriteImageBrowser::on_twFiles_currentItemChanged(QTreeWidgetItem *item)
{
	if (item->text(1) == "img")
		lblPreview->setPixmap(QPixmap(item->text(2)));
	else
		lblPreview->setPixmap(QPixmap());
}

void BattleSpriteImageBrowser::on_twFiles_itemActivated(QTreeWidgetItem *item)
{
	if (item->text(1) == "img")
		accept();
}

void BattleSpriteImageBrowser::setupDirectoryTree(QTreeWidgetItem *directoryItem, QString selectedImage)
{
	QDir directory(directoryItem->text(2));
	QFileInfoList files;
	directory.setFilter(QDir::AllDirs|QDir::Files|QDir::NoSymLinks|QDir::NoDotAndDotDot|QDir::Readable);
	directory.setSorting(QDir::Name|QDir::DirsFirst|QDir::IgnoreCase);
	files = directory.entryInfoList();
	for (int i = 0, numfiles = files.size(); i < numfiles; ++i)
	{
		QCoreApplication::processEvents();
		if (files[i].isDir())
		{
			QTreeWidgetItem *newDirItem = new QTreeWidgetItem();
			newDirItem->setIcon(0, QIcon(QString::fromUtf8(":/icons/folder.png")));
			newDirItem->setText(0, files[i].fileName());
			newDirItem->setText(1, "dir");
			newDirItem->setText(2, directoryItem->text(2) + QString("/") + files[i].fileName());
			newDirItem->setFlags(Qt::ItemIsEnabled);
			setupDirectoryTree(newDirItem, selectedImage);
			if (newDirItem->childCount() > 0)
				directoryItem->addChild(newDirItem);
		}
		else
		{
			QTreeWidgetItem *newFileItem = new QTreeWidgetItem();
			newFileItem->setIcon(0, QIcon(":/icons/resourcemanager.png"));
			newFileItem->setText(0, files[i].fileName());
			newFileItem->setText(1, "img");
			newFileItem->setText(2, directoryItem->text(2) + QString("/") + files[i].fileName());
			directoryItem->addChild(newFileItem);
			if (newFileItem->text(2) == selectedImage)
				newFileItem->setSelected(true);
		}
	}
}
