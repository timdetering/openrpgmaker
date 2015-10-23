/****************************************************************************
*  Open RPG MAker is free software: you can redistribute it and/or modify  *
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

#include <QImage>
#include <QColor>
#include <QRect>
#include <QDir>
#include <QFileInfo>
#include <QString>
#include <QMessageBox>
#include <QPoint>
#include <cstdio>
#include <cstdlib>
#include "projectdata.h"
#include "importerplugin.h"
#include "importerplugininterface.h"

void ImporterPlugin::initializeImporter(QString homePath)
{
	ProjectData::homePath = homePath;
	ProjectData::ImporterNode = XMLNode::createXMLTopNode("openrpgmaker-imports");
	ProjectData::recoverProject();
}

void ImporterPlugin::shutdownImporter()
{
	ProjectData::saveToStorage();
	ProjectData::saveImportedData();
}

void ImporterPlugin::drawImage(QImage src, QRect srcRect, QImage &dst, QPoint dstPoint, unsigned int src_tcolor)
{
	QRgb pixel;
	unsigned int index;
	if (src.depth() > 8)
	{
		for (int y = 0; y < srcRect.height(); ++y)
		{
			for (int x = 0; x < srcRect.width(); ++x)
			{
				pixel = src.pixel(x + srcRect.x(), y + srcRect.y());
				if (pixel == src_tcolor)
					dst.setPixel(x + dstPoint.x(), y + dstPoint.y(), qRgba(0, 0, 0, 0));
				else
					dst.setPixel(x + dstPoint.x(), y + dstPoint.y(), pixel);
			}
		}
	}
	else
	{
		for (int y = 0; y < srcRect.height(); ++y)
		{
			for (int x = 0; x < srcRect.width(); ++x)
			{
				index = src.pixelIndex(x + srcRect.x(), y + srcRect.y());
				pixel = src.pixel(x + srcRect.x(), y + srcRect.y());
				if (index == src_tcolor)
					dst.setPixel(x + dstPoint.x(), y + dstPoint.y(), qRgba(0, 0, 0, 0));
				else
					dst.setPixel(x + dstPoint.x(), y + dstPoint.y(), pixel);
			}
		}
	}
}

bool ImporterPlugin::isImageEmpty(QImage image)
{
	QRgb pixel, firstPixel = image.pixel(0, 0);
	for (int y = 0; y < image.height(); ++y)
	{
		for (int x = 0; x < image.width(); ++x)
		{
			pixel = image.pixel(x, y);
			if (pixel != firstPixel)
				return false;
		}
	}
	return true;
}

void ImporterPlugin::copyDirectory(QString dirLocation, QString newLocation)
{
	QFileInfoList fileList;
	QDir dir(dirLocation);
	QDir newDir(newLocation);
	fileList = dir.entryInfoList(QDir::Dirs|QDir::Files|QDir::Readable|QDir::NoDotAndDotDot);
	for (int i = 0; i < fileList.size(); ++i)
	{
		if (fileList[i].isDir())
		{
			dir.mkdir(fileList[i].fileName());
			copyDirectory(dirLocation + QString("/") + fileList[i].fileName(), newLocation + QString("/") + fileList[i].fileName());
		}
		else
			copyFile(dirLocation + QString("/") + fileList[i].fileName(), newLocation + QString("/") + fileList[i].fileName());
	}
}

void ImporterPlugin::copyFile(QString fileLocation, QString newLocation)
{
	FILE *file1;
	FILE *file2;
	size_t bytesRead;
	char temp_string[2048];
	file1 = fopen(fileLocation.toLocal8Bit().data(), "rb");
	if (file1 == NULL)
	{
		QMessageBox::critical(0, "Critical Error", QString("Failed to open file \"%1\" in read-only mode.").arg(fileLocation));
		return;
	}
	file2 = fopen(newLocation.toLocal8Bit().data(), "wb");
	if (file2 == NULL)
	{
		QMessageBox::critical(0, "Critical Error", QString("Failed to open file \"%1\" in write-only mode.").arg(newLocation));
		return;
	}
	while (!feof(file1))
	{
		bytesRead = fread(temp_string, 1, sizeof(temp_string), file1);
		fwrite(temp_string, 1, bytesRead, file2);
	}
	fclose(file1);
	fclose(file2);
}
