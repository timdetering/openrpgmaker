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
#include "importrm2k3background.h"

ImportRM2K3Background::~ImportRM2K3Background()
{
}

void ImportRM2K3Background::runImporter()
{
	QString fileLocation = QFileDialog::getOpenFileName(0, "Choose an RPG Maker 2000/2003 Background", QString(), "PNG Files (*.png *.PNG)");
	if (!fileLocation.isEmpty())
		importRM2K3Background(fileLocation);
}

QString ImportRM2K3Background::getImporterName()
{
	return "RPG Maker 2000/2003 Background";
}

QString ImportRM2K3Background::getImporterDescription()
{
	return "Import an RPG Maker 2000/2003 formatted background.";
}

void ImportRM2K3Background::importRM2K3Background(QString fileLocation)
{
	QImage background = QImage(fileLocation);
	QString backgroundLocation = ProjectData::projectLocation + "/images/backgrounds/" + QFileInfo(fileLocation).baseName() + ".png";
	background.scaled(background.width() * 2, background.height() * 2).save(backgroundLocation);
}

Q_EXPORT_PLUGIN2(importrm2k3background, ImportRM2K3Background);
