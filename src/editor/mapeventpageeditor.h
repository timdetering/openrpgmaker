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

#ifndef MAPEVENTPAGEEDITOR_H
#define MAPEVENTPAGEEDITOR_H

#include "ui_mapeventpageeditor.h"
#include "projectdata.h"
#include "projectdata_mapevent.h"

class MapEventPageEditor : public QWidget, public Ui::MapEventPageEditor
{
	Q_OBJECT
	public:
		MapEventPageEditor(QWidget *parent=0);
		~MapEventPageEditor();
		void setupEditor(int map, MapEvent *event, int pageID);
		void applyChanges();
	signals:
		void eventCopied();
	protected slots:
		void on_cboxConditionsMet_currentIndexChanged(int index);
		void on_bDeletePrecondition_clicked();
		void on_bAddPrecondition_clicked();
		void on_lwPreconditions_itemDoubleClicked(QListWidgetItem *item);
		void on_sbSpriteOpacity_valueChanged(int value);
		void on_bSetEventGraphic_clicked();
		void on_cboxAnimationType_currentIndexChanged(int index);
		void on_cboxMovementFrequency_currentIndexChanged(int index);
		void on_cboxMovementType_currentIndexChanged(int index);
		void on_cboxTriggerCondition_currentIndexChanged(int index);
		void on_bSetEventID_clicked();
		void on_bConfigureMovementPattern_clicked();
		void animateSpriteGraphic();
	private:
		MapEvent *mapEvent;
		MapEvent::Page *currentPage;
		QGraphicsPixmapItem *spriteItem;
		QList<QPointF> spriteFrames;
		QTimer *animationTimer;
		int currentSpriteFrame;
		int mapID;
		static QList<int> movementFrequencyTable;
};

#endif // MAPEVENTPAGEEDITOR_H
