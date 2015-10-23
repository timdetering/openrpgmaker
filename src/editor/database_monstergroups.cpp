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

#include <QMessageBox>
#include <QGraphicsPixmapItem>
#include <QGraphicsRectItem>
#include <QGraphicsScene>
#include <QPixmap>
#include <QToolTip>
#include <QString>
#include <QCheckBox>
#include <QListWidgetItem>
#include <QHeaderView>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cmath>
#include "arraysize.h"
#include "clipboard.h"
#include "database_monstergroups.h"
#include "imageview.h"
#include "projectdata.h"
#include "projectdata_monster.h"
#include "projectdata_monsteranimation.h"
#include "projectdata_monstergroup.h"
#include "projectdata_terrain.h"
#include "xmlParser.h"

Database_MonsterGroups::Database_MonsterGroups(QWidget *parent) : QWidget(parent)
{
	setupUi(this);
}

Database_MonsterGroups::~Database_MonsterGroups()
{
}

void Database_MonsterGroups::setupMonsterGroups()
{
}

void Database_MonsterGroups::shutdownMonsterGroups()
{
}
