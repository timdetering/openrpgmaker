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

#ifndef PROJECTDATA_VEHICLE_H
#define PROJECTDATA_VEHICLE_H

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include "audiomanager.h"
#include "storagefile.h"
#include "xmlParser.h"

class Vehicle
{
	public:
		friend class ProjectData;
		Vehicle();
		Vehicle(Vehicle &other);
		Vehicle(const Vehicle &other);
		Vehicle(XMLNode VehicleNode);
		Vehicle(StorageFile &storageFile);
		~Vehicle();
		void saveToStorageFile(StorageFile &storageFile);
		QStringList getResources();
		XMLNode getVehicleNode();
		QString getName();
		void setName(QString value);
		int getSpriteID();
		void setSpriteID(int id);
		void getBGMInfo(AudioManager::MusicInfo *info);
		void setBGMInfo(AudioManager::MusicInfo info);
		int getPassingPhase();
		void setPassingPhase(int phase);
		int getLandingPhase();
		void setLandingPhase(int phase);
		bool isFlying();
		void setIsFlying(bool value);
		static bool tryLoad(XMLNode VehicleNode);
	private:
		QString name;
		AudioManager::MusicInfo bgmInfo;
		int spriteID;
		int passingPhase;
		int landingPhase;
		bool flying;
};

#endif // PROJECTDATA_VEHICLE_H
