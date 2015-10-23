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
#include "importrm2k3battleanimation.h"

ImportRM2K3BattleAnimation::~ImportRM2K3BattleAnimation()
{
}

void ImportRM2K3BattleAnimation::runImporter()
{
	QString fileLocation = QFileDialog::getOpenFileName(0, "Choose an RPG Maker 2000/2003 Battle Animation", QString(), "PNG Files (*.png *.PNG)");
	if (!fileLocation.isEmpty())
		importRM2K3BattleAnimation(fileLocation);
}

QString ImportRM2K3BattleAnimation::getImporterName()
{
	return "RPG Maker 2000/2003 Battle Animation";
}

QString ImportRM2K3BattleAnimation::getImporterDescription()
{
	return "Import an RPG Maker 2000/2003 formatted battle animation.";
}

void ImportRM2K3BattleAnimation::importRM2K3BattleAnimation(QString fileLocation)
{
	QImage battleAnimationSheet = QImage(fileLocation);
	QImage battleAnimation = QImage(96, 96, QImage::Format_ARGB32);
	QString battleAnimationLocation;
	int battleAnimationSheetWidth = battleAnimationSheet.width() / 96;
	int battleAnimationSheetHeight = battleAnimationSheet.height() / 96;
	int i = 0;
	QDir(ProjectData::projectLocation + "/images/animations").mkdir(QFileInfo(fileLocation).baseName());
	battleAnimationLocation = ProjectData::projectLocation + "/images/animations/" + QFileInfo(fileLocation).baseName();
	for (int y = 0; y < battleAnimationSheetHeight; ++y)
	{
		for (int x = 0; x < battleAnimationSheetWidth; ++x)
		{
			drawImage(battleAnimationSheet, QRect(x * 96, y * 96, 96, 96), battleAnimation, QPoint(0, 0), 0);
			if (!isImageEmpty(battleAnimation))
				battleAnimation.scaled(192, 192).save(QString("%1/frame%2.png").arg(battleAnimationLocation).arg(i));
			++i;
		}
	}
}

Q_EXPORT_PLUGIN2(importrm2k3battleanimation, ImportRM2K3BattleAnimation);
