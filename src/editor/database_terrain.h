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

#ifndef DATABASE_TERRAIN_H
#define DATABASE_TERRAIN_H

class Terrain;

#include "ui_database_terrain.h"
#include "projectdata.h"

class Database_Terrain : public QWidget, public Ui::Database_Terrain
{
	Q_OBJECT
	public:
		Database_Terrain(QWidget *parent=0);
		~Database_Terrain();
		void setupTerrain();
		void shutdownTerrain();
	protected slots:
		void on_twTerrainList_cellClicked(int row);
		void on_bArraySize_clicked();
		void on_leName_textChanged(QString text);
		void on_nbrDamage_valueChanged(int value);
		void on_nbrEncMul_valueChanged(int value);
		void on_bBrowseSounds_clicked();
		void on_cbSoundIfDamaged_toggled(bool on);
		void on_rbBackground_toggled(bool on);
		void on_rbFrame_toggled(bool on);
		void on_cboxGlobalAnimation_currentIndexChanged(int index);
		void on_rbGlobalAnimBG_toggled(bool on);
		void on_rbGlobalAnimFG_toggled(bool on);
		void on_bSetBG_clicked();
		void on_nbrBGHScrollSpeed_valueChanged(int value);
		void on_nbrBGVScrollSpeed_valueChanged(int value);
		void on_bSetFG_clicked();
		void on_nbrFGHScrollSpeed_valueChanged(int value);
		void on_nbrFGVScrollSpeed_valueChanged(int value);
		void on_cbInitiative_Party_toggled();
		void on_nbrInitiativePercent_Party_valueChanged(int value);
		void on_cbBackAttack_Enemy_toggled();
		void on_nbrBackAttackPercent_Enemy_valueChanged(int value);
		void on_cbSurroundAttack_Party_toggled();
		void on_nbrSurroundAttackPercent_Party_valueChanged(int value);
		void on_cbSurroundAttack_Enemy_toggled();
		void on_nbrSurroundAttackPercent_Enemy_valueChanged(int value);
		void on_rbShallowGD_toggled(bool on);
		void on_rbDeepGD_toggled(bool on);
		void on_rbCustomGD_toggled(bool on);
		void on_bConfigureCustomGD_clicked();
		void on_rbNormal_toggled(bool on);
		void on_rbObsBottHalf_toggled(bool on);
		void on_rbObsBottThird_toggled(bool on);
		void on_rbSemiTransparent_toggled(bool on);
	private:
		void updateTerrainList();
		void updateGlobalAnimationList();
		void updateGridDepthLines();
		QGraphicsScene *bgPreviewScene;
		QGraphicsPixmapItem *bgPreview_BGImageItem;
		QGraphicsPixmapItem *bgPreview_FGImageItem;
		QGraphicsLineItem *hLine_Top;
		QGraphicsLineItem *hLine_CenterTop;
		QGraphicsLineItem *hLine_CenterBottom;
		QGraphicsLineItem *hLine_Bottom;
		QGraphicsLineItem *vLine_Monsters_Left;
		QGraphicsLineItem *vLine_Monsters_Right;
		QGraphicsLineItem *vLine_Center;
		QGraphicsLineItem *vLine_Characters_Left;
		QGraphicsLineItem *vLine_Characters_Right;
		ProjectData::DataReference<Terrain> *terrainRef;
		bool ignoreEvents;
};

#endif // DATABASE_TERRAIN
