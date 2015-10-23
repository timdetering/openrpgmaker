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

#include <QtPlugin>
#include <QString>
#include <QFileDialog>
#include <QImage>
#include <QFileInfo>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include "projectdata.h"
#include "importrm2k3characterspritesheet.h"

ImportRM2K3CharacterSpriteSheet::~ImportRM2K3CharacterSpriteSheet()
{
}

void ImportRM2K3CharacterSpriteSheet::runImporter()
{
	QString fileLocation = QFileDialog::getOpenFileName(0, "Choose an RPG Maker 2000/2003 Character Sprite Sheet", QString(), "PNG Files (*.png *.PNG)");
	if (!fileLocation.isEmpty())
		importRM2K3CharacterSpriteSheet(fileLocation);
}

QString ImportRM2K3CharacterSpriteSheet::getImporterName()
{
	return "RPG Maker 2000/2003 Character Sprite Sheet";
}

QString ImportRM2K3CharacterSpriteSheet::getImporterDescription()
{
	return "Import an RPG Maker 2000/2003 formatted character sprite sheet.";
}

void ImportRM2K3CharacterSpriteSheet::importRM2K3CharacterSpriteSheet(QString fileLocation)
{
	QString characterLocation;
	QImage spriteSheet = QImage(fileLocation);
	QImage characterSheet = QImage(72, 128, QImage::Format_ARGB32);
	if (spriteSheet.width() != 288 || spriteSheet.height() != 256)
		return;
	characterLocation = ProjectData::projectLocation + "/images/sprites/" + QFileInfo(fileLocation).baseName();
	drawImage(spriteSheet, QRect(0, 0, 72, 128), characterSheet, QPoint(0, 0), 0);
	if (!isImageEmpty(characterSheet))
	{
		characterSheet.copy(24, 0, 24, 128).scaled(48, 256).save(characterLocation + "_0_idle.png");
		characterSheet.scaled(144, 256).save(characterLocation + "_0_walking.png");
	}
	drawImage(spriteSheet, QRect(72, 0, 72, 128), characterSheet, QPoint(0, 0), 0);
	if (!isImageEmpty(characterSheet))
	{
		characterSheet.copy(24, 0, 24, 128).scaled(48, 256).save(characterLocation + "_1_idle.png");
		characterSheet.scaled(144, 256).save(characterLocation + "_1_walking.png");
	}
	drawImage(spriteSheet, QRect(144, 0, 72, 128), characterSheet, QPoint(0, 0), 0);
	if (!isImageEmpty(characterSheet))
	{
		characterSheet.copy(24, 0, 24, 128).scaled(48, 256).save(characterLocation + "_2_idle.png");
		characterSheet.scaled(144, 256).save(characterLocation + "_2_walking.png");
	}
	drawImage(spriteSheet, QRect(216, 0, 72, 128), characterSheet, QPoint(0, 0), 0);
	if (!isImageEmpty(characterSheet))
	{
		characterSheet.copy(24, 0, 24, 128).scaled(48, 256).save(characterLocation + "_3_idle.png");
		characterSheet.scaled(144, 256).save(characterLocation + "_3_walking.png");
	}
	drawImage(spriteSheet, QRect(0, 128, 72, 128), characterSheet, QPoint(0, 0), 0);
	if (!isImageEmpty(characterSheet))
	{
		characterSheet.copy(24, 0, 24, 128).scaled(48, 256).save(characterLocation + "_4_idle.png");
		characterSheet.scaled(144, 256).save(characterLocation + "_4_walking.png");
	}
	drawImage(spriteSheet, QRect(72, 128, 72, 128), characterSheet, QPoint(0, 0), 0);
	if (!isImageEmpty(characterSheet))
	{
		characterSheet.copy(24, 0, 24, 128).scaled(48, 256).save(characterLocation + "_5_idle.png");
		characterSheet.scaled(144, 256).save(characterLocation + "_5_walking.png");
	}
	drawImage(spriteSheet, QRect(144, 128, 72, 128), characterSheet, QPoint(0, 0), 0);
	if (!isImageEmpty(characterSheet))
	{
		characterSheet.copy(24, 0, 24, 128).scaled(48, 256).save(characterLocation + "_6_idle.png");
		characterSheet.scaled(144, 256).save(characterLocation + "_6_walking.png");
	}
	drawImage(spriteSheet, QRect(216, 128, 72, 128), characterSheet, QPoint(0, 0), 0);
	if (!isImageEmpty(characterSheet))
	{
		characterSheet.copy(24, 0, 24, 128).scaled(48, 256).save(characterLocation + "_7_idle.png");
		characterSheet.scaled(144, 256).save(characterLocation + "_7_walking.png");
	}
}

Q_EXPORT_PLUGIN2(importrm2k3characterspritesheet, ImportRM2K3CharacterSpriteSheet);
