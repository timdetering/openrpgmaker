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
#include "projectdata.h"
#include "projectdata_map.h"
#include "randommapgenerator_mountainstructure.h"
#include "randomnumbergenerator.h"
#include "tilebutton.h"
#include "tilechooser.h"
#include "xmlParser.h"

RandomMapGenerator_MountainStructure::RandomMapGenerator_MountainStructure(QWidget *parent) : QDialog(parent)
{
	setupUi(this);
	resize(sizeHint());
	setMinimumWidth(width());
	setMaximumWidth(width());
	setMinimumHeight(height());
	setMaximumHeight(height());
}

RandomMapGenerator_MountainStructure::~RandomMapGenerator_MountainStructure()
{
}

void RandomMapGenerator_MountainStructure::setupGenerator(XMLNode ConfigurationNode)
{
}

XMLNode RandomMapGenerator_MountainStructure::getConfigurationNode()
{
}

bool RandomMapGenerator_MountainStructure::isConfigured()
{
}

QStringList RandomMapGenerator_MountainStructure::getGeneratedMapNames()
{
}

Map *RandomMapGenerator_MountainStructure::getGeneratedMap(int mapNum)
{
}

void RandomMapGenerator_MountainStructure::generateMountainStructure(unsigned long seed)
{
}
