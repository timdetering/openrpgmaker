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

#ifndef PROJECTDATA_MONSTERANIMATION_H
#define PROJECTDATA_MONSTERANIMATION_H

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include "storagefile.h"
#include "xmlParser.h"

class MonsterAnimation
{
	public:
		friend class ProjectData;
		enum DefaultPoses
		{
			POSE_IDLESTANCE=0,
			POSE_ATTACK=1,
			POSE_SKILLUSED=2,
			POSE_DIEING=3,
			POSE_DEAD=4,
			POSE_DAMAGED=5,
			POSE_BADSTATUS=6,
			POSE_DEFENDING=7,
			POSE_RUNNINGLEFT=8,
			POSE_RUNNINGRIGHT=9,
			POSE_ITEMUSED=10,
			POSE_WOUNDED=11,
			POSE_VICTORY=12
		};
		MonsterAnimation();
		MonsterAnimation(MonsterAnimation &other);
		MonsterAnimation(const MonsterAnimation &other);
		MonsterAnimation(XMLNode MonsterAnimationNode);
		MonsterAnimation(StorageFile &storageFile);
		~MonsterAnimation();
		void saveToStorageFile(StorageFile &storageFile);
		QStringList getResources();
		XMLNode getMonsterAnimationNode();
		QString getName();
		void setName(QString value);
		QString getPoseName(int poseID);
		void setPoseName(int poseID, QString value);
		QString getPoseLocation(int poseID);
		void setPoseLocation(int poseID, QString value);
		int getPoseNumFrames(int poseID);
		void setPoseNumFrames(int poseID, int value);
		int getPoseFPS(int poseID);
		void setPoseFPS(int poseID, int value);
		bool isPosePingPong(int poseID);
		void setPosePingPong(int poseID, bool value);
		static bool tryLoad(XMLNode MonsterAnimationNode);
	private:
		QString name;
		QString poseNames[32];
		QString poseLocations[32];
		int poseNumFrames[32];
		int poseFPS[32];
		bool posePingPong[32];
};

#endif // PROJECTDATA_MONSTERANIMATION_H
