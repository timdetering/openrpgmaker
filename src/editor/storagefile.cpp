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

#ifdef DEBUG_STORAGE
#include <QStringList>
#include <QFile>
#include <QTextStream>
#endif

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cerrno>
#include <cassert>
#include "storagefile.h"

StorageFile::StorageFile()
{
	file = NULL;
	debugFile = NULL;
	sprintf(errorMessage, "No error.");
}

StorageFile::~StorageFile()
{
	if (file != NULL)
	{
		fclose(file);
		file = NULL;
	}
	if (debugFile != NULL)
	{
		fclose(debugFile);
		debugFile = NULL;
	}
}

char *StorageFile::getError()
{
	return &errorMessage[0];
}

void StorageFile::openFile(const char *filelocation, int mode)
{
	file = fopen(filelocation, (mode == MODE_READ) ? "rb":"wb");
	if (file == NULL)
	{
		printf("StorageFile Error: %s\n", strerror(errno));
		printf("  File: \"%s\"\n", filelocation);
		sprintf(errorMessage, "%s", strerror(errno));
	}
	#ifdef DEBUG_STORAGE
	char tempString[512];
	storageFileLocation = filelocation;
	if (mode == MODE_READ)
	{
		QFile writeFile(QString("%1.write").arg(filelocation));
		QTextStream stream;
		writeFile.open(QIODevice::ReadOnly);
		stream.setDevice(&writeFile);
		while (!stream.atEnd())
			writeOperations.append(stream.readLine());
		writeFile.close();
	}
	sprintf(tempString, "%s.%s", filelocation, (mode == MODE_READ) ? "read":"write");
	debugFile = fopen(tempString, "w");
	currentOperation = 0;
	#endif
}

void StorageFile::closeFile()
{
	if (file != NULL)
	{
		fclose(file);
		file = NULL;
	}
	if (debugFile != NULL)
	{
		fclose(debugFile);
		debugFile = NULL;
	}
}

bool StorageFile::isOpen()
{
	return (file != NULL);
}

bool StorageFile::isEOF()
{
	return (feof(file) != 0);
}

void StorageFile::putString(char *string)
{
	size_t size = strlen(string);
	size_t result;
	putUnsignedShort(size);
	result = fwrite(string, 1, size, file);
	assert(result == size);
	#ifdef DEBUG_STORAGE
	fprintf(debugFile, "string\n");
	#endif
}

char *StorageFile::getString()
{
	size_t size = getUnsignedShort();
	size_t result;
	char *string = new char[size+1];
	result = fread(string, 1, size, file);
	assert(result == size);
	string[size] = '\0';
	#ifdef DEBUG_STORAGE
	checkReadOperation("string");
	fprintf(debugFile, "string\n");
	#endif
	return string;
}

void StorageFile::putLongDouble(long double value)
{
	size_t result;
	result = fwrite(&value, sizeof(long double), 1, file);
	assert(result == 1);
	#ifdef DEBUG_STORAGE
	fprintf(debugFile, "long double\n");
	#endif
}

long double StorageFile::getLongDouble()
{
	long double value;
	size_t result;
	result = fread(&value, sizeof(long double), 1, file);
	assert(result == 1);
	#ifdef DEBUG_STORAGE
	checkReadOperation("long double");
	fprintf(debugFile, "long double\n");
	#endif
	return value;
}

void StorageFile::putDouble(double value)
{
	size_t result;
	result = fwrite(&value, sizeof(double), 1, file);
	assert(result == 1);
	#ifdef DEBUG_STORAGE
	fprintf(debugFile, "double\n");
	#endif
}

double StorageFile::getDouble()
{
	double value;
	size_t result;
	result = fread(&value, sizeof(double), 1, file);
	assert(result == 1);
	#ifdef DEBUG_STORAGE
	checkReadOperation("double");
	fprintf(debugFile, "double\n");
	#endif
	return value;
}

