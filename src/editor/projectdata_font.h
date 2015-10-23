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

#ifndef PROJECTDATA_FONT_H
#define PROJECTDATA_FONT_H

#include <QString>
#include <QMap>
#include <QImage>
#include "storagefile.h"
#include "xmlParser.h"

class Font
{
	public:
		Font();
		Font(XMLNode FontNode);
		Font(StorageFile &storageFile);
		~Font();
		XMLNode getFontNode();
		void saveToStorageFile(StorageFile &storageFile);
		QStringList getResources();
		void setData(QString image, QString characters, int charWidth, int charHeight, int space, int tab, int dropX, int dropY);
		QString getName();
		void setName(QString value);
		QImage getCharacterImage(unsigned short character);
		int getCharacterWidth(unsigned short character);
		QString getCharacterList();
		QString getImageLocation();
		QImage getRenderedText(QString text, QColor topColor, QColor bottomColor, bool drawDropShadow=true);
		int getTextWidth(QString text);
		int getCharacterWidth();
		int getCharacterHeight();
		int getSpaceWidth();
		int getTabWidth();
		int getDropShadowXOffset();
		int getDropShadowYOffset();
		static bool tryLoad(XMLNode FontNode);
	private:
		void loadCharacterImages();
		QImage getRenderedCharacter(unsigned short character, QList<QRgb> colors, bool drawDropShadow);
		QMap<unsigned short, QImage> characterImages;
		QMap<unsigned short, int> characterWidths;
		QImage unknownImage;
		QString name;
		QString characterList;
		QString imageLocation;
		int characterWidth;
		int characterHeight;
		int spaceWidth;
		int tabWidth;
		int dropShadowXOffset;
		int dropShadowYOffset;
};

#endif //PROJECTDATA_FONT_H
