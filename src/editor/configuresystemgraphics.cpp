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

#include <QPixmap>
#include "configuresystemgraphics.h"
#include "imagebrowser.h"
#include "projectdata.h"

ConfigureSystemGraphics::ConfigureSystemGraphics(QWidget *parent) : QDialog(parent)
{
	ProjectData::SystemGraphics *systemGraphics = &ProjectData::systemGraphics;
	setupUi(this);
	frameBGLocation = systemGraphics->frameBGLocation;
	frameBorderLocation = systemGraphics->frameBorderLocation;
	selectionBorderLocation = systemGraphics->selectionBorderLocation;
	equipmentIndicatorsLocation = systemGraphics->equipmentIndicatorsLocation;
	menuBGLocation = systemGraphics->menuBGLocation;
	scrollArrowsLocation = systemGraphics->scrollArrowsLocation;
	grappleArmLocation = systemGraphics->grappleArmLocation;
	grappleHookLocation = systemGraphics->grappleHookLocation;
	timerNumbersLocation = systemGraphics->timerNumbersLocation;
	if (systemGraphics->stretchToFit)
		rbStretched->setChecked(true);
	else
		rbTiled->setChecked(true);
	lblFrameBG->setPixmap(QPixmap(frameBGLocation));
	lblFrameBorder->setPixmap(QPixmap(frameBorderLocation));
	lblSelectionBorder->setPixmap(QPixmap(selectionBorderLocation));
	lblEquipmentIndicators->setPixmap(QPixmap(equipmentIndicatorsLocation));
	lblMenuBG->setPixmap(QPixmap(menuBGLocation));
	lblScrollArrows->setPixmap(QPixmap(scrollArrowsLocation));
	lblGrappleArm->setPixmap(QPixmap(grappleArmLocation).copy(0, 96, 32, 32));
	lblGrappleHook->setPixmap(QPixmap(grappleHookLocation).copy(0, 96, 32, 32));
	lblTimerNumbers->setPixmap(QPixmap(timerNumbersLocation));
	bFontShadowColor->setColor(systemGraphics->fontShadowColor);
	bDefaultEnabledTopColor->setColor(systemGraphics->defaultFontEnabledTopColor);
	bDefaultEnabledBottomColor->setColor(systemGraphics->defaultFontEnabledBottomColor);
	bDefaultDisabledTopColor->setColor(systemGraphics->defaultFontDisabledTopColor);
	bDefaultDisabledBottomColor->setColor(systemGraphics->defaultFontDisabledBottomColor);
	resize(sizeHint());
	setMinimumWidth(width());
	setMaximumWidth(width());
	setMinimumHeight(height());
	setMaximumHeight(height());
}

ConfigureSystemGraphics::~ConfigureSystemGraphics()
{
}

void ConfigureSystemGraphics::on_bSetFrameBG_clicked()
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
	imageBrowser->setupBrowser("/system", blockedTypes, QList<QSize>() << QSize(64, 64));
	if (imageBrowser->exec())
	{
		frameBGLocation = imageBrowser->getFileLocation();
		lblFrameBG->setPixmap(QPixmap(frameBGLocation));
	}
	delete imageBrowser;
}

void ConfigureSystemGraphics::on_bSetFrameBorder_clicked()
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
	imageBrowser->setupBrowser("/system", blockedTypes, QList<QSize>() << QSize(64, 64));
	if (imageBrowser->exec())
	{
		frameBorderLocation = imageBrowser->getFileLocation();
		lblFrameBorder->setPixmap(QPixmap(frameBorderLocation));
	}
	delete imageBrowser;
}

void ConfigureSystemGraphics::on_bSetSelectionBorder_clicked()
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
	imageBrowser->setupBrowser("/system", blockedTypes, QList<QSize>() << QSize(128, 64));
	if (imageBrowser->exec())
	{
		selectionBorderLocation = imageBrowser->getFileLocation();
		lblSelectionBorder->setPixmap(QPixmap(selectionBorderLocation));
	}
	delete imageBrowser;
}

void ConfigureSystemGraphics::on_bSetEquipmentIndicators_clicked()
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
	imageBrowser->setupBrowser("/system", blockedTypes, QList<QSize>() << QSize(64, 64));
	if (imageBrowser->exec())
	{
		equipmentIndicatorsLocation = imageBrowser->getFileLocation();
		lblEquipmentIndicators->setPixmap(QPixmap(equipmentIndicatorsLocation));
	}
	delete imageBrowser;
}

void ConfigureSystemGraphics::on_bSetMenuBG_clicked()
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
	imageBrowser->setupBrowser("/system", blockedTypes, QList<QSize>() << QSize(32, 32));
	if (imageBrowser->exec())
	{
		menuBGLocation = imageBrowser->getFileLocation();
		lblMenuBG->setPixmap(QPixmap(menuBGLocation));
	}
	delete imageBrowser;
}

void ConfigureSystemGraphics::on_bSetScrollArrows_clicked()
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
	imageBrowser->setupBrowser("/system", blockedTypes, QList<QSize>() << QSize(32, 32));
	if (imageBrowser->exec())
	{
		scrollArrowsLocation = imageBrowser->getFileLocation();
		lblScrollArrows->setPixmap(QPixmap(scrollArrowsLocation));
	}
	delete imageBrowser;
}

