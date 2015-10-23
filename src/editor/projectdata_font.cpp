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

#include <QRegExp>
#include <QPainter>
#include "projectdata.h"
#include "projectdata_font.h"
#include "xmlParser.h"
#include "xmlTools.h"

Font::Font()
{
	name = "Untitled Font";
	characterList = "";
	imageLocation = "";
	characterWidth = 24;
	characterHeight = 32;
	spaceWidth = 12;
	tabWidth = 36;
	dropShadowXOffset = 2;
	dropShadowYOffset = 2;
}

Font::Font(XMLNode FontNode)
{
	QString characters;
	name = FontNode.getAttribute("name");
	imageLocation = ProjectData::getAbsoluteResourcePath(FontNode.getAttribute("imageLocation"));
	characterWidth = QString(FontNode.getAttribute("characterWidth")).toInt();
	characterHeight = QString(FontNode.getAttribute("characterHeight")).toInt();
	spaceWidth = QString(FontNode.getAttribute("spaceWidth")).toInt();
	tabWidth = QString(FontNode.getAttribute("tabWidth")).toInt();
	dropShadowXOffset = QString(FontNode.getAttribute("dropShadowXOffset")).toInt();
	dropShadowYOffset = QString(FontNode.getAttribute("dropShadowYOffset")).toInt();
	characterList = QString(FontNode.getText()).replace(QRegExp("\\s"), "");
	loadCharacterImages();
}

Font::Font(StorageFile &storageFile)
{
	unsigned short numCharacters;
	char *temp_string;
	temp_string = storageFile.getString();
	name = temp_string;
	delete[] temp_string;
	numCharacters = storageFile.getUnsignedShort();
	characterList = "";
	for (unsigned short i = 0; i < numCharacters; ++i)
		characterList.append(QChar(storageFile.getUnsignedShort()));
	temp_string = storageFile.getString();
	imageLocation = temp_string;
	delete[] temp_string;
	characterWidth = storageFile.getSignedInt();
	characterHeight = storageFile.getSignedInt();
	spaceWidth = storageFile.getSignedInt();
	tabWidth = storageFile.getSignedInt();
	dropShadowXOffset = storageFile.getSignedInt();
	dropShadowYOffset = storageFile.getSignedInt();
	loadCharacterImages();
}

Font::~Font()
{
}

XMLNode Font::getFontNode()
{
	XMLNode FontNode = XMLNode::createXMLTopNode("font");
	QString characters = "";
	FontNode.addAttribute("name", name.toUtf8().data());
	FontNode.addAttribute("imageLocation", ProjectData::getRelativeResourcePath(imageLocation).toUtf8().data());
	FontNode.addAttribute("characterWidth", QString::number(characterWidth).toUtf8().data());
	FontNode.addAttribute("characterHeight", QString::number(characterHeight).toUtf8().data());
	FontNode.addAttribute("spaceWidth", QString::number(spaceWidth).toUtf8().data());
	FontNode.addAttribute("tabWidth", QString::number(tabWidth).toUtf8().data());
	FontNode.addAttribute("dropShadowXOffset", QString::number(dropShadowXOffset).toUtf8().data());
	FontNode.addAttribute("dropShadowYOffset", QString::number(dropShadowYOffset).toUtf8().data());
	FontNode.addText(characterList.toUtf8().data());
	return FontNode;
}

void Font::saveToStorageFile(StorageFile &storageFile)
{
	storageFile.putString(name.toUtf8().data());
	storageFile.putUnsignedShort((unsigned short)characterList.size());
	for (int i = 0; i < characterList.size(); ++i)
		storageFile.putUnsignedShort(characterList[i].unicode());
	storageFile.putString(imageLocation.toUtf8().data());
	storageFile.putSignedInt(characterWidth);
	storageFile.putSignedInt(characterHeight);
	storageFile.putSignedInt(spaceWidth);
	storageFile.putSignedInt(tabWidth);
	storageFile.putSignedInt(dropShadowXOffset);
	storageFile.putSignedInt(dropShadowYOffset);
}

