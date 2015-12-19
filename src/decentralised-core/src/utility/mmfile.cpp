#include <core/utility/mmfile.hpp>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#ifndef _MSC_VER
#include <unistd.h>
#include <sys/mman.h>
#endif
#include <sys/types.h>
#include <sys/stat.h>
#include <core/utility/assert.hpp>

namespace decentralised
{
	namespace core
	{
		mmfile::mmfile(const std::string& filename)
		{
#ifndef _MSC_VER
			// Not yet MSVC portable (maybe windows).
			BITCOIN_ASSERT_MSG(sizeof (void*) == 8, "Not a 64bit system!");
			file_handle_ = open(filename.c_str(),
				O_RDWR, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
			if (file_handle_ == -1)
				return;
			struct stat sbuf;
			if (fstat(file_handle_, &sbuf) == -1)
				return;
			size_ = (long long)sbuf.st_size;
			// You can static_cast void* pointers.
			data_ = static_cast<uint8_t*>(mmap(
				0, size_, PROT_READ | PROT_WRITE, MAP_SHARED, file_handle_, 0));
			if (data_ == MAP_FAILED)
				data_ = nullptr;
			// mmap opens its own file handle.
			close(file_handle_);
#endif
		}
		mmfile::~mmfile()
		{
		}

		uint8_t* mmfile::data()
		{
			return data_;
		}
		const uint8_t* mmfile::data() const
		{
			return data_;
		}
		size_t mmfile::size() const
		{
			return size_;
		}

	}
}