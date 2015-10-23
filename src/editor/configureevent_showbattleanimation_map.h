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
 *  Copyright (C) 2012, Justin Davis <tuxdavis@gmail.com>                   *
 ****************************************************************************/

#ifndef CONFIGUREEVENT_SHOWBATTLEANIMATION_MAP_H
#define CONFIGUREEVENT_SHOWBATTLEANIMATION_MAP_H

#include "ui_configureevent_showbattleanimation_map.h"
#include "projectdata.h"
#include "projectdata_battleanimation.h"

class ConfigureEvent_ShowBattleAnimation_Map : public QDialog, public Ui::ConfigureEvent_ShowBattleAnimation_Map
{
	Q_OBJECT
	public:
		enum TargetType {HERO=0, THIS_EVENT, VEHICLE, EVENT};
		ConfigureEvent_ShowBattleAnimation_Map(int mapID, QWidget *parent=0);
		~ConfigureEvent_ShowBattleAnimation_Map();
		int getBattleAnimationID();
		void setBattleAnimationID(int value);
		int getTargetType();
		void setTargetType(int type);
		int getTargetID();
		void setTargetID(int value);
		bool getHaltOtherProcesses();
		void setHaltOtherProcesses(bool value);
	protected slots:
		void on_cboxBattleAnimation_currentIndexChanged(int index);
	private:
		QList<int> typeList;
		QList<int> idList;
		QList<int> vehicleIndexes;
		QList<int> eventIndexes;
		ProjectData::DataReference<BattleAnimation> *animRef;
};

#endif // CONFIGUREEVENT_SHOWBATTLEANIMATION_MAP_H