QStringList Font::getResources()
{
	return QStringList() << imageLocation;
}

void Font::setData(QString image, QString characters, int charWidth, int charHeight, int space, int tab, int dropX, int dropY)
{
	imageLocation = image;
	characterList = characters;
	characterWidth = charWidth;
	characterHeight = charHeight;
	spaceWidth = space;
	tabWidth = tab;
	dropShadowXOffset = dropX;
	dropShadowYOffset = dropY;
	loadCharacterImages();
}

QString Font::getName()
{
	return name;
}

void Font::setName(QString value)
{
	name = value;
}

QImage Font::getCharacterImage(unsigned short character)
{
	return characterImages[character];
}

int Font::getCharacterWidth(unsigned short character)
{
	if (characterWidths.contains(character))
		return characterWidths[character];
	return characterWidth;
}

QString Font::getCharacterList()
{
	return characterList;
}

QString Font::getImageLocation()
{
	return imageLocation;
}

QImage Font::getRenderedText(QString text, QColor topColor, QColor bottomColor, bool drawDropShadow)
{
	QImage finalImage;
	QPainter painter;
	QList<QRgb> colors;
	float tR = (float)topColor.red() / 255.0;
	float tG = (float)topColor.green() / 255.0;
	float tB = (float)topColor.blue() / 255.0;
	float bR = (float)bottomColor.red() / 255.0;
	float bG = (float)bottomColor.green() / 255.0;
	float bB = (float)bottomColor.blue() / 255.0;
	float rangeR = bR - tR;
	float rangeG = bG - tG;
	float rangeB = bB - tB;
	float fR, fG, fB;
	int xOffset = 0;
	int finalWidth = 0;
	for (int y = 0; y < characterHeight; ++y)
	{
		float loc = (float)y / (float)(characterHeight - 1);
		fR = (loc * rangeR) + tR;
		fG = (loc * rangeG) + tG;
		fB = (loc * rangeB) + tB;
		colors.append(qRgb((int)(fR * 255), (int)(fG * 255), (int)(fB * 255)));
	}
	if (characterImages.size() == 0)
	{
		finalImage = QImage(characterWidth, characterHeight, QImage::Format_ARGB32);
		finalImage.fill(qRgba(0, 0, 0, 0));
	}
	else
	{
		for (int i = 0; i < text.size(); ++i)
		{
			if (text.at(i).unicode() == QChar(' ').unicode())
				finalWidth += spaceWidth;
			else if (text.at(i).unicode() == QChar('\t').unicode())
				finalWidth += tabWidth;
			else if (!characterList.contains(text.at(i)))
				finalWidth += characterWidth + abs(dropShadowXOffset) + 2;
			else
				finalWidth += characterWidths[text.at(i).unicode()] + abs(dropShadowXOffset) + 2;
		}
		finalWidth -= 2;
		finalImage = QImage(finalWidth, characterHeight + abs(dropShadowYOffset), QImage::Format_ARGB32);
		finalImage.fill(qRgba(0, 0, 0, 0));
		painter.begin(&finalImage);
		for (int i = 0; i < text.size(); ++i)
		{
			QImage characterImage = getRenderedCharacter(text.at(i).unicode(), colors, drawDropShadow);
			painter.drawImage(xOffset, 0, characterImage);
			xOffset += characterImage.width() + 2;
		}
		painter.end();
	}
	return finalImage;
}

int Font::getTextWidth(QString text)
{
	int finalWidth = 0;
	for (int i = 0; i < text.size(); ++i)
	{
		if (text.at(i).unicode() == QChar(' ').unicode())
			finalWidth += spaceWidth;
		else if (text.at(i).unicode() == QChar('\t').unicode())
			finalWidth += tabWidth;
		else if (!characterList.contains(text.at(i)))
			finalWidth += characterWidth + abs(dropShadowXOffset) + 2;
		else
			finalWidth += characterWidths[text.at(i).unicode()] + abs(dropShadowXOffset) + 2;
	}
	finalWidth -= 2;
	return finalWidth;
}

int Font::getCharacterWidth()
{
	return characterWidth;
}

