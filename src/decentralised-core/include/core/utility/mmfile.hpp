#ifndef DC_CORE_MMFILE_HPP
#define DC_CORE_MMFILE_HPP

#include <cstddef>
#include <cstdint>
#include <string>

namespace decentralised
{
	namespace core
	{
		class mmfile
		{
		public:
			mmfile(const std::string& filename);
			~mmfile();
			uint8_t* data();
			const uint8_t* data() const;
			size_t size() const;
		private:
			int file_handle_ = 0;
			uint8_t* data_ = nullptr;
			size_t size_;
		};

	}
}
#endif
