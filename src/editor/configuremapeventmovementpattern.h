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

#ifndef CONFIGUREMAPEVENTMOVEMENTPATTERN_H
#define CONFIGUREMAPEVENTMOVEMENTPATTERN_H

#include "ui_configuremapeventmovementpattern.h"
#include "projectdata_mapevent.h"

class ConfigureMapEventMovementPattern : public QDialog, public Ui::ConfigureMapEventMovementPattern
{
	Q_OBJECT
	public:
		ConfigureMapEventMovementPattern(QWidget *parent=0);
		~ConfigureMapEventMovementPattern();
		void addPattern(MapEvent::MovementPattern *pattern);
		MapEvent::MovementPattern *getPattern(int location);
		int numPatterns();
		bool getRepeatPattern();
		void setRepeatPattern(bool value);
		bool getIgnoreImpossibleMoves();
		void setIgnoreImpossibleMoves(bool value);
	protected slots:
		void on_bDelete_clicked();
		void on_bClearList_clicked();
		void on_bMoveSelectedUp_clicked();
		void on_bMoveSelectedDown_clicked();
		void on_bMoveUpLeft_clicked();
		void on_bMoveUp_clicked();
		void on_bMoveUpRight_clicked();
		void on_bMoveRight_clicked();
		void on_bMoveDownRight_clicked();
		void on_bMoveDown_clicked();
		void on_bMoveDownLeft_clicked();
		void on_bMoveLeft_clicked();
		void on_bMoveRandomly_clicked();
		void on_bIncreaseMoveSpeed_clicked();
		void on_bDecreaseMoveSpeed_clicked();
		void on_bMoveForward_clicked();
		void on_bMoveBackward_clicked();
		void on_bMoveTowardHero_clicked();
		void on_bMoveAwayFromHero_clicked();
		void on_bFaceUp_clicked();
		void on_bFaceDown_clicked();
		void on_bFaceLeft_clicked();
		void on_bFaceRight_clicked();
		void on_bTurnLeft_clicked();
		void on_bTurnRight_clicked();
		void on_bUTurn_clicked();
		void on_bFaceRandomDirection_clicked();
		void on_bFaceLeftRightRandomly_clicked();
		void on_bFaceHero_clicked();
		void on_bFaceAwayFromHero_clicked();
		void on_bIncreaseMovementFrequency_clicked();
		void on_bDecreaseMovementFrequency_clicked();
		void on_bBeginJump_clicked();
		void on_bEndJump_clicked();
		void on_bLockFacing_clicked();
		void on_bUnlockFacing_clicked();
		void on_bWait_clicked();
		void on_bPhasingModeOFF_clicked();
		void on_bPhasingModeON_clicked();
		void on_bStopAnimation_clicked();
		void on_bResumeAnimation_clicked();
		void on_bChangeGraphic_clicked();
		void on_bIncreaseTransparency_clicked();
		void on_bDecreaseTransparency_clicked();
		void on_bSwitchON_clicked();
		void on_bSwitchOFF_clicked();
		void on_bPlaySoundEffect_clicked();
		void on_lwPattern_itemSelectionChanged();
	private:
		QString getPatternViewableText(MapEvent::MovementPattern *pattern);
		void addNewPattern(int type);
		QList<MapEvent::MovementPattern*> patterns;
		static QStringList patternNames;
};

#endif // CONFIGUREMAPEVENTMOVEMENTPATTERN_H
