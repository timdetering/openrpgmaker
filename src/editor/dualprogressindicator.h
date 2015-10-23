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

#ifndef DUALPROGRESSINDICATOR_H
#define DUALPROGRESSINDICATOR_H

#include "ui_dualprogressindicator.h"

class DualProgressIndicator : public QDialog, protected Ui::DualProgressIndicator
{
	Q_OBJECT
	public:
		DualProgressIndicator(QString title, QWidget *parent=0);
		~DualProgressIndicator();
		void setOverallProgressBarRange(int min, int max);
		void setOverallProgressBarValue(int value);
		void setCurrentProcessProgressBarRange(int min, int max);
		void setCurrentProcessProgressBarValue(int value);
		void setCurrentProcessTitle(QString value);
		void doneLoading();
	protected slots:
		void on_buttonBox_rejected();
	signals:
		void processCanceled();
	private:
		void closeEvent(QCloseEvent *event);
		bool allowClose;
};

#endif // DUALPROGRESSINDICATOR_H
