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

#ifndef PROJECTDATA_COMMONEVENT_H
#define PROJECTDATA_COMMONEVENT_H

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include "storagefile.h"
#include "xmlParser.h"

class CommonEvent
{
	public:
		friend class ProjectData;
		CommonEvent();
		CommonEvent(CommonEvent &other);
		CommonEvent(const CommonEvent &other);
		CommonEvent(XMLNode CommonEventNode);
		CommonEvent(StorageFile &storageFile);
		~CommonEvent();
		void saveToStorageFile(StorageFile &storageFile);
		XMLNode getCommonEventNode();
		QString getName();
		void setName(QString value);
		static bool tryLoad(XMLNode CommonEventNode);
	private:
		QString name;
};

#endif // PROJECTDATA_COMMONEVENT_H
