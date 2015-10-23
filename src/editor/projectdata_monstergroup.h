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

#ifndef PROJECTDATA_MONSTERGROUP_H
#define PROJECTDATA_MONSTERGROUP_H

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include "storagefile.h"
#include "xmlParser.h"

class MonsterGroup
{
	public:
		friend class ProjectData;
		enum Alignment {ALIGN_MANUAL=0, ALIGN_AUTOMATIC};
		struct MonsterPosition
		{
			int monsterID;
			int x;
			int y;
		};
		MonsterGroup();
		MonsterGroup(MonsterGroup &other);
		MonsterGroup(const MonsterGroup &other);
		MonsterGroup(XMLNode MonsterGroupNode);
		MonsterGroup(StorageFile &storageFile);
		~MonsterGroup();
		void saveToStorageFile(StorageFile &storageFile);
		QStringList getResources();
		XMLNode getMonsterGroupNode();
		QString getName();
		void setName(QString value);
		void addMonster(MonsterPosition position);
		void deleteMonsterPosition(int positionID);
		MonsterPosition *getMonsterPositionPointer(int positionID);
		bool getHabitat(int terrainID);
		void setHabitat(int terrainID, bool value);
		void resizeHabitat(int numTerrain);
		BattleEvent *getBattleEventPointer();
		int getAlignment();
		void setAlignment(int value);
		static bool tryLoad(XMLNode MonsterGroupNode);
	private:
		QString name;
		QList<MonsterPosition> monsterLayout;
		QList<bool> habitat;
		BattleEvent *battleEvent;
		int alignment;
};

#endif // PROJECTDATA_MONSTERGROUP_H
