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

#ifndef PROJECTDATA_ATTRIBUTE_H
#define PROJECTDATA_ATTRIBUTE_H

#include <QString>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include "storagefile.h"
#include "xmlParser.h"

class Attribute
{
	public:
		friend class ProjectData;
		enum AttributeType {TYPE_WEAPON=0, TYPE_MAGIC};
		Attribute();
		Attribute(Attribute &other);
		Attribute(const Attribute &other);
		Attribute(XMLNode AttributeNode);
		Attribute(StorageFile &storageFile);
		~Attribute();
		void saveToStorageFile(StorageFile &storageFile);
		QStringList getResources();
		XMLNode getAttributeNode();
		QString getName();
		void setName(QString value);
		int getType();
		void setType(int value);
		int getMultiplierA();
		void setMultiplierA(int value);
		int getMultiplierB();
		void setMultiplierB(int value);
		int getMultiplierC();
		void setMultiplierC(int value);
		int getMultiplierD();
		void setMultiplierD(int value);
		int getMultiplierE();
		void setMultiplierE(int value);
		static bool tryLoad(XMLNode AttributeNode);
	private:
		QString name;
		int type;
		int multiplierA;
		int multiplierB;
		int multiplierC;
		int multiplierD;
		int multiplierE;
};

#endif // PROJECTDATA_ATTRIBUTE_H
