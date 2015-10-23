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
#include "imagebrowser.h"
#include "loading.h"
#include "projectdata.h"

ImageBrowser::ImageBrowser(QWidget *parent) : QDialog(parent)
{
	setupUi(this);
}

ImageBrowser::~ImageBrowser()
{
}

void ImageBrowser::setupBrowser(QString startSubFolder, unsigned short blockFilters, QList<QSize> sizeRestrictions)
{
	QTreeWidgetItem *item;
	Loading *loading = new Loading;
	loading->show();
	blockedFilters = blockFilters;
	if ((blockedFilters & BlockBMP) == 0)
		cboxFilter->addItem("BMP");
	if ((blockedFilters & BlockGIF) == 0)
		cboxFilter->addItem("GIF");
	if ((blockedFilters & BlockJPEG) == 0)
		cboxFilter->addItem("JPEG");
	if ((blockedFilters & BlockPBM) == 0)
		cboxFilter->addItem("PBM");
	if ((blockedFilters & BlockPGM) == 0)
		cboxFilter->addItem("PGM");
	if ((blockedFilters & BlockPNG) == 0)
		cboxFilter->addItem("PNG");
	if ((blockedFilters & BlockPNM) == 0)
		cboxFilter->addItem("PNM");
	if ((blockedFilters & BlockPPM) == 0)
		cboxFilter->addItem("PPM");
	if ((blockedFilters & BlockXPM) == 0)
		cboxFilter->addItem("XPM");
	if ((blockedFilters & BlockTIFF) == 0)
		cboxFilter->addItem("TIFF");
	item = twFiles->topLevelItem(0);
	item->setFlags(Qt::ItemIsEnabled);
	item->setText(1, "dir");
	if (item->text(0) == "Global Data")
		item->setText(2, ProjectData::datadir + "/images" + startSubFolder);
	else
		item->setText(2, ProjectData::projectLocation + "/images" + startSubFolder);
	item->setIcon(0, QIcon(":/icons/folder.png"));
	QCoreApplication::processEvents();
	setupDirectoryTree(item, sizeRestrictions);
	QCoreApplication::processEvents();
	item = twFiles->topLevelItem(1);
	item->setFlags(Qt::ItemIsEnabled);
	item->setText(1, "dir");
	if (item->text(0) == "Global Data")
		item->setText(2, ProjectData::datadir + "/images" + startSubFolder);
	else
		item->setText(2, ProjectData::projectLocation + "/images" + startSubFolder);
	item->setIcon(0, QIcon(":/icons/folder.png"));
	QCoreApplication::processEvents();
	setupDirectoryTree(item, sizeRestrictions);
	loading->doneLoading();
	delete loading;
}

QString ImageBrowser::getFileLocation()
{
	QTreeWidgetItem *item = twFiles->currentItem();
	if (item->text(1) == "img")
		return item->text(2);
	else
		return "";
}

void ImageBrowser::on_twFiles_itemCollapsed(QTreeWidgetItem *item)
{
	if (item->text(1) == "dir")
		item->setIcon(0, QIcon(":/icons/folder.png"));
}

void ImageBrowser::on_twFiles_itemExpanded(QTreeWidgetItem *item)
{
	if (item->text(1) == "dir")
		item->setIcon(0, QIcon(":/icons/folder_open.png"));
}

void ImageBrowser::on_twFiles_currentItemChanged(QTreeWidgetItem *item)
{
	if (item->text(1) == "img")
		lblPreview->setPixmap(QPixmap(item->text(2)));
	else
		lblPreview->setPixmap(QPixmap());
}

void ImageBrowser::on_twFiles_itemActivated(QTreeWidgetItem *item)
{
	if (item->text(1) == "img")
		accept();
}