int Font::getCharacterHeight()
{
	return characterHeight;
}

int Font::getSpaceWidth()
{
	return spaceWidth;
}

int Font::getTabWidth()
{
	return tabWidth;
}

int Font::getDropShadowXOffset()
{
	return dropShadowXOffset;
}

int Font::getDropShadowYOffset()
{
	return dropShadowYOffset;
}

bool Font::tryLoad(XMLNode FontNode)
{
	if (!XMLTools::attributeExists(FontNode, "name"))
		return false;
	if (!XMLTools::attributeExists(FontNode, "imageLocation"))
		return false;
	if (!XMLTools::attributeExists(FontNode, "characterWidth"))
		return false;
	if (!XMLTools::attributeIntRangeValid(FontNode, "characterWidth", 8, 64))
		return false;
	if (!XMLTools::attributeExists(FontNode, "characterHeight"))
		return false;
	if (!XMLTools::attributeIntRangeValid(FontNode, "characterHeight", 8, 64))
		return false;
	if (!XMLTools::attributeExists(FontNode, "spaceWidth"))
		return false;
	if (!XMLTools::attributeIntRangeValid(FontNode, "spaceWidth", 0, 64))
		return false;
	if (!XMLTools::attributeExists(FontNode, "tabWidth"))
		return false;
	if (!XMLTools::attributeIntRangeValid(FontNode, "tabWidth", 0, 64))
		return false;
	if (!XMLTools::attributeExists(FontNode, "dropShadowXOffset"))
		return false;
	if (!XMLTools::attributeIntRangeValid(FontNode, "dropShadowXOffset", -4, 4))
		return false;
	if (!XMLTools::attributeExists(FontNode, "dropShadowYOffset"))
		return false;
	if (!XMLTools::attributeIntRangeValid(FontNode, "dropShadowYOffset", -4, 4))
		return false;
	return true;
}

void Font::loadCharacterImages()
{
	QImage image = QImage(imageLocation);
	for (int i = 0; i <= characterList.size(); ++i)
	{
		QImage characterImage;
		int imageWidth = image.width() / characterWidth;
		int offsetX = (i % imageWidth) * characterWidth;
		int offsetY = (i / imageWidth) * characterHeight;
		int left = 0;
		int right = characterWidth - 1;
		int width;
		int alpha;
		unsigned short character;
		bool found = false;
		if (i < characterList.size())
			character = characterList.at(i).unicode();
		for (int x = 0; x < characterWidth && !found; ++x)
		{
			for (int y = 0; y < characterHeight && !found; ++y)
			{
				alpha = qAlpha(image.pixel(x + offsetX, y + offsetY));
				if (alpha != 0)
				{
					left = x;
					found = true;
				}
			}
		}
		found = false;
		for (int x = characterWidth - 1; x >= 0 && !found; --x)
		{
			for (int y = 0; y < characterHeight && !found; ++y)
			{
				alpha = qAlpha(image.pixel(x + offsetX, y + offsetY));
				if (alpha != 0)
				{
					right = x;
					found = true;
				}
			}
		}
		width = right - left + 1;
		characterImage = image.copy(left + offsetX, offsetY, width, characterHeight);
		if (i < characterList.size())
		{
			characterImages.insert(character, characterImage);
			characterWidths.insert(character, width);
		}
		else
			unknownImage = characterImage;
	}
}

