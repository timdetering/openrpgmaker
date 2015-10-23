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
#include <QDir>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include "projectdata.h"
#include "importrm2k3battlecharacter.h"

ImportRM2K3BattleCharacter::~ImportRM2K3BattleCharacter()
{
}

void ImportRM2K3BattleCharacter::runImporter()
{
	QString fileLocation = QFileDialog::getOpenFileName(0, "Choose an RPG Maker 2000/2003 Battle Character", QString(), "PNG Files (*.png *.PNG)");
	if (!fileLocation.isEmpty())
		importRM2K3BattleCharacter(fileLocation);
}

QString ImportRM2K3BattleCharacter::getImporterName()
{
	return "RPG Maker 2000/2003 Battle Character";
}

QString ImportRM2K3BattleCharacter::getImporterDescription()
{
	return "Import an RPG Maker 2000/2003 formatted battle character.";
}

void ImportRM2K3BattleCharacter::importRM2K3BattleCharacter(QString fileLocation)
{
	QString characterLocation;
	QImage spriteSheet = QImage(fileLocation);
	QImage battleCharacter = QImage(144, 48, QImage::Format_ARGB32);
	if (spriteSheet.width() != 144 || spriteSheet.height() != 384)
		return;
	QDir(ProjectData::projectLocation + "/images/battlecharacters").mkdir(QFileInfo(fileLocation).baseName());
	characterLocation = ProjectData::projectLocation + "/images/battlecharacters/" + QFileInfo(fileLocation).baseName();
	drawImage(spriteSheet, QRect(0, 0, 144, 48), battleCharacter, QPoint(0, 0), 0);
	if (!isImageEmpty(battleCharacter))
		battleCharacter.scaled(288, 96).save(characterLocation + "/pose0.png");
	drawImage(spriteSheet, QRect(0, 48, 144, 48), battleCharacter, QPoint(0, 0), 0);
	if (!isImageEmpty(battleCharacter))
		battleCharacter.scaled(288, 96).save(characterLocation + "/pose1.png");
	drawImage(spriteSheet, QRect(0, 96, 144, 48), battleCharacter, QPoint(0, 0), 0);
	if (!isImageEmpty(battleCharacter))
		battleCharacter.scaled(288, 96).save(characterLocation + "/pose2.png");
	drawImage(spriteSheet, QRect(0, 144, 144, 48), battleCharacter, QPoint(0, 0), 0);
	if (!isImageEmpty(battleCharacter))
		battleCharacter.scaled(288, 96).save(characterLocation + "/pose3.png");
	drawImage(spriteSheet, QRect(0, 192, 144, 48), battleCharacter, QPoint(0, 0), 0);
	if (!isImageEmpty(battleCharacter))
		battleCharacter.scaled(288, 96).save(characterLocation + "/pose4.png");
	drawImage(spriteSheet, QRect(0, 240, 144, 48), battleCharacter, QPoint(0, 0), 0);
	if (!isImageEmpty(battleCharacter))
		battleCharacter.scaled(288, 96).save(characterLocation + "/pose5.png");
	drawImage(spriteSheet, QRect(0, 288, 144, 48), battleCharacter, QPoint(0, 0), 0);
	if (!isImageEmpty(battleCharacter))
		battleCharacter.scaled(288, 96).save(characterLocation + "/pose6.png");
	drawImage(spriteSheet, QRect(0, 336, 144, 48), battleCharacter, QPoint(0, 0), 0);
	if (!isImageEmpty(battleCharacter))
		battleCharacter.scaled(288, 96).save(characterLocation + "/pose7.png");
}

Q_EXPORT_PLUGIN2(importrm2k3battlecharacter, ImportRM2K3BattleCharacter);
