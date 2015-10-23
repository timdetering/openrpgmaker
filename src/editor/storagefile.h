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

#ifndef STORAGEFILE_H
#define STORAGEFILE_H

#ifdef DEBUG_STORAGE
#include <QStringList>
#endif

#include <cstdio>
#include <cstdlib>
#include <cstring>

class StorageFile
{
	public:
		friend class ProjectData;
		StorageFile();
		~StorageFile();
		char *getError();
		bool isOpen();
		bool isEOF();
		void putString(char *string);
		char *getString();
		void putLongDouble(long double value);
		long double getLongDouble();
		void putDouble(double value);
		double getDouble();
		void putFloat(float value);
		float getFloat();
		void putUnsignedLong(unsigned long value);
		unsigned long getUnsignedLong();
		void putSignedLong(signed long value);
		signed long getSignedLong();
		void putUnsignedInt(unsigned int value);
		unsigned int getUnsignedInt();
		void putSignedInt(signed int value);
		signed int getSignedInt();
		void putUnsignedShort(unsigned short value);
		unsigned short getUnsignedShort();
		void putSignedShort(signed short value);
		signed short getSignedShort();
		void putUnsignedChar(unsigned char value);
		unsigned char getUnsignedChar();
		void putSignedChar(signed char value);
		signed char getSignedChar();
		void putBool(bool value);
		bool getBool();
		void putRawData(char *buffer, size_t size);
		char *getRawData(size_t size);
		void putNewLine();
		void skipChar();
		enum {MODE_READ, MODE_WRITE};
	private:
		void openFile(const char *filelocation, int mode);
		void closeFile();
		FILE *file;
		FILE *debugFile;
		char errorMessage[256];
		#ifdef DEBUG_STORAGE
		QStringList writeOperations;
		QString storageFileLocation;
		int currentOperation;
		void checkReadOperation(QString readOperation);
		#endif
};

#endif // STORAGEFILE_H
