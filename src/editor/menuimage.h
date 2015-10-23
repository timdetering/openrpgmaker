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
 *  Copyright (C) 2011, Justin Davis <tuxdavis@gmail.com>                   *
 ****************************************************************************/

#ifndef MENUIMAGE_H
#define MENUIMAGE_H

#include "menuobject.h"
#include "xmlParser.h"

class MenuImage : public MenuObject
{
	public:
		friend class MenuObject;
		friend class MainMenuLayout;
		friend class PopupMenuLayout;
		friend class TitleMenuLayout;
		friend class MenuItemList;
		enum ImageType {TYPE_PARTYMEMBER=0, TYPE_CHARACTER, TYPE_CLASS, TYPE_MONSTER, TYPE_VEHICLE, TYPE_TILE, TYPE_FILE};
		enum CharacterGraphic {GRAPHIC_FACE=0, GRAPHIC_MAPSPRITE, GRAPHIC_BATTLESPRITE};
		struct PartyMemberImage
		{
			int memberID;
			int graphic;
			int direction;
			int pose;
		};
		struct CharacterImage
		{
			int characterID;
			int graphic;
			int direction;
			int pose;
			bool idIsReferencedInVariable;
		};
		struct ClassImage
		{
			int classID;
			int graphic;
			int direction;
			int pose;
			bool idIsReferencedInVariable;
		};
		struct MonsterImage
		{
			int monsterID;
			int pose;
			bool idIsReferencedInVariable;
		};
		struct VehicleImage
		{
			int vehicleID;
			int direction;
			int pose;
			bool idIsReferencedInVariable;
		};
		struct TileImage
		{
			int tilesetID;
			int tileID;
			bool idsReferencedInVariables;
		};
		MenuImage(MenuObject *parent=0);
		MenuImage(MainMenuLayout *layout, XMLNode ObjectNode, MenuObject *parent=0);
		MenuImage(PopupMenuLayout *layout, XMLNode ObjectNode, MenuObject *parent=0);
		MenuImage(TitleMenuLayout *layout, XMLNode ObjectNode, MenuObject *parent=0);
		MenuImage(MainMenuLayout *layout, StorageFile &storageFile, MenuObject *parent=0);
		MenuImage(PopupMenuLayout *layout, StorageFile &storageFile, MenuObject *parent=0);
		MenuImage(TitleMenuLayout *layout, StorageFile &storageFile, MenuObject *parent=0);
		~MenuImage();
		void getCopy(MenuObject *object);
		XMLNode getObjectNode();
		void saveToStorageFile(StorageFile &storageFile);
		QStringList getResources();
		int getImageType();
		void setImageType(int value);
		void getImageLocation(PartyMemberImage *image);
		void getImageLocation(CharacterImage *image);
		void getImageLocation(ClassImage *image);
		void getImageLocation(MonsterImage *image);
		void getImageLocation(VehicleImage *image);
		void getImageLocation(TileImage *image);
		void getImageLocation(QString *fileLocation);
		void setImageLocation(PartyMemberImage *image);
		void setImageLocation(CharacterImage *image);
		void setImageLocation(ClassImage *image);
		void setImageLocation(MonsterImage *image);
		void setImageLocation(VehicleImage *image);
		void setImageLocation(TileImage *image);
		void setImageLocation(QString fileLocation);
		int getScaledToWidth();
		void setScaledToWidth(int value);
		int getScaledToHeight();
		void setScaledToHeight(int value);
		int getSaveSlotID();
		void setSaveSlotID(int value);
		bool isScaled();
		void setScaled(bool value);
		static bool tryLoad(XMLNode ObjectNode);
		static bool tryLoadImageLocation(XMLNode ImageLocationNode);
		union ImageLocation
		{
			PartyMemberImage *partyMemberImage;
			CharacterImage *characterImage;
			ClassImage *classImage;
			MonsterImage *monsterImage;
			VehicleImage *vehicleImage;
			TileImage *tileImage;
			QString *fileLocation;
		};
	protected:
		void updatePixmap();
		ImageLocation imageLocation;
		int imageType;
		int numFrames;
		int scaledToWidth;
		int scaledToHeight;
		int saveSlotID;
		bool scaled;
	private:
		void loadXMLData(XMLNode ObjectNode);
		void loadStorageData(StorageFile &storageFile);
};

#endif // MENUIMAGE_H
