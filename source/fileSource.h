/*  YUView - YUV player with advanced analytics toolset
*   Copyright (C) 2015  Institut für Nachrichtentechnik
*                       RWTH Aachen University, GERMANY
*
*   YUView is free software; you can redistribute it and/or modify
*   it under the terms of the GNU General Public License as published by
*   the Free Software Foundation; either version 2 of the License, or
*   (at your option) any later version.
*
*   YUView is distributed in the hope that it will be useful,
*   but WITHOUT ANY WARRANTY; without even the implied warranty of
*   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
*   GNU General Public License for more details.
*
*   You should have received a copy of the GNU General Public License
*   along with YUView.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef FILESOURCE_H
#define FILESOURCE_H

#include <QString>
#include <QFile>
#include <QFileInfo>
#include <QTextStream>
#include <QFuture>
#include "typedef.h"

/* The fileSource class provides functions for accessing files. Besides the reading of
 * certain blocks of the file, it also directly provides information on the file for the
 * fileInfoWidget. It also adds functions for guessing the format from the filename.
 */
class fileSource
{
public:
  fileSource();
  ~fileSource();

  void openFile(QString filePath);

  // Return information on this file (like path, date created file Size ...)
  virtual QList<infoItem> getFileInfoList();

  QString absoluteFilePath() { return srcFile ? fileInfo.absoluteFilePath() : ""; }

  // Return true if the file could be opened and is ready for use.
  bool isOk() { return srcFile != NULL; }

  QFile *getQFile() { return srcFile; }

  // Pass on to srcFile
  bool atEnd() { return (srcFile == NULL) ? true : srcFile->atEnd(); }
  QByteArray readLine() { return (srcFile == NULL) ? QByteArray() : srcFile->readLine(); }
  bool seek(qint64 pos) { return (srcFile == NULL) ? false : srcFile->seek(pos); }

  // Guess the format (width, height, frameTate...) from the file name.
  // Certain patterns are recognized. E.g: "something_352x288_24.yuv"
  void formatFromFilename(int &width, int &height, int &frameRate, int &bitDepth, int &subFormat);

  // Get the file size in bytes
  qint64 getFileSize() { return (srcFile == NULL) ? -1 : fileInfo.size(); }

  // Read the given number of bytes starting at startPos into the QByteArray out
  // Resize the QByteArray if necessary
  void readBytes(QByteArray &targetBuffer, qint64 startPos, qint64 nrBytes);

  QString getAbsoluteFilePath() { return fileInfo.absoluteFilePath(); }

protected:

private:
  // Info on the source file. 
  QFileInfo fileInfo;

  // The pointer to the QFile to open. If opening failed, this will be NULL;
  QFile *srcFile;

  // ---- Caching ---- 
  // A byte array that holds the result of the (probably next) read operation
  QByteArray cacheBuffer;
  qint64 cacheStartPos;
  qint64 cacheNrBytes;
  QFuture<void> backgroundReaderFuture;
  void backgroundCaching();
};

#endif