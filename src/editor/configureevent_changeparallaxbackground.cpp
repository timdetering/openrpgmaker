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

#include "configureevent_changeparallaxbackground.h"
#include "imagebrowser.h"

ConfigureEvent_ChangeParallaxBackground::ConfigureEvent_ChangeParallaxBackground(QWidget *parent) : QDialog(parent)
{
	setupUi(this);
	resize(sizeHint().width(), sizeHint().height());
	setMinimumWidth(width());
	setMaximumWidth(width());
	setMinimumHeight(height());
	setMaximumHeight(height());
	bHScrollSpeedSideButton->setSpinBox(sbHScrollSpeed);
	bVScrollSpeedSideButton->setSpinBox(sbVScrollSpeed);
}

ConfigureEvent_ChangeParallaxBackground::~ConfigureEvent_ChangeParallaxBackground()
{
}

QString ConfigureEvent_ChangeParallaxBackground::getBackgroundLocation()
{
	return bgLocation;
}

void ConfigureEvent_ChangeParallaxBackground::setBackgroundLocation(QString value)
{
	bgLocation = value;
	lblBGPreview->setPixmap(QPixmap(bgLocation));
}

int ConfigureEvent_ChangeParallaxBackground::getHScrollSpeed()
{
	return sbHScrollSpeed->value();
}

void ConfigureEvent_ChangeParallaxBackground::setHScrollSpeed(int value)
{
	sbHScrollSpeed->setValue(value);
}

bool ConfigureEvent_ChangeParallaxBackground::getAutoHScroll()
{
	return cbAutoHScroll->isChecked();
}

void ConfigureEvent_ChangeParallaxBackground::setAutoHScroll(bool value)
{
	cbAutoHScroll->setChecked(value);
}

int ConfigureEvent_ChangeParallaxBackground::getVScrollSpeed()
{
	return sbVScrollSpeed->value();
}

void ConfigureEvent_ChangeParallaxBackground::setVScrollSpeed(int value)
{
	sbVScrollSpeed->setValue(value);
}

bool ConfigureEvent_ChangeParallaxBackground::getAutoVScroll()
{
	return cbAutoVScroll->isChecked();
}

void ConfigureEvent_ChangeParallaxBackground::setAutoVScroll(bool value)
{
	cbAutoVScroll->setChecked(value);
}

void ConfigureEvent_ChangeParallaxBackground::on_bSetBG_clicked()
{
	ImageBrowser *imageBrowser = new ImageBrowser;
	imageBrowser->setupBrowser("/backgrounds", ImageBrowser::BlockNone, QList<QSize>() << QSize(640, 480));
	if (imageBrowser->exec())
	{
		bgLocation = imageBrowser->getFileLocation();
		lblBGPreview->setPixmap(QPixmap(bgLocation));
	}
	delete imageBrowser;
}
