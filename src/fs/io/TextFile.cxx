/*
 * Copyright 2003-2017 The Music Player Daemon Project
 * http://www.musicpd.org
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#include "config.h"
#include "TextFile.hxx"
#include "FileReader.hxx"
#include "AutoGunzipReader.hxx"
#include "BufferedReader.hxx"
#include "fs/Path.hxx"

#include <assert.h>

TextFile::TextFile(Path path_fs)
	:file_reader(new FileReader(path_fs)),
#ifdef ENABLE_ZLIB
	 gunzip_reader(new AutoGunzipReader(*file_reader)),
#endif
	 buffered_reader(new BufferedReader(*
#ifdef ENABLE_ZLIB
					    gunzip_reader
#else
					    file_reader
#endif
					    ))
{
}

TextFile::~TextFile()
{
	delete buffered_reader;
#ifdef ENABLE_ZLIB
	delete gunzip_reader;
#endif
	delete file_reader;
}

char *
TextFile::ReadLine()
{
	assert(buffered_reader != nullptr);

	return buffered_reader->ReadLine();
}
