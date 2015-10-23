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
#include "importrm2k3faceset.h"

ImportRM2K3FaceSet::~ImportRM2K3FaceSet()
{
}

void ImportRM2K3FaceSet::runImporter()
{
	QString fileLocation = QFileDialog::getOpenFileName(0, "Choose an RPG Maker 2000/2003 Battle Character", QString(), "PNG Files (*.png *.PNG)");
	if (!fileLocation.isEmpty())
		importRM2K3FaceSet(fileLocation);
}

QString ImportRM2K3FaceSet::getImporterName()
{
	return "RPG Maker 2000/2003 Face Set";
}

QString ImportRM2K3FaceSet::getImporterDescription()
{
	return "Import an RPG Maker 2000/2003 formatted face set.";
}

void ImportRM2K3FaceSet::importRM2K3FaceSet(QString fileLocation)
{
	QString faceLocation;
	QImage faceSet = QImage(fileLocation).convertToFormat(QImage::Format_ARGB32);
	QImage face;
	if (faceSet.width() != 192 || faceSet.height() != 192)
		return;
	QDir(ProjectData::projectLocation + "/images/faces").mkdir(QFileInfo(fileLocation).baseName());
	faceLocation = ProjectData::projectLocation + "/images/faces/" + QFileInfo(fileLocation).baseName();
	face = faceSet.copy(0, 0, 48, 48);
	if (!isImageEmpty(face))
		face.scaled(96, 96).save(faceLocation + "/face0.png");
	face = faceSet.copy(48, 0, 48, 48);
	if (!isImageEmpty(face))
		face.scaled(96, 96).save(faceLocation + "/face1.png");
	face = faceSet.copy(96, 0, 48, 48);
	if (!isImageEmpty(face))
		face.scaled(96, 96).save(faceLocation + "/face2.png");
	face = faceSet.copy(144, 0, 48, 48);
	if (!isImageEmpty(face))
		face.scaled(96, 96).save(faceLocation + "/face3.png");
	face = faceSet.copy(0, 48, 48, 48);
	if (!isImageEmpty(face))
		face.scaled(96, 96).save(faceLocation + "/face4.png");
	face = faceSet.copy(48, 48, 48, 48);
	if (!isImageEmpty(face))
		face.scaled(96, 96).save(faceLocation + "/face5.png");
	face = faceSet.copy(96, 48, 48, 48);
	if (!isImageEmpty(face))
		face.scaled(96, 96).save(faceLocation + "/face6.png");
	face = faceSet.copy(144, 48, 48, 48);
	if (!isImageEmpty(face))
		face.scaled(96, 96).save(faceLocation + "/face7.png");
	face = faceSet.copy(0, 96, 48, 48);
	if (!isImageEmpty(face))
		face.scaled(96, 96).save(faceLocation + "/face8.png");
	face = faceSet.copy(48, 96, 48, 48);
	if (!isImageEmpty(face))
		face.scaled(96, 96).save(faceLocation + "/face9.png");
	face = faceSet.copy(96, 96, 48, 48);
	if (!isImageEmpty(face))
		face.scaled(96, 96).save(faceLocation + "/face10.png");
	face = faceSet.copy(144, 96, 48, 48);
	if (!isImageEmpty(face))
		face.scaled(96, 96).save(faceLocation + "/face11.png");
	face = faceSet.copy(0, 144, 48, 48);
	if (!isImageEmpty(face))
		face.scaled(96, 96).save(faceLocation + "/face12.png");
	face = faceSet.copy(48, 144, 48, 48);
	if (!isImageEmpty(face))
		face.scaled(96, 96).save(faceLocation + "/face13.png");
	face = faceSet.copy(96, 144, 48, 48);
	if (!isImageEmpty(face))
		face.scaled(96, 96).save(faceLocation + "/face14.png");
	face = faceSet.copy(144, 144, 48, 48);
	if (!isImageEmpty(face))
		face.scaled(96, 96).save(faceLocation + "/face15.png");
}

Q_EXPORT_PLUGIN2(importrm2k3faceset, ImportRM2K3FaceSet);
