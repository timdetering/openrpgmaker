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

#ifndef CONFIGUREEVENT_KEYINPUTPROCESSING_H
#define CONFIGUREEVENT_KEYINPUTPROCESSING_H

#include "ui_configureevent_keyinputprocessing.h"

class ConfigureEvent_KeyInputProcessing : public QDialog, public Ui::ConfigureEvent_KeyInputProcessing
{
	Q_OBJECT
	public:
		ConfigureEvent_KeyInputProcessing(QWidget *parent=0);
		~ConfigureEvent_KeyInputProcessing();
		void setVariableID(int variableID);
		void setWaitForKeyPress(bool wait);
		void setStoreTimeTaken(bool storeTimeTaken);
		void setTimeTakenVariableID(int variableID);
		int getVariableID();
		bool waitForKeyPress();
		bool storeTimeTaken();
		int getTimeTakenVariableID();
	protected slots:
		void on_bBrowseTimeVariables_clicked();
		void on_bBrowseVariables_clicked();
		void on_cbStoreTimeTaken_toggled(bool checked);
		void on_cbWait_toggled(bool checked);
};

#endif // CONFIGUREEVENT_KEYINPUTPROCESSING_H
