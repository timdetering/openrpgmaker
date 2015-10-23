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
 *  Copyright (C) 2013, Justin Davis <tuxdavis@gmail.com>                   *
 ****************************************************************************/

#ifndef PROCESSLOGDIALOG_H
#define PROCESSLOGDIALOG_H

#include <cstdio>
#include "ui_processlogdialog.h"

class ProcessLogDialog : public QDialog, public Ui::ProcessLogDialog
{
	Q_OBJECT
	public:
		ProcessLogDialog(QString fileLocation, QWidget *parent=0);
		~ProcessLogDialog();
		void setCurrentTask(QString currentTask);
		void addLogMessage(QString message);
		void clearLog();
		void done();
	private:
		void closeEvent(QCloseEvent *event);
		QScrollBar *scrollBar;
		FILE *file;
		bool allowClose;
};

#endif // PROCESSLOGDIALOG_H
