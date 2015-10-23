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

#ifndef CONFIGUREEVENT_SHOWGLOBALANIMATION_H
#define CONFIGUREEVENT_SHOWGLOBALANIMATION_H

#include "ui_configureevent_showglobalanimation.h"
#include "projectdata.h"
#include "projectdata_globalanimation.h"

class ConfigureEvent_ShowGlobalAnimation : public QDialog, public Ui::ConfigureEvent_ShowGlobalAnimation
{
	Q_OBJECT
	public:
		enum Action {PLAY_ONCE=0, BEGIN_PLAYING, STOP_PLAYING};
		ConfigureEvent_ShowGlobalAnimation(QWidget *parent=0);
		~ConfigureEvent_ShowGlobalAnimation();
		int getAction();
		void setAction(int value);
		int getAnimationID();
		void setAnimationID(int value);
		bool getHaltOtherProcesses();
		void setHaltOtherProcesses(bool value);
	protected slots:
		void on_cboxGlobalAnimation_currentIndexChanged(int index);
		void on_rbBeginPlaying_toggled(bool checked);
		void on_rbPlayOnce_toggled(bool checked);
		void on_rbStopPlaying_toggled(bool checked);
	private:
		ProjectData::DataReference<GlobalAnimation> *animRef;
};

#endif // CONFIGUREEVENT_SHOWGLOBALANIMATION_H