QImage Font::getRenderedCharacter(unsigned short character, QList<QRgb> colors, bool drawDropShadow)
{
	QImage finalImage;
	int shadowColorR = ProjectData::systemGraphics.fontShadowColor.red();
	int shadowColorG = ProjectData::systemGraphics.fontShadowColor.green();
	int shadowColorB = ProjectData::systemGraphics.fontShadowColor.blue();
	int gradientColorR, gradientColorG, gradientColorB;
	if (character == QChar(' ').unicode())
	{
		finalImage = QImage(spaceWidth - 2, characterHeight, QImage::Format_ARGB32);
		finalImage.fill(qRgba(0, 0, 0, 0));
	}
	else if (character == QChar('\t').unicode())
	{
		finalImage = QImage(tabWidth - 2, characterHeight, QImage::Format_ARGB32);
		finalImage.fill(qRgba(0, 0, 0, 0));
	}
	else if (!characterList.contains(QChar(character)))
	{
		QImage characterImage = unknownImage;
		QImage shadowImage, finalCharacterImage;
		QPainter painter;
		int width = characterWidth;
		int height = characterHeight;
		int shadowXOffset = 0, shadowYOffset = 0;
		int characterXOffset = 0, characterYOffset = 0;
		if (dropShadowXOffset < 0)
			characterXOffset = -dropShadowXOffset;
		else
			shadowXOffset = dropShadowXOffset;
		if (dropShadowYOffset < 0)
			characterYOffset = -dropShadowYOffset;
		else
			shadowYOffset = dropShadowYOffset;
		shadowImage = QImage(width, height, QImage::Format_ARGB32);
		finalCharacterImage = QImage(width, height, QImage::Format_ARGB32);
		finalImage = QImage(width + abs(dropShadowXOffset), height + abs(dropShadowYOffset), QImage::Format_ARGB32);
		finalImage.fill(qRgba(0, 0, 0, 0));
		if (drawDropShadow)
		{
			for (int y = 0; y < height; ++y)
			{
				for (int x = 0; x < width; ++x)
					shadowImage.setPixel(x, y, qRgba(shadowColorR, shadowColorG, shadowColorB, qAlpha(characterImage.pixel(x, y))));
			}
		}
		for (int y = 0; y < height; ++y)
		{
			gradientColorR = qRed(colors[y]);
			gradientColorG = qGreen(colors[y]);
			gradientColorB = qBlue(colors[y]);
			for (int x = 0; x < width; ++x)
				finalCharacterImage.setPixel(x, y, qRgba(gradientColorR, gradientColorG, gradientColorB, qAlpha(characterImage.pixel(x, y))));
		}
		painter.begin(&finalImage);
		painter.drawImage(shadowXOffset, shadowYOffset, shadowImage);
		painter.drawImage(characterXOffset, characterYOffset, finalCharacterImage);
		painter.end();
	}
	else
	{
		QImage characterImage = characterImages[character];
		QImage shadowImage, finalCharacterImage;
		QPainter painter;
		int width = characterWidths[character];
		int height = characterHeight;
		int shadowXOffset = 0, shadowYOffset = 0;
		int characterXOffset = 0, characterYOffset = 0;
		if (dropShadowXOffset < 0)
			characterXOffset = -dropShadowXOffset;
		else
			shadowXOffset = dropShadowXOffset;
		if (dropShadowYOffset < 0)
			characterYOffset = -dropShadowYOffset;
		else
			shadowYOffset = dropShadowYOffset;
		shadowImage = QImage(width, height, QImage::Format_ARGB32);
		finalCharacterImage = QImage(width, height, QImage::Format_ARGB32);
		finalImage = QImage(width + abs(dropShadowXOffset), height + abs(dropShadowYOffset), QImage::Format_ARGB32);
		finalImage.fill(qRgba(0, 0, 0, 0));
		if (drawDropShadow)
		{
			for (int y = 0; y < height; ++y)
			{
				for (int x = 0; x < width; ++x)
					shadowImage.setPixel(x, y, qRgba(shadowColorR, shadowColorG, shadowColorB, qAlpha(characterImage.pixel(x, y))));
			}
		}
		for (int y = 0; y < height; ++y)
		{
			gradientColorR = qRed(colors[y]);
			gradientColorG = qGreen(colors[y]);
			gradientColorB = qBlue(colors[y]);
			for (int x = 0; x < width; ++x)
				finalCharacterImage.setPixel(x, y, qRgba(gradientColorR, gradientColorG, gradientColorB, qAlpha(characterImage.pixel(x, y))));
		}
		painter.begin(&finalImage);
		painter.drawImage(shadowXOffset, shadowYOffset, shadowImage);
		painter.drawImage(characterXOffset, characterYOffset, finalCharacterImage);
		painter.end();
	}
	return finalImage;
}
