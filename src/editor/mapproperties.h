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

#ifndef MAPPROPERTIES_H
#define MAPPROPERTIES_H

class QGraphicsPixmapItem;
class Map;
class QGraphicsScene;
class QCloseEvent;
class MonsterEncounterDelegate;

#include "ui_mapproperties.h"
#include "audiomanager.h"
#include "projectdata.h"

class MapProperties : public QDialog, public Ui::MapProperties
{
	Q_OBJECT
	public:
		MapProperties(QWidget *parent=0);
		~MapProperties();
		void setupMapProperties(int mapID);
	protected slots:
		// Left Data
		void on_tbAddEncounter_clicked();
		void on_tbSubEncounter_clicked();
		// Middle Data
		void on_cbUseBG_toggled(bool on);
		void on_gvBGPreview_mouseDoubleClickEvent();
		void on_bSetBG_clicked();
		void on_rbBGM_Specify_toggled(bool on);
		void on_tbBGM_Browse_clicked();
		void on_rbBBG_Specify_toggled(bool on);
		void on_tbBBG_Browse_clicked();
		// Misc
		void on_buttonBox_accepted();
		void on_buttonBox_helpRequested();
	private:
		void closeEvent(QCloseEvent *event);
		ProjectData::DataReference<Map> *mapRef;
		QGraphicsScene *bgPreviewScene;
		QGraphicsPixmapItem *bgPreviewItem;
		MonsterEncounterDelegate *monsterEncounterDelegate;
		AudioManager::MusicInfo bgmInfo;
		QString bgLocation;
		QString bgmLocation;
		QString bbgLocation;
};

#endif // MAPPROPERTIES_H