void StorageFile::putFloat(float value)
{
	size_t result;
	result = fwrite(&value, sizeof(float), 1, file);
	assert(result == 1);
	#ifdef DEBUG_STORAGE
	fprintf(debugFile, "float\n");
	#endif
}

float StorageFile::getFloat()
{
	float value;
	size_t result;
	result = fread(&value, sizeof(float), 1, file);
	assert(result == 1);
	#ifdef DEBUG_STORAGE
	checkReadOperation("float");
	fprintf(debugFile, "float\n");
	#endif
	return value;
}

void StorageFile::putUnsignedLong(unsigned long value)
{
	size_t result;
	result = fwrite(&value, sizeof(unsigned long), 1, file);
	assert(result == 1);
	#ifdef DEBUG_STORAGE
	fprintf(debugFile, "unsigned long\n");
	#endif
}

unsigned long StorageFile::getUnsignedLong()
{
	unsigned long value;
	size_t result;
	result = fread(&value, sizeof(unsigned long), 1, file);
	assert(result == 1);
	#ifdef DEBUG_STORAGE
	checkReadOperation("unsigned long");
	fprintf(debugFile, "unsigned long\n");
	#endif
	return value;
}

void StorageFile::putSignedLong(signed long value)
{
	size_t result;
	result = fwrite(&value, sizeof(signed long), 1, file);
	assert(result == 1);
	#ifdef DEBUG_STORAGE
	fprintf(debugFile, "signed long\n");
	#endif
}

signed long StorageFile::getSignedLong()
{
	signed long value;
	size_t result;
	result = fread(&value, sizeof(signed long), 1, file);
	assert(result == 1);
	#ifdef DEBUG_STORAGE
	checkReadOperation("signed long");
	fprintf(debugFile, "signed long\n");
	#endif
	return value;
}

void StorageFile::putUnsignedInt(unsigned int value)
{
	size_t result;
	result = fwrite(&value, sizeof(unsigned int), 1, file);
	assert(result == 1);
	#ifdef DEBUG_STORAGE
	fprintf(debugFile, "unsigned int\n");
	#endif
}

unsigned int StorageFile::getUnsignedInt()
{
	size_t result;
	unsigned int value;
	result = fread(&value, sizeof(unsigned int), 1, file);
	assert(result == 1);
	#ifdef DEBUG_STORAGE
	checkReadOperation("unsigned int");
	fprintf(debugFile, "unsigned int\n");
	#endif
	return value;
}

void StorageFile::putSignedInt(signed int value)
{
	size_t result;
	result = fwrite(&value, sizeof(signed int), 1, file);
	assert(result == 1);
	#ifdef DEBUG_STORAGE
	fprintf(debugFile, "signed int\n");
	#endif
}

signed int StorageFile::getSignedInt()
{
	size_t result;
	signed int value;
	result = fread(&value, sizeof(signed int), 1, file);
	assert(result == 1);
	#ifdef DEBUG_STORAGE
	checkReadOperation("signed int");
	fprintf(debugFile, "signed int\n");
	#endif
	return value;
}

void StorageFile::putUnsignedShort(unsigned short value)
{
	size_t result;
	result = fwrite(&value, sizeof(unsigned short), 1, file);
	assert(result == 1);
	#ifdef DEBUG_STORAGE
	fprintf(debugFile, "unsigned short\n");
	#endif
}

unsigned short StorageFile::getUnsignedShort()
{
	size_t result;
	unsigned short value;
	result = fread(&value, sizeof(unsigned short), 1, file);
	assert(result == 1);
	#ifdef DEBUG_STORAGE
	checkReadOperation("unsigned short");
	fprintf(debugFile, "unsigned short\n");
	#endif
	return value;
}

void StorageFile::putSignedShort(signed short value)
{
	size_t result;
	result = fwrite(&value, sizeof(signed short), 1, file);
	assert(result == 1);
	#ifdef DEBUG_STORAGE
	fprintf(debugFile, "signed short\n");
	#endif
}

