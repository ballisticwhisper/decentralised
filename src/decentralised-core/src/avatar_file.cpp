#include <core/avatar_file.hpp>

namespace decentralised
{
	namespace core
	{
		avatar_file::avatar_file()
		{
		}

		avatar_file::~avatar_file()
		{

		}

		void avatar_file::setVersion(unsigned long version)
		{
			version_ = version;
		}

		unsigned long avatar_file::getVersion()
		{
			return version_;
		}

		void avatar_file::setFirstName(std::wstring firstName)
		{
			firstName_ = firstName;
		}

		std::wstring avatar_file::getFirstName()
		{
			return firstName_;
		}

		void avatar_file::setLastName(std::wstring lastName)
		{
			lastName_ = lastName;
		}

		std::wstring avatar_file::getLastName()
		{
			return lastName_;
		}

		void avatar_file::setKeyPair(secret_parameter secret)
		{
			keyPair_.set_secret(secret);
		}

		elliptic_curve_key avatar_file::getKeyPair()
		{
			return keyPair_;
		}

		void avatar_file::create(std::string path, std::wstring firstName, std::wstring lastName, elliptic_curve_key &keyPair)
		{
			path_ = path;
			firstName_ = firstName;
			lastName_ = lastName;
			keyPair_ = elliptic_curve_key(keyPair);

			std::ofstream file;
			file.open(path_.c_str(), std::ios::out | std::ios::app | std::ios::binary);

			// Write avatar file version. v1
			version_ = 1;
			file.write((char*)&version_, sizeof(DWORD));

			// Write length of avatar firstname, and then the firstname in unicode format.
			DWORD firstNameLength = firstName_.size() * sizeof(wchar_t);
			file.write((char*)&firstNameLength, sizeof(DWORD));
			file.write((char*)firstName_.c_str(), firstNameLength);

			// Write length of avatar lastname, and then the lastname in unicode format.
			DWORD lastNameLength = lastName_.size() * sizeof(wchar_t);
			file.write((char*)&lastNameLength, sizeof(DWORD));
			file.write((char*)lastName_.c_str(), lastNameLength);

			// Write length of private key, and then the private key itself.
			secret_parameter raw_private_key = keyPair_.secret();

			DWORD privateKeyLength = raw_private_key.size();
			file.write((char*)&privateKeyLength, sizeof(DWORD));
			file.write((char*)raw_private_key.data(), privateKeyLength);

			file.close();
		}

		std::vector<avatar_file> avatar_file::loadAll(std::string avatarFolderPath)
		{
			std::vector<avatar_file> results;

			if (is_directory(avatarFolderPath)) {
				for (directory_iterator itr(avatarFolderPath.c_str()); itr != directory_iterator(); ++itr)
				{
					std::string path = itr->path().string();
					printf(std::string("Loading avatar file: ").append(path).append("\n").c_str());

					std::ifstream file;
					file.open(path.c_str(), std::ios::binary | std::ios::in);

					avatar_file avatar;

					// Read avatar file version
					DWORD version = -1;
					file.read((char*)&version, 4); 
					avatar.setVersion(version);

					// Read the avatar first name
					DWORD firstNameLength = -1;
					std::wstring firstName;

					file.read((char*)&firstNameLength, 4);
					wchar_t* firstNameBinary = new wchar_t[firstNameLength];
					file.read((char*)firstNameBinary, firstNameLength);
					firstName.assign(firstNameBinary, firstNameLength / 2);
					delete firstNameBinary;
					avatar.setFirstName(firstName);

					// Read the avatar last name
					DWORD lastNameLength = -1;
					std::wstring lastName;

					file.read((char*)&lastNameLength, 4);
					wchar_t* lastNameBinary = new wchar_t[lastNameLength];
					file.read((char*)lastNameBinary, lastNameLength);
					lastName.assign(lastNameBinary, lastNameLength / 2);
					delete lastNameBinary;
					avatar.setLastName(lastName);

					// Read the avatar key pair
					DWORD keyPairLength = -1;
					secret_parameter raw_private_key;

					file.read((char*)&keyPairLength, 4);
					file.read((char*)&raw_private_key, keyPairLength);

					avatar.setKeyPair(raw_private_key);

					file.close();

					results.push_back(avatar);
				}
			}

			return results;
		}
	}
}