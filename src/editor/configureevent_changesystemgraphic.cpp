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

#include <QFileInfo>
#include "configureevent_changesystemgraphic.h"
#include "imagebrowser.h"
#include "projectdata.h"

ConfigureEvent_ChangeSystemGraphic::ConfigureEvent_ChangeSystemGraphic(QWidget *parent) : QDialog(parent)
{
	setupUi(this);
	resize(sizeHint().width(), sizeHint().height());
	setMinimumWidth(width());
	setMaximumWidth(width());
	setMinimumHeight(height());
	setMaximumHeight(height());
	graphicLocation = ProjectData::systemGraphics.frameBGLocation;
	leGraphic->setText(QFileInfo(graphicLocation).fileName());
}

ConfigureEvent_ChangeSystemGraphic::~ConfigureEvent_ChangeSystemGraphic()
{
}

void ConfigureEvent_ChangeSystemGraphic::setGraphicToChange(int graphic)
{
	cboxSystemGraphic->setCurrentIndex(graphic);
}

void ConfigureEvent_ChangeSystemGraphic::setGraphicLocation(QString location)
{
	if (!location.isEmpty())
	{
		graphicLocation = location;
		leGraphic->setText(QFileInfo(graphicLocation).fileName());
	}
}

int ConfigureEvent_ChangeSystemGraphic::getGraphicToChange()
{
	return cboxSystemGraphic->currentIndex();
}

QString ConfigureEvent_ChangeSystemGraphic::getGraphicLocation()
{
	return graphicLocation;
}

void ConfigureEvent_ChangeSystemGraphic::on_bBrowseGraphics_clicked()
{
	ImageBrowser *imageBrowser = new ImageBrowser();
	int graphic = cboxSystemGraphic->currentIndex();
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
	if (graphic == FRAME_BG)
		imageBrowser->setupBrowser("/system", blockedTypes, QList<QSize>() << QSize(64, 64));
	else if (graphic == FRAME_BORDER)
		imageBrowser->setupBrowser("/system", blockedTypes, QList<QSize>() << QSize(64, 64));
	else if (graphic == SELECTION_BORDER)
		imageBrowser->setupBrowser("/system", blockedTypes, QList<QSize>() << QSize(128, 64));
	else if (graphic == EQUIPMENT_INDICATORS)
		imageBrowser->setupBrowser("/system", blockedTypes, QList<QSize>() << QSize(64, 64));
	else if (graphic == MENU_BG)
		imageBrowser->setupBrowser("/system", blockedTypes, QList<QSize>() << QSize(32, 32));
	else if (graphic == SCROLL_ARROWS)
		imageBrowser->setupBrowser("/system", blockedTypes, QList<QSize>() << QSize(32, 32));
	else if (graphic == GRAPPLE_ARM || graphic == GRAPPLE_HOOK)
		imageBrowser->setupBrowser("/system", blockedTypes);
	else if (graphic == TIMER_NUMBERS)
		imageBrowser->setupBrowser("/system", blockedTypes, QList<QSize>() << QSize(192, 32));
	if (imageBrowser->exec())
	{
		graphicLocation = imageBrowser->getFileLocation();
		leGraphic->setText(QFileInfo(graphicLocation).fileName());
	}
	delete imageBrowser;
}

void ConfigureEvent_ChangeSystemGraphic::on_cboxSystemGraphic_currentIndexChanged(int index)
{
	if (index == FRAME_BG)
		graphicLocation = ProjectData::systemGraphics.frameBGLocation;
	else if (index == FRAME_BORDER)
		graphicLocation = ProjectData::systemGraphics.frameBorderLocation;
	else if (index == SELECTION_BORDER)
		graphicLocation = ProjectData::systemGraphics.selectionBorderLocation;
	else if (index == EQUIPMENT_INDICATORS)
		graphicLocation = ProjectData::systemGraphics.equipmentIndicatorsLocation;
	else if (index == MENU_BG)
		graphicLocation = ProjectData::systemGraphics.menuBGLocation;
	else if (index == SCROLL_ARROWS)
		graphicLocation = ProjectData::systemGraphics.scrollArrowsLocation;
	else if (index == GRAPPLE_ARM)
		graphicLocation = ProjectData::systemGraphics.grappleArmLocation;
	else if (index == GRAPPLE_HOOK)
		graphicLocation = ProjectData::systemGraphics.grappleHookLocation;
	else if (index == TIMER_NUMBERS)
		graphicLocation = ProjectData::systemGraphics.timerNumbersLocation;
	leGraphic->setText(QFileInfo(graphicLocation).fileName());
}
