/****************************************************************************
*  Open RPG MAker is free software: you can redistribute it and/or modify  *
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

#ifndef IMPORTERPLUGIN_H
#define IMPORTERPLUGIN_H

#include <QImage>
#include <QColor>
#include <QRect>
#include <QPoint>
#include "importerplugininterface.h"

class ImporterPlugin : public QObject, ImporterPluginInterface
{
	Q_OBJECT
	Q_INTERFACES(ImporterPluginInterface);
	public:
		virtual ~ImporterPlugin() {}
		void initializeImporter(QString homePath);
		void shutdownImporter();
		virtual void runImporter() = 0;
		virtual QString getImporterName() = 0;
		virtual QString getImporterDescription() = 0;
	protected:
		void drawImage(QImage src, QRect srcRect, QImage &dst, QPoint dstPoint, unsigned int src_tcolor);
		bool isImageEmpty(QImage image);
		void copyDirectory(QString dirLocation, QString newLocation);
		void copyFile(QString fileLocation, QString newLocation);
};

#endif // IMPORTERPLUGIN_H