signed short StorageFile::getSignedShort()
{
	size_t result;
	signed short value;
	result = fread(&value, sizeof(unsigned short), 1, file);
	assert(result == 1);
	#ifdef DEBUG_STORAGE
	checkReadOperation("signed short");
	fprintf(debugFile, "signed short\n");
	#endif
	return value;
}

void StorageFile::putUnsignedChar(unsigned char value)
{
	size_t result;
	result = fwrite(&value, sizeof(unsigned char), 1, file);
	assert(result == 1);
	#ifdef DEBUG_STORAGE
	fprintf(debugFile, "unsigned char\n");
	#endif
}

unsigned char StorageFile::getUnsignedChar()
{
	size_t result;
	unsigned char value;
	result = fread(&value, sizeof(unsigned char), 1, file);
	assert(result == 1);
	#ifdef DEBUG_STORAGE
	checkReadOperation("unsigned char");
	fprintf(debugFile, "unsigned char\n");
	#endif
	return value;
}

void StorageFile::putSignedChar(signed char value)
{
	size_t result;
	result = fwrite(&value, sizeof(signed char), 1, file);
	assert(result == 1);
	#ifdef DEBUG_STORAGE
	fprintf(debugFile, "signed char\n");
	#endif
}

signed char StorageFile::getSignedChar()
{
	size_t result;
	signed char value;
	result = fread(&value, sizeof(signed char), 1, file);
	assert(result == 1);
	#ifdef DEBUG_STORAGE
	checkReadOperation("signed char");
	fprintf(debugFile, "signed char\n");
	#endif
	return value;
}

void StorageFile::putBool(bool value)
{
	size_t result;
	result = fwrite(&value, sizeof(bool), 1, file);
	assert(result == 1);
	#ifdef DEBUG_STORAGE
	fprintf(debugFile, "bool\n");
	#endif
}

bool StorageFile::getBool()
{
	size_t result;
	bool value;
	result = fread(&value, sizeof(bool), 1, file);
	assert(result == 1);
	#ifdef DEBUG_STORAGE
	checkReadOperation("bool");
	fprintf(debugFile, "bool\n");
	#endif
	return value;
}

void StorageFile::putRawData(char *buffer, size_t size)
{
	size_t result;
	result = fwrite(buffer, 1, size, file);
	assert(result == size);
	#ifdef DEBUG_STORAGE
	fprintf(debugFile, "rawData\n");
	#endif
}

char *StorageFile::getRawData(size_t size)
{
	size_t result;
	char *buffer = new char[size];
	result = fread(buffer, 1, size, file);
	assert(result == size);
	#ifdef DEBUG_STORAGE
	checkReadOperation("rawData");
	fprintf(debugFile, "rawData\n");
	#endif
	return buffer;
}

void StorageFile::putNewLine()
{
	size_t result;
	char value = '\n';
	result = fwrite(&value, sizeof(char), 1, file);
	assert(result == 1);
	#ifdef DEBUG_STORAGE
	fprintf(debugFile, "newLine\n");
	#endif
}

void StorageFile::skipChar()
{
	size_t result;
	char null;
	result = fread(&null, sizeof(char), 1, file);
	assert(result == 1);
	#ifdef DEBUG_STORAGE
	fprintf(debugFile, "skipChar\n");
	#endif
}

#ifdef DEBUG_STORAGE
void StorageFile::checkReadOperation(QString readOperation)
{
	QString writeOperation = writeOperations.takeFirst();
	if (writeOperation != readOperation)
	{
		printf("StorageError: mismatched read/write operations (operation#%d).\n", currentOperation);
		printf("  File: \"%s\"\n", storageFileLocation.toUtf8().data());
		printf("  Read:  %s\n", readOperation.toUtf8().data());
		printf("  Write: %s\n", writeOperation.toUtf8().data());
		assert(writeOperation != readOperation);
	}
	++currentOperation;
}
#endif
