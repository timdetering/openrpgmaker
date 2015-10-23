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

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include "clipboard.h"
#include "menuobject.h"
#include "projectdata.h"
#include "projectdata_attribute.h"
#include "projectdata_baseevent.h"
#include "projectdata_battleanimation.h"
#include "projectdata_battleevent.h"
#include "projectdata_battlelayout.h"
#include "projectdata_character.h"
#include "projectdata_charactersprite.h"
#include "projectdata_class.h"
#include "projectdata_commonevent.h"
#include "projectdata_condition.h"
#include "projectdata_globalanimation.h"
#include "projectdata_item.h"
#include "projectdata_mainmenulayout.h"
#include "projectdata_map.h"
#include "projectdata_mapevent.h"
#include "projectdata_monster.h"
#include "projectdata_monsteranimation.h"
#include "projectdata_monstergroup.h"
#include "projectdata_popupmenulayout.h"
#include "projectdata_skill.h"
#include "projectdata_terrain.h"
#include "projectdata_tile.h"
#include "projectdata_tileset.h"
#include "projectdata_vehicle.h"

Clipboard::DataType Clipboard::dataType = Clipboard::TYPE_NONE;
Clipboard::Data Clipboard::data;

void Clipboard::clear()
{
	switch (dataType)
	{
		case TYPE_STRING:
			if (data.string != NULL)
			{
				delete data.string;
				data.string = NULL;
			}
			break;
		case TYPE_PIXMAP:
			if (data.pixmap != NULL)
			{
				delete data.pixmap;
				data.pixmap = NULL;
			}
			break;
		case TYPE_IMAGE:
			if (data.image != NULL)
			{
				delete data.image;
				data.image = NULL;
			}
			break;
		case TYPE_TILEDATA:
			if (data.tileData.data != NULL)
			{
				data.tileData.data->clear();
				delete data.tileData.data;
				data.tileData.data = NULL;
			}
			break;
		case TYPE_ATTRIBUTE:
			if (data.attribute != NULL)
			{
				delete data.attribute;
				data.attribute = NULL;
			}
			break;
		case TYPE_BATTLEANIMATION:
			if (data.battleAnimation != NULL)
			{
				delete data.battleAnimation;
				data.battleAnimation = NULL;
			}
			break;
		case TYPE_BATTLELAYOUT:
			if (data.battleLayout != NULL)
			{
				delete data.battleLayout;
				data.battleLayout = NULL;
			}
			break;
		case TYPE_CHARACTER:
			if (data.character != NULL)
			{
				delete data.character;
				data.character = NULL;
			}
			break;
		case TYPE_CHARACTERSPRITE:
			if (data.characterSprite != NULL)
			{
				delete data.characterSprite;
				data.characterSprite = NULL;
			}
			break;
		case TYPE_CLASS:
			if (data.characterClass != NULL)
			{
				delete data.characterClass;
				data.characterClass = NULL;
			}
			break;
		case TYPE_COMMONEVENT:
			if (data.commonEvent != NULL)
			{
				delete data.commonEvent;
				data.commonEvent = NULL;
			}
			break;
		case TYPE_CONDITION:
			if (data.condition != NULL)
			{
				delete data.condition;
				data.condition = NULL;
			}
			break;
		case TYPE_GLOBALANIMATION:
			if (data.globalAnimation != NULL)
			{
				delete data.globalAnimation;
				data.globalAnimation = NULL;
			}
			break;
		case TYPE_GLOBALANIMATION_IMAGELOCATION:
			if (data.globalAnimation_imageLocation != NULL)
			{
				delete data.globalAnimation_imageLocation;
				data.globalAnimation_imageLocation = NULL;
			}
		case TYPE_ITEM:
			if (data.item != NULL)
			{
				delete data.item;
				data.item = NULL;
			}
			break;
		case TYPE_MAINMENULAYOUT:
			if (data.mainMenuLayout != NULL)
			{
				delete data.mainMenuLayout;
				data.mainMenuLayout = NULL;
			}
			break;
		case TYPE_MAP:
			if (data.map != NULL)
			{
				delete data.map;
				data.map = NULL;
			}
			break;
		case TYPE_MENUOBJECT:
			if (data.menuObject != NULL)
			{
				delete data.menuObject;
				data.menuObject = NULL;
			}
			break;
		case TYPE_MONSTER:
			if (data.monster != NULL)
			{
				delete data.monster;
				data.monster = NULL;
			}
			break;
		case TYPE_MONSTERANIMATION:
			if (data.monsterAnimation != NULL)
			{
				delete data.monsterAnimation;
				data.monsterAnimation = NULL;
			}
		case TYPE_MONSTERGROUP:
			if (data.monsterGroup != NULL)
			{
				delete data.monsterGroup;
				data.monsterGroup = NULL;
			}
			break;
		case TYPE_POPUPMENULAYOUT:
			if (data.popupMenuLayout != NULL)
			{
				delete data.popupMenuLayout;
				data.popupMenuLayout = NULL;
			}
		case TYPE_SKILL:
			if (data.skill != NULL)
			{
				delete data.skill;
				data.skill = NULL;
			}
			break;
		case TYPE_TERRAIN:
			if (data.terrain != NULL)
			{
				delete data.terrain;
				data.terrain = NULL;
			}
			break;
		case TYPE_TILE:
			if (data.tile != NULL)
			{
				delete data.tile;
				data.tile = NULL;
			}
			break;
		case TYPE_TILESET:
			if (data.tileset != NULL)
			{
				delete data.tileset;
				data.tileset = NULL;
			}
			break;
		case TYPE_VEHICLE:
			if (data.vehicle != NULL)
			{
				delete data.vehicle;
				data.vehicle = NULL;
			}
			break;
		case TYPE_MAPEVENT:
			if (data.mapEvent != NULL)
			{
				delete data.mapEvent;
				data.mapEvent = NULL;
			}
			break;
		case TYPE_BASEEVENT:
			if (data.baseEvent != NULL)
			{
				delete data.baseEvent;
				data.baseEvent = NULL;
			}
			break;
		case TYPE_MAPEVENTPAGE:
			if (data.mapEventPage != NULL)
			{
				delete data.mapEventPage;
				data.mapEventPage = NULL;
			}
			break;
		default: // DO NOTHING
			break;
	}
	dataType = TYPE_NONE;
}
