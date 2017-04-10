#ifndef CFILE_HPP_
#define CFILE_HPP_

#include <stdio.h>
#include <string>
#include <string.h>
#include <fcntl.h>
#include <gctypes.h>
#include <unistd.h>

class CFile
{
	public:
		enum eOpenTypes
		{
		    ReadOnly,
		    WriteOnly,
		    ReadWrite,
		    Append
		};

		CFile();
		CFile(const std::string & filepath, eOpenTypes mode);
		CFile(const u8 * memory, s32 memsize);
		virtual ~CFile();

		s32 open(const std::string & filepath, eOpenTypes mode);
		s32 open(const u8 * memory, s32 memsize);

		bool isOpen() const {
            if(iFd >= 0)
                return true;

            if(mem_file)
                return true;

            return false;
		}

		void close();

		s32 read(u8 * ptr, size_t size);
		s32 write(const u8 * ptr, size_t size);
		s32 fwrite(const char *format, ...);
		s32 seek(long int offset, s32 origin);
		u64 tell() { return pos; };
		u64 size() { return filesize; };
		void rewind() { this->seek(0, SEEK_SET); };

	protected:
		s32 iFd;
		const u8 * mem_file;
		u64 filesize;
		u64 pos;
};

#endif
