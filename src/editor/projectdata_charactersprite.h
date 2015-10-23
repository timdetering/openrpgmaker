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

#ifndef PROJECTDATA_CHARACTERSPRITE_H
#define PROJECTDATA_CHARACTERSPRITE_H

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include "storagefile.h"
#include "xmlParser.h"

class CharacterSprite
{
	public:
		friend class ProjectData;
		enum DefaultMapPoses
		{
			MAPPOSE_IDLE=0,
			MAPPOSE_WALKING=1,
			MAPPOSE_JUMPING=2,
			MAPPOSE_GRAPPLING=3
		};
		enum DefaultBattlePoses
		{
			BATTLEPOSE_IDLESTANCE=0,
			BATTLEPOSE_ATTACK=1,
			BATTLEPOSE_SKILLUSED=2,
			BATTLEPOSE_DIEING=3,
			BATTLEPOSE_DEAD=4,
			BATTLEPOSE_DAMAGED=5,
			BATTLEPOSE_BADSTATUS=6,
			BATTLEPOSE_DEFENDING=7,
			BATTLEPOSE_RUNNINGLEFT=8,
			BATTLEPOSE_RUNNINGRIGHT=9,
			BATTLEPOSE_ITEMUSED=10,
			BATTLEPOSE_WOUNDED=11,
			BATTLEPOSE_VICTORY=12
		};
		CharacterSprite();
		CharacterSprite(CharacterSprite &other);
		CharacterSprite(const CharacterSprite &other);
		CharacterSprite(XMLNode CharacterSpriteNode);
		CharacterSprite(StorageFile &storageFile);
		~CharacterSprite();
		void saveToStorageFile(StorageFile &storageFile);
		QStringList getResources();
		XMLNode getCharacterSpriteNode();
		QString getName();
		void setName(QString value);
		QString getMapPoseName(int poseID);
		void setMapPoseName(int poseID, QString name);
		QString getMapPoseLocation(int poseID);
		void setMapPoseLocation(int poseID, QString location);
		int getMapPoseNumLoops(int poseID);
		void setMapPoseNumLoops(int poseID, int numLoops);
		bool isMapPosePingPong(int poseID);
		void setMapPosePingPong(int poseID, bool pingPong);
		QString getBattlePoseName(int poseID);
		void setBattlePoseName(int poseID, QString name);
		QString getBattlePoseLocation(int poseID);
		void setBattlePoseLocation(int poseID, QString location);
		int getBattlePoseNumFrames(int poseID);
		void setBattlePoseNumFrames(int poseID, int numFrames);
		int getBattlePoseFPS(int poseID);
		void setBattlePoseFPS(int poseID, int fps);
		bool isBattlePosePingPong(int poseID);
		void setBattlePosePingPong(int poseID, bool pingPong);
		static bool tryLoad(XMLNode CharacterSpriteNode);
	private:
		QString name;
		QStringList mapPoseNames;
		QStringList mapPoseLocations;
		QStringList battlePoseNames;
		QStringList battlePoseLocations;
		int mapPoseNumLoops[32];
		int battlePoseNumFrames[32];
		int battlePoseFPS[32];
		bool mapPosePingPong[32];
		bool battlePosePingPong[32];
};

#endif // PROJECTDATA_CHARACTERSPRITE_H
