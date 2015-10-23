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

#ifndef CONFIGUREEVENT_MEMORIZEPOSITION_H
#define CONFIGUREEVENT_MEMORIZEPOSITION_H

#include "ui_configureevent_memorizeposition.h"

class ConfigureEvent_MemorizePosition : public QDialog, public Ui::ConfigureEvent_MemorizePosition
{
	Q_OBJECT
	public:
		ConfigureEvent_MemorizePosition(QWidget *parent=0);
		~ConfigureEvent_MemorizePosition();
		int getMapVariableID();
		void setMapVariableID(int value);
		int getXVariableID();
		void setXVariableID(int value);
		int getYVariableID();
		void setYVariableID(int value);
		int getLayerVariableID();
		void setLayerVariableID(int value);
	protected slots:
		void on_bBrowseMapVariables_clicked();
		void on_bBrowseXVariables_clicked();
		void on_bBrowseYVariables_clicked();
		void on_bBrowseLayerVariables_clicked();
};

#endif // CONFIGUREEVENT_MEMORIZEPOSITION_H
