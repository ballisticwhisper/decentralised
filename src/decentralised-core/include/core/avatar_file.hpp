#ifndef DC_CORE_AVATAR_FILE_HPP
#define DC_CORE_AVATAR_FILE_HPP

#include "core/address.hpp"
#include "core/types.hpp"
#include <core/utility/elliptic_curve_key.hpp>
#include <iostream>
#include <fstream>
#include <filesystem>
#include <boost/filesystem.hpp>

using namespace boost::filesystem;

namespace decentralised
{
	namespace core
	{
		class avatar_file
		{
		public:
			avatar_file();
			~avatar_file();

			void create(std::string path, std::wstring firstName, std::wstring lastName, elliptic_curve_key &keyPair);
			std::vector<avatar_file> loadAll(std::string avatarFolderPath);

			void setVersion(unsigned long version);
			unsigned long getVersion();

			void setFirstName(std::wstring firstName);
			std::wstring getFirstName();

			void setLastName(std::wstring lastName);
			std::wstring getLastName();

			void setKeyPair(secret_parameter secret);
			elliptic_curve_key getKeyPair();

			//std::wstring getFullName();

		private:
			std::string path_;

			DWORD version_;
			std::wstring firstName_;
			std::wstring lastName_;
			elliptic_curve_key keyPair_;
		};
	}
}

#endif