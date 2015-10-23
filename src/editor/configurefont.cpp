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
 *  Copyright (C) 2011, Justin Davis <tuxdavis@gmail.com>                   *
 ****************************************************************************/

#include "configurefont.h"
#include "imagebrowser.h"
#include "projectdata.h"
#include "projectdata_font.h"

ConfigureFont::ConfigureFont(int fontID, QWidget *parent) : QDialog(parent)
{
	QPixmap blankFontPixmap = QPixmap(24, 32);
	QPixmap blankPreviewPixmap = QPixmap(24, 32);
	setupUi(this);
	bCharacterWidthSideButton->setSpinBox(sbCharacterWidth);
	bCharacterHeightSideButton->setSpinBox(sbCharacterHeight);
	bSpaceWidthSideButton->setSpinBox(sbSpaceWidth);
	bTabWidthSideButton->setSpinBox(sbTabWidth);
	bDropXSideButton->setSpinBox(sbDropX);
	bDropYSideButton->setSpinBox(sbDropY);
	fontRef = new ProjectData::DataReference<Font>(fontID, __FILE__, __LINE__);
	imageLocation = (*fontRef)->getImageLocation();
	lblImage->setPixmap(QPixmap(imageLocation));
	leCharacterList->setText((*fontRef)->getCharacterList());
	leFontName->setText((*fontRef)->getName());
	sbCharacterWidth->setValue((*fontRef)->getCharacterWidth());
	sbCharacterHeight->setValue((*fontRef)->getCharacterHeight());
	sbSpaceWidth->setValue((*fontRef)->getSpaceWidth());
	sbTabWidth->setValue((*fontRef)->getTabWidth());
	sbDropX->setValue((*fontRef)->getDropShadowXOffset());
	sbDropY->setValue((*fontRef)->getDropShadowYOffset());
	lblPreviewText->setPixmap(QPixmap::fromImage((*fontRef)->getRenderedText(lePreviewText->text(), ProjectData::systemGraphics.defaultFontEnabledTopColor, ProjectData::systemGraphics.defaultFontEnabledBottomColor)));
	resize(width(), sizeHint().height());
	setMinimumWidth(width());
	setMaximumWidth(width());
	setMinimumHeight(height());
	setMaximumHeight(height());
}

ConfigureFont::~ConfigureFont()
{
	if (fontRef != NULL)
	{
		delete fontRef;
		fontRef = NULL;
	}
}

void ConfigureFont::on_bRenderPreviewText_clicked()
{
	QImage renderedImage;
	QString characterList = leCharacterList->text();
	int characterWidth = sbCharacterWidth->value();
	int characterHeight = sbCharacterHeight->value();
	int spaceWidth = sbSpaceWidth->value();
	int tabWidth = sbTabWidth->value();
	int dropX = sbDropX->value();
	int dropY = sbDropY->value();
	(*fontRef)->setData(imageLocation, characterList, characterWidth, characterHeight, spaceWidth, tabWidth, dropX, dropY);
	renderedImage = (*fontRef)->getRenderedText(lePreviewText->text(), ProjectData::systemGraphics.defaultFontEnabledTopColor, ProjectData::systemGraphics.defaultFontEnabledBottomColor);
	lblPreviewText->setPixmap(QPixmap::fromImage(renderedImage));
}

void ConfigureFont::on_bSetImage_clicked()
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
	imageBrowser->setupBrowser("/fonts", blockedTypes);
	if (imageBrowser->exec())
	{
		imageLocation = imageBrowser->getFileLocation();
		lblImage->setPixmap(QPixmap(imageLocation));
	}
	delete imageBrowser;
}

void ConfigureFont::on_buttonBox_accepted()
{
	QString characterList = leCharacterList->text();
	int characterWidth = sbCharacterWidth->value();
	int characterHeight = sbCharacterHeight->value();
	int spaceWidth = sbSpaceWidth->value();
	int tabWidth = sbTabWidth->value();
	int dropX = sbDropX->value();
	int dropY = sbDropY->value();
	(*fontRef)->setData(imageLocation, characterList, characterWidth, characterHeight, spaceWidth, tabWidth, dropX, dropY);
	(*fontRef)->setName(leFontName->text());
}
