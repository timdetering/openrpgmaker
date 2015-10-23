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
 *  Copyright (C) 2012, Justin Davis <tuxdavis@gmail.com>                   *
 ****************************************************************************/

#include "configuremapeventpagespritegraphic.h"
#include "imagebrowser.h"
#include "projectdata.h"

ConfigureMapEventPageSpriteGraphic::ConfigureMapEventPageSpriteGraphic(QString spriteLocation, QWidget *parent) : QDialog(parent)
{
	setupUi(this);
	resize(sizeHint());
	setMinimumWidth(width());
	setMaximumWidth(width());
	setMinimumHeight(height());
	setMaximumHeight(height());
	if (spriteLocation.startsWith("sprite:"))
	{
		rbFile->setChecked(true);
		leFile->setText(ProjectData::getRelativeResourcePath(spriteLocation.mid(7)));
	}
	else if (spriteLocation.startsWith("tile:"))
	{
		rbTile->setChecked(true);
		bTile->setTile(spriteLocation.mid(5).toUShort(NULL, 16));
	}
}

ConfigureMapEventPageSpriteGraphic::~ConfigureMapEventPageSpriteGraphic()
{
}

QString ConfigureMapEventPageSpriteGraphic::getSpriteLocation()
{
	if (rbFile->isChecked())
		return QString("sprite:%1").arg(ProjectData::getAbsoluteResourcePath(leFile->text()));
	else
		return QString("tile:%1").arg(bTile->getTile(), 4, 16, QChar('0'));
}

void ConfigureMapEventPageSpriteGraphic::on_bBrowseFiles_clicked()
{
	ImageBrowser *imageBrowser = new ImageBrowser();
	int blockedTypes = 0;
	blockedTypes += ImageBrowser::BlockBMP;
	blockedTypes += ImageBrowser::BlockGIF;
	blockedTypes += ImageBrowser::BlockJPEG;
	blockedTypes += ImageBrowser::BlockPBM;
	blockedTypes += ImageBrowser::BlockPCX;
	blockedTypes += ImageBrowser::BlockPGM;
	blockedTypes += ImageBrowser::BlockPNM;
	blockedTypes += ImageBrowser::BlockPPM;
	blockedTypes += ImageBrowser::BlockTGA;
	blockedTypes += ImageBrowser::BlockTIFF;
	blockedTypes += ImageBrowser::BlockXPM;
	imageBrowser->setupBrowser("/sprites", blockedTypes, QList<QSize>() << QSize(144, 256));
	if (imageBrowser->exec())
		leFile->setText(ProjectData::getRelativeResourcePath(imageBrowser->getFileLocation()));
	delete imageBrowser;
}

void ConfigureMapEventPageSpriteGraphic::on_rbFile_toggled(bool checked)
{
	if (checked)
		stackedWidget->setCurrentWidget(wFile);
}

void ConfigureMapEventPageSpriteGraphic::on_rbTile_toggled(bool checked)
{
	if (checked)
		stackedWidget->setCurrentWidget(wTile);
}