void ImageBrowser::on_cboxFilter_currentIndexChanged(int index)
{
	if (index == 0)
	{
		applyFilter(twFiles->topLevelItem(0), QRegExp("*", Qt::CaseInsensitive, QRegExp::Wildcard));
		applyFilter(twFiles->topLevelItem(1), QRegExp("*", Qt::CaseInsensitive, QRegExp::Wildcard));
	}
	else
	{
		applyFilter(twFiles->topLevelItem(0), QRegExp(QString("*.%1").arg(cboxFilter->itemText(index).toLower()), Qt::CaseInsensitive, QRegExp::Wildcard));
		applyFilter(twFiles->topLevelItem(1), QRegExp(QString("*.%1").arg(cboxFilter->itemText(index).toLower()), Qt::CaseInsensitive, QRegExp::Wildcard));
	}
}

void ImageBrowser::setupDirectoryTree(QTreeWidgetItem *directoryItem, QList<QSize> sizeRestrictions)
{
	QDir directory(directoryItem->text(2));
	QFileInfoList files;
	QStringList fileFilters;
	if ((blockedFilters & BlockBMP) == 0)
		fileFilters << "*.bmp" << "*.BMP";
	if ((blockedFilters & BlockGIF) == 0)
		fileFilters << "*.gif" << "*.GIF";
	if ((blockedFilters & BlockJPEG) == 0)
	{
		fileFilters << "*.jpg" << "*.JPG";
		fileFilters << "*.jpeg" << "*.JPEG";
	}
	if ((blockedFilters & BlockPBM) == 0)
		fileFilters << "*.pbm" << "*.PBM";
	if ((blockedFilters & BlockPGM) == 0)
		fileFilters << "*.pgm" << "*.PGM";
	if ((blockedFilters & BlockPNG) == 0)
		fileFilters << "*.png" << "*.PNG";
	if ((blockedFilters & BlockPNM) == 0)
		fileFilters << "*.pnm" << "*.PNM";
	if ((blockedFilters & BlockPPM) == 0)
		fileFilters << "*.ppm" << "*.PPM";
	if ((blockedFilters & BlockXPM) == 0)
		fileFilters << "*.xpm" << "*.XPM";
	if ((blockedFilters & BlockTIFF) == 0)
	{
		fileFilters << "*.tif" << "*.TIF";
		fileFilters << "*.tiff" << "*.TIFF";
	}
	directory.setFilter(QDir::AllDirs|QDir::Files|QDir::NoSymLinks|QDir::NoDotAndDotDot|QDir::Readable);
	directory.setNameFilters(fileFilters);
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
			setupDirectoryTree(newDirItem, sizeRestrictions);
			if (newDirItem->childCount() > 0)
				directoryItem->addChild(newDirItem);
		}
		else
		{
			if (sizeRestrictions.size() > 0)
			{
				QImage image(files[i].absoluteFilePath());
				QSize size(image.width(), image.height());
				if (sizeRestrictions.contains(size))
				{
					QTreeWidgetItem *newFileItem = new QTreeWidgetItem();
					newFileItem->setIcon(0, QIcon(":/icons/resourcemanager.png"));
					newFileItem->setText(0, files[i].fileName());
					newFileItem->setText(1, "img");
					newFileItem->setText(2, directoryItem->text(2) + QString("/") + files[i].fileName());
					directoryItem->addChild(newFileItem);
				}
			}
			else
			{
				QTreeWidgetItem *newFileItem = new QTreeWidgetItem();
				newFileItem->setIcon(0, QIcon(":/icons/resourcemanager.png"));
				newFileItem->setText(0, files[i].fileName());
				newFileItem->setText(1, "img");
				newFileItem->setText(2, directoryItem->text(2) + QString("/") + files[i].fileName());
				directoryItem->addChild(newFileItem);
			}
		}
	}
}

void ImageBrowser::applyFilter(QTreeWidgetItem *directory, QRegExp filter)
{
	for (int i = 0, count = directory->childCount(); i < count; ++i)
	{
		QTreeWidgetItem *child = directory->child(i);
		if (child->text(1) == "dir")
			applyFilter(child, filter);
		else
			child->setHidden(!filter.exactMatch(child->text(0)));
	}
}