void ConfigureSystemGraphics::on_bSetGrappleArm_clicked()
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
	imageBrowser->setupBrowser("/system", blockedTypes);
	if (imageBrowser->exec())
	{
		grappleArmLocation = imageBrowser->getFileLocation();
		lblGrappleArm->setPixmap(QPixmap(grappleArmLocation).copy(0, 96, 32, 32));
	}
	delete imageBrowser;
}

void ConfigureSystemGraphics::on_bSetGrappleHook_clicked()
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
	imageBrowser->setupBrowser("/system", blockedTypes);
	if (imageBrowser->exec())
	{
		grappleHookLocation = imageBrowser->getFileLocation();
		lblGrappleHook->setPixmap(QPixmap(grappleHookLocation).copy(0, 96, 32, 32));
	}
	delete imageBrowser;
}

void ConfigureSystemGraphics::on_bSetTimerNumbers_clicked()
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
	imageBrowser->setupBrowser("/system", blockedTypes, QList<QSize>() << QSize(192, 32));
	if (imageBrowser->exec())
	{
		timerNumbersLocation = imageBrowser->getFileLocation();
		lblTimerNumbers->setPixmap(QPixmap(timerNumbersLocation));
	}
	delete imageBrowser;
}

void ConfigureSystemGraphics::on_bDefaultEnabledTopColor_colorChanged(QColor color)
{
	QImage gradient = createGradientImage(color, bDefaultEnabledBottomColor->getColor(), lblDefaultEnabledColor->width() - 4, lblDefaultEnabledColor->height() - 4);
	lblDefaultEnabledColor->setPixmap(QPixmap::fromImage(gradient));
}

void ConfigureSystemGraphics::on_bDefaultEnabledBottomColor_colorChanged(QColor color)
{
	QImage gradient = createGradientImage(bDefaultEnabledTopColor->getColor(), color, lblDefaultEnabledColor->width() - 4, lblDefaultEnabledColor->height() - 4);
	lblDefaultEnabledColor->setPixmap(QPixmap::fromImage(gradient));
}

void ConfigureSystemGraphics::on_bDefaultDisabledTopColor_colorChanged(QColor color)
{
	QImage gradient = createGradientImage(color, bDefaultDisabledBottomColor->getColor(), lblDefaultDisabledColor->width() - 4, lblDefaultDisabledColor->height() - 4);
	lblDefaultDisabledColor->setPixmap(QPixmap::fromImage(gradient));
}

void ConfigureSystemGraphics::on_bDefaultDisabledBottomColor_colorChanged(QColor color)
{
	QImage gradient = createGradientImage(bDefaultDisabledTopColor->getColor(), color, lblDefaultDisabledColor->width() - 4, lblDefaultDisabledColor->height() - 4);
	lblDefaultDisabledColor->setPixmap(QPixmap::fromImage(gradient));
}

void ConfigureSystemGraphics::on_buttonBox_accepted()
{
	ProjectData::SystemGraphics *systemGraphics = &ProjectData::systemGraphics;
	systemGraphics->frameBGLocation = frameBGLocation;
	systemGraphics->frameBorderLocation = frameBorderLocation;
	systemGraphics->selectionBorderLocation = selectionBorderLocation;
	systemGraphics->equipmentIndicatorsLocation = equipmentIndicatorsLocation;
	systemGraphics->menuBGLocation = menuBGLocation;
	systemGraphics->scrollArrowsLocation = scrollArrowsLocation;
	systemGraphics->grappleArmLocation = grappleArmLocation;
	systemGraphics->grappleHookLocation = grappleHookLocation;
	systemGraphics->timerNumbersLocation = timerNumbersLocation;
	systemGraphics->stretchToFit = rbStretched->isChecked();
	systemGraphics->fontShadowColor = bFontShadowColor->getColor();
	systemGraphics->defaultFontEnabledTopColor = bDefaultEnabledTopColor->getColor();
	systemGraphics->defaultFontEnabledBottomColor = bDefaultEnabledBottomColor->getColor();
	systemGraphics->defaultFontDisabledTopColor = bDefaultDisabledTopColor->getColor();
	systemGraphics->defaultFontDisabledBottomColor = bDefaultDisabledBottomColor->getColor();
}

QImage ConfigureSystemGraphics::createGradientImage(QColor top, QColor bottom, int width, int height)
{
	QImage gradient = QImage(width, height, QImage::Format_RGB32);
	float tR = (float)top.red() / 255.0;
	float tG = (float)top.green() / 255.0;
	float tB = (float)top.blue() / 255.0;
	float bR = (float)bottom.red() / 255.0;
	float bG = (float)bottom.green() / 255.0;
	float bB = (float)bottom.blue() / 255.0;
	float rangeR = bR - tR;
	float rangeG = bG - tG;
	float rangeB = bB - tB;
	float fR, fG, fB;
	for (int y = 0; y < height; ++y)
	{
		float loc = (float)y / (float)(height - 1);
		fR = (loc * rangeR) + tR;
		fG = (loc * rangeG) + tG;
		fB = (loc * rangeB) + tB;
		for (int x = 0; x < width; ++x)
			gradient.setPixel(x, y, qRgb((int)(fR * 255), (int)(fG * 255), (int)(fB * 255)));
	}
	return gradient;
}
