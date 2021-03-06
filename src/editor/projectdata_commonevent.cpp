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
#include "projectdata.h"
#include "projectdata_commonevent.h"
#include "xmlParser.h"
#include "xmlTools.h"

CommonEvent::CommonEvent()
{
	name = "TODO";
}

CommonEvent::CommonEvent(CommonEvent &other)
{
	name = "TODO";
}

CommonEvent::CommonEvent(const CommonEvent &other)
{
	name = "TODO";
}

CommonEvent::CommonEvent(XMLNode CommonEventNode)
{
	name = "TODO";
}

CommonEvent::CommonEvent(StorageFile &storageFile)
{
	name = "TODO";
}

CommonEvent::~CommonEvent()
{
}

void CommonEvent::saveToStorageFile(StorageFile &storageFile)
{
}

XMLNode CommonEvent::getCommonEventNode()
{
}

QString CommonEvent::getName()
{
	return name;
}

void CommonEvent::setName(QString value)
{
	name = value;
}

bool CommonEvent::tryLoad(XMLNode CommonEventNode)
{
}
