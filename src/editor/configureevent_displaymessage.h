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

#ifndef CONFIGUREEVENT_DISPLAYMESSAGE_H
#define CONFIGUREEVENT_DISPLAYMESSAGE_H

#include "ui_configureevent_displaymessage.h"

class ConfigureEvent_DisplayMessage : public QDialog, public Ui::ConfigureEvent_DisplayMessage
{
	Q_OBJECT
	public:
		ConfigureEvent_DisplayMessage(QWidget *parent=0);
		~ConfigureEvent_DisplayMessage();
		QString getFaceLocation();
		void setFaceLocation(QString value);
		QString getMessage();
		void setMessage(QString value);
		int getX();
		void setX(int value);
		int getY();
		void setY(int value);
		int getWidth();
		void setWidth(int value);
		int getHeight();
		void setHeight(int value);
		int getXPadding();
		void setXPadding(int value);
		int getYPadding();
		void setYPadding(int value);
		int getOpacity();
		void setOpacity(int value);
		bool getCoordinatesStoredInVariables();
		void setCoordinatesStoredInVariables(bool stored);
		bool getSizeStoredInVariables();
		void setSizeStoredInVariables(bool stored);
		bool getOpacityStoredInVariable();
		void setOpacityStoredInVariable(bool stored);
		bool getBlockEvents();
		void setBlockEvents(bool blockEvents);
		bool getFlipFaceHorizontaly();
		void setFlipFaceHorizontaly(bool flip);
		bool getFaceDisplayedOnLeft();
		void setFaceDisplayedOnLeft(bool displayOnLeft);
		int getFontID();
		void setFontID(int font);
		QColor getTopColor();
		QColor getBottomColor();
		void setColor(QColor top, QColor bottom);
	protected slots:
		void on_aChangeColor_triggered();
		void on_aChangeDisplaySpeed_triggered();
		void on_aChangeFaceGraphic_triggered();
		void on_aCloseWindow_triggered();
		void on_aInsertBitmap_triggered();
		void on_aInsertDollarSign_triggered();
		void on_aInsertPause_triggered();
		void on_aInsertSpecialText_triggered();
		void on_bBrowseHeightVariables_clicked();
		void on_bBrowseOpacityVariables_clicked();
		void on_bBrowseWidthVariables_clicked();
		void on_bBrowseXVariables_clicked();
		void on_bBrowseYVariables_clicked();
		void on_bClearFaceGraphic_clicked();
		void on_bPreviewMessage_clicked();
		void on_bSetFaceGraphic_clicked();
		void on_buttonBox_helpRequested();
		void on_cbFlipFaceHorizontaly_toggled(bool checked);
		void on_rbSpecificCoordinates_toggled(bool checked);
		void on_rbSpecificOpacity_toggled(bool checked);
		void on_rbSpecificSize_toggled(bool checked);
		void on_sbHeight_valueChanged(int value);
		void on_sbYPadding_valueChanged(int value);
		void updateGradientPreview();
	private:
		QString faceLocation;
};

#endif // CONFIGUREEVENT_DISPLAYMESSAGE_H
