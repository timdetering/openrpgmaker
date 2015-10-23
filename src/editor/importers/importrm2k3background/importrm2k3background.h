/****************************************************************************
*  Open RPG MAker is free software: you can redistribute it and/or modify  *
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

#ifndef IMPORTRM2K3BACKGROUND_H
#define IMPORTRM2K3BACKGROUND_H

class Tileset;
class Tile;

#include <QImage>
#include <QString>
#include <QObject>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include "importerplugin.h"

class ImportRM2K3Background : public ImporterPlugin
{
	Q_OBJECT
	public:
		virtual ~ImportRM2K3Background();
		virtual void runImporter();
		virtual QString getImporterName();
		virtual QString getImporterDescription();
	protected:
		void importRM2K3Background(QString fileLocation);
};

#endif // IMPORTRM2K3BACKGROUND_H
