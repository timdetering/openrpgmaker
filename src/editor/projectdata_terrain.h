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

#ifndef PROJECTDATA_TERRAIN_H
#define PROJECTDATA_TERRAIN_H

#include <QString>
#include <QList>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include "audiomanager.h"
#include "storagefile.h"
#include "xmlParser.h"

class Terrain
{
	public:
		friend class ProjectData;
		enum GridDepth {DEPTH_SHALLOW=0, DEPTH_DEEP=1, DEPTH_CUSTOM=2};
		enum SpriteDisplay {DISPLAY_NORMAL=0, DISPLAY_OBSBOTHALF=1, DISPLAY_OBSBOTTHIRD=2, DISPLAY_SEMITRANSPARENT=3};
		enum BackgroundAssociation {BG_BACKGROUND=0, BG_FRAME=1};
		enum GlobalAnimationType {TYPE_BACKGROUND=0, TYPE_FOREGROUND=1};
		struct GridDepthValues
		{
			int depth;
			int hTopLineLocation;
			int hBottomLineLocation;
			int vMonsterLine_TopLocation;
			int vMonsterLine_BottomLocation;
			int vMonsterLine_Spacing;
			int vCharacterLine_TopLocation;
			int vCharacterLine_BottomLocation;
			int vCharacterLine_Spacing;
		};
		Terrain();
		Terrain(Terrain &other);
		Terrain(const Terrain &other);
		Terrain(XMLNode TerrainNode);
		Terrain(StorageFile &storageFile);
		~Terrain();
		void saveToStorageFile(StorageFile &storageFile);
		QStringList getResources();
		XMLNode getTerrainNode();
		QString getName();
		void setName(QString value);
		QString getBGImageLocation();
		void setBGImageLocation(QString location);
		QString getFGImageLocation();
		void setFGImageLocation(QString location);
		void getSoundEffectInfo(AudioManager::SoundEffectInfo *info);
		void setSoundEffectInfo(AudioManager::SoundEffectInfo info);
		int getDamage();
		void setDamage(int value);
		int getEncMultiplier();
		void setEncMultiplier(int value);
		int getGridDepth();
		void setGridDepth(int value);
		int getGridDepth_hTopLineLocation();
		void setGridDepth_hTopLineLocation(int value);
		int getGridDepth_hBottomLineLocation();
		void setGridDepth_hBottomLineLocation(int value);
		int getGridDepth_vMonsterLine_TopLocation();
		void setGridDepth_vMonsterLine_TopLocation(int value);
		int getGridDepth_vMonsterLine_BottomLocation();
		void setGridDepth_vMonsterLine_BottomLocation(int value);
		int getGridDepth_vMonsterLine_Spacing();
		void setGridDepth_vMonsterLine_Spacing(int value);
		int getGridDepth_vCharacterLine_TopLocation();
		void setGridDepth_vCharacterLine_TopLocation(int value);
		int getGridDepth_vCharacterLine_BottomLocation();
		void setGridDepth_vCharacterLine_BottomLocation(int value);
		int getGridDepth_vCharacterLine_Spacing();
		void setGridDepth_vCharacterLine_Spacing(int value);
		GridDepthValues *getGridDepthValues();
		int getSpriteDisplay();
		void setSpriteDisplay(int value);
		int getBackgroundAssociation();
		void setBackgroundAssociation(int value);
		int getBGHScrollSpeed();
		void setBGHScrollSpeed(int value);
		int getBGVScrollSpeed();
		void setBGVScrollSpeed(int value);
		int getFGHScrollSpeed();
		void setFGHScrollSpeed(int value);
		int getFGVScrollSpeed();
		void setFGVScrollSpeed(int value);
		int getInitPartyMod();
		void setInitPartyMod(int value);
		int getBackAttackEnemyMod();
		void setBackAttackEnemyMode(int value);
		int getSurroundAttackPartyMod();
		void setSurroundAttackPartyMod(int value);
		int getSurroundAttackEnemyMod();
		void setSurroundAttackEnemyMod(int value);
		int getGlobalAnimID();
		void setGlobalAnimID(int value);
		int getGlobalAnimType();
		void setGlobalAnimType(int value);
		bool playSoundOnlyIfDamaged();
		void setPlaySoundOnlyIfDamaged(bool value);
		static bool tryLoad(XMLNode TerrainNode);
		static GridDepthValues gridDepth_ShallowValues;
		static GridDepthValues gridDepth_DeepValues;
	private:
		QString name;
		QString bgImageLocation;
		QString fgImageLocation;
		AudioManager::SoundEffectInfo soundEffect;
		GridDepthValues gridDepth;
		int damage;
		int encMultiplier;
		int spriteDisplay;
		int backgroundAssociation;
		int bgHScrollSpeed;
		int bgVScrollSpeed;
		int fgHScrollSpeed;
		int fgVScrollSpeed;
		int initPartyMod;
		int backAttackEnemyMod;
		int surroundAttackPartyMod;
		int surroundAttackEnemyMod;
		int globalAnimID;
		int globalAnimType;
		bool soundOnlyIfDamaged;
};

#endif // PROJECTDATA_TERRAIN_H
