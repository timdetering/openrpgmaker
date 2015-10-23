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

#ifndef CONFIGUREEVENT_CHANGEFACEGRAPHIC_H
#define CONFIGUREEVENT_CHANGEFACEGRAPHIC_H

#include "ui_configureevent_changefacegraphic.h"

class ConfigureEvent_ChangeFaceGraphic : public QDialog, public Ui::ConfigureEvent_ChangeFaceGraphic
{
	Q_OBJECT
	public:
		ConfigureEvent_ChangeFaceGraphic(QWidget *parent=0);
		~ConfigureEvent_ChangeFaceGraphic();
		void setCharacterID(int characterID);
		void setIsReferencedCharacter(bool referenced);
		void setFaceGraphicLocation(QString location);
		int getCharacterID();
		bool isReferencedCharacter();
		QString getFaceGraphicLocation();
	protected slots:
		void on_bBrowseCharacterVariables_clicked();
		void on_bSetFaceGraphic_clicked();
		void on_cboxCharacters_currentIndexChanged(int index);
		void on_rbSpecificCharacter_toggled(bool checked);
	private:
		QString faceLocation;
};

#endif // CONFIGUREEVENT_CHANGEFACEGRAPHIC_H
