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

#include <QDir>
#include <QRect>
#include <QSplashScreen>
#include <QDesktopWidget>
#include <QBitmap>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include "audiomanager.h"
#include "clipboard.h"
#include "helpmanual.h"
#include "mainwindow.h"
#include "projectdata.h"
#include "xmlParser.h"

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);
	QPixmap pixmap = QPixmap(":/images/splashScreen.png");
	QSplashScreen splashScreen(pixmap);
	QDir homeDir = QDir::home();
	int returnValue;
	splashScreen.setMask(pixmap.mask());
	splashScreen.show();
	app.processEvents();
	if (!homeDir.exists(".OpenRPGMaker"))
		homeDir.mkdir(".OpenRPGMaker");
	homeDir.cd(".OpenRPGMaker");
	homeDir.mkdir("Projects");
	app.processEvents();
	ProjectData::setupProjectData();
	app.processEvents();
	sscanf(VERSION, "%d.%d.%d", &ProjectData::majorVersion, &ProjectData::minorVersion, &ProjectData::develVersion);
	printf("Open RPG Maker v%d.%d.%d\n", ProjectData::majorVersion, ProjectData::minorVersion, ProjectData::develVersion);
	printf("Using file format version: %d\n", FILE_FORMAT_VERSION);
	ProjectData::mainwindow = new MainWindow;
	app.processEvents();
	ProjectData::mainwindow->setupWindow(argc, argv);
	app.processEvents();
	ProjectData::mainwindow->show();
	app.processEvents();
	srand(ProjectData::randomSeed);
	splashScreen.finish(ProjectData::mainwindow);
	returnValue = app.exec();
	return returnValue;
}
