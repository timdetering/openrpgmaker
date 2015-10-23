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
#include "projectdata_monsteranimation.h"
#include "importrm2k3monster.h"

ImportRM2K3Monster::~ImportRM2K3Monster()
{
}

void ImportRM2K3Monster::runImporter()
{
	QString fileLocation = QFileDialog::getOpenFileName(0, "Choose an RPG Maker 2000/2003 Monster", QString(), "PNG Files (*.png *.PNG)");
	if (!fileLocation.isEmpty())
		importRM2K3Monster(fileLocation);
}

QString ImportRM2K3Monster::getImporterName()
{
	return "RPG Maker 2000/2003 Monster";
}

QString ImportRM2K3Monster::getImporterDescription()
{
	return "Import an RPG Maker 2000/2003 formatted monster.";
}

void ImportRM2K3Monster::importRM2K3Monster(QString fileLocation)
{
	MonsterAnimation *animation = new MonsterAnimation;
	QImage oldImage = QImage(fileLocation);
	QImage newImage = QImage(oldImage.width(), oldImage.height(), QImage::Format_ARGB32);
	QString monsterLocation = ProjectData::projectLocation + "/images/monsters/" + QFileInfo(fileLocation).baseName() + ".png";
	for (int y = 0; y < oldImage.height(); ++y)
	{
		for (int x = 0; x < oldImage.width(); ++x)
		{
			if (oldImage.pixelIndex(x, y) == 0)
				newImage.setPixel(x, y, qRgba(0, 0, 0, 0));
			else
				newImage.setPixel(x, y, oldImage.pixel(x, y));
		}
	}
	if (!isImageEmpty(newImage))
		newImage.scaled(newImage.width() * 2, newImage.height() * 2).save(monsterLocation);
	animation->setName(QFileInfo(fileLocation).baseName());
	for (int i = 0; i < 32; ++i)
	{
		animation->setPoseLocation(i, monsterLocation);
		animation->setPoseNumFrames(i, 1);
	}
}

Q_EXPORT_PLUGIN2(importrm2k3monster, ImportRM2K3Monster);
