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

#ifndef CLIPBOARD_H
#define CLIPBOARD_H

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include "projectdata.h"
#include "projectdata_battleanimation.h"
#include "projectdata_globalanimation.h"
#include "projectdata_map.h"
#include "projectdata_mapevent.h"

class MenuObject;
class BaseEvent;

class Clipboard
{
	public:
		static void clear();
		enum DataType
		{
			TYPE_NONE=-1,
			TYPE_DOUBLE,
			TYPE_FLOAT,
			TYPE_ULONG,
			TYPE_SLONG,
			TYPE_UINT,
			TYPE_SINT,
			TYPE_USINT,
			TYPE_SSINT,
			TYPE_UCHAR,
			TYPE_SCHAR,
			TYPE_STRING,
			TYPE_PIXMAP,
			TYPE_IMAGE,
			TYPE_TILEDATA,
			TYPE_ATTRIBUTE,
			TYPE_BATTLEANIMATION,
			TYPE_BATTLEANIMATION_IMAGELOCATION,
			TYPE_BATTLELAYOUT,
			TYPE_CHARACTER,
			TYPE_CHARACTERSPRITE,
			TYPE_CLASS,
			TYPE_COMMONEVENT,
			TYPE_CONDITION,
			TYPE_GLOBALANIMATION,
			TYPE_GLOBALANIMATION_IMAGELOCATION,
			TYPE_ITEM ,
			TYPE_MAINMENULAYOUT,
			TYPE_MAP,
			TYPE_MENUOBJECT,
			TYPE_MONSTER,
			TYPE_MONSTERANIMATION,
			TYPE_MONSTERGROUP,
			TYPE_POPUPMENULAYOUT,
			TYPE_SKILL,
			TYPE_TERRAIN,
			TYPE_TILE,
			TYPE_TILESET,
			TYPE_VEHICLE,
			TYPE_MAPEVENT,
			TYPE_BASEEVENT,
			TYPE_MAPEVENTPAGE
		};
		union Data
		{
			double valueDouble;
			float valueFloat;
			unsigned long unsignedLong;
			signed long signedLong;
			unsigned int unsignedInt;
			signed int signedInt;
			unsigned short unsignedShort;
			signed short signedShort;
			unsigned char unsignedChar;
			signed char signedChar;
			QString *string;
			QPixmap *pixmap;
			QImage *image;
			struct
			{
				QList<Map::TileData> *data;
				int width;
				int height;
			} tileData;
			Attribute *attribute;
			BattleAnimation *battleAnimation;
			BattleAnimation::ImageLocation *battleAnimation_imageLocation;
			BattleLayout *battleLayout;
			Character *character;
			CharacterSprite *characterSprite;
			Class *characterClass;
			CommonEvent *commonEvent;
			Condition *condition;
			GlobalAnimation *globalAnimation;
			GlobalAnimation::ImageLocation *globalAnimation_imageLocation;
			Item *item;
			MainMenuLayout *mainMenuLayout;
			Map *map;
			MenuObject *menuObject;
			Monster *monster;
			MonsterAnimation *monsterAnimation;
			MonsterGroup *monsterGroup;
			PopupMenuLayout *popupMenuLayout;
			Skill *skill;
			Terrain *terrain;
			Tile *tile;
			Tileset *tileset;
			Vehicle *vehicle;
			MapEvent *mapEvent;
			BaseEvent *baseEvent;
			MapEvent::Page *mapEventPage;
		};
		static DataType dataType;
		static Data data;
};

#endif // CLIPBOARD_H