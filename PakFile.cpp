#include "BinaryReader.h"
#include "PakFile.h"
#include "Crypto.h"
#include <windows.h>


namespace Pakfile
{
	void OutDict(const std::string& pakPath)
	{
	
		auto Buf = ReadFile(pakPath);
		BinaryReader br(Buf);

		Header header{};

		header.KEY = br.Read<uint8_t>();


		if (header.KEY != 0xC0 && header.KEY != 0x00) //Prevent unpacking the file if the key is not the default one or
			//if the file has been purposely decrypted
		{
			std::cout << "This file has been purposely encrypted by the developer, the operation will abort.";
			
			std::exit(5);
		}
		header.FILE_COUNT = br.Read<uint32_t>();


		for (uint32_t i = 0; i < header.FILE_COUNT; ++i)
		{
			FileEntry entry{};

			entry.FNAME_LEN = br.Read<uint32_t>();
			entry.OFFSET = br.Read<uint64_t>();
			entry.SIZE = br.Read<uint64_t>();

			auto eFNAME_RAW = br.ReadBytes(entry.FNAME_LEN);
			auto dFNAME_RAW = Xor(eFNAME_RAW, header.KEY);

			entry.FNAME = std::string(dFNAME_RAW.begin(),dFNAME_RAW.end());
			std::cout << entry.FNAME << std::endl;

		}
		std::cout<<std::endl;
		std::cout << "Files: " << header.FILE_COUNT << std::endl;
	}


	void Unpack(const std::string& pakPath, const std::string& outPath)
	{
		bool err = false;//just4 fun
		auto Buf = ReadFile(pakPath);
		BinaryReader br(Buf);

		Header header{};

		header.KEY = br.Read<uint8_t>();

		if (header.KEY!= 0xC0 && header.KEY != 0x00) //Prevent unpacking the file if the key is not the default one or
			                   //if the file has been purposely decrypted
		{
			std::cout << "This file has been purposely encrypted by the developer, the operation will abort.";
			err = true; //just4 fun
			std::exit(5);
		}

		header.FILE_COUNT = br.Read<uint32_t>();

		//if the user got to this condition, it means they used a debugger to step over or patched the exit call, congratulate them!
		if (err == true)
		{

			MessageBoxA(nullptr, "Good Job! You've earned it!", "Winner Winner Chicken Dinner!", MB_OK);
		}

		for (uint32_t i = 0; i < header.FILE_COUNT; ++i)
		{
			FileEntry entry{};

			std::vector<uint8_t> data;

			entry.FNAME_LEN = br.Read<uint32_t>();
			entry.OFFSET = br.Read<uint64_t>();
			entry.SIZE = br.Read<uint64_t>();

			auto eFNAME_RAW = br.ReadBytes(entry.FNAME_LEN);
			auto dFNAME_RAW = Xor(eFNAME_RAW, header.KEY);

			entry.FNAME = std::string(dFNAME_RAW.begin(), dFNAME_RAW.end());
			auto here = br.Tell(); //dont really wnat to bother with storing the dict in a list...

			br.Seek(entry.OFFSET);

			//watermark the scripts
			if (entry.FNAME.ends_with(".script"))
			{
				std::string watermark = "//SCRIPT DECOMPILED USING AEXADEV PAK TOOL https://github.com/ExIfDev/Easy-FPS-Decompiler";
				std::vector<uint8_t> wtbytes;
				wtbytes.assign(watermark.begin(), watermark.end());
				std::vector<uint8_t> newline = { 0x0D, 0x0A };
				ExtendBuf(wtbytes,newline);//add the newline bytes
				ExtendBuf(data, wtbytes);	
			}
			
			ExtendBuf(data, Xor(br.ReadBytes(entry.SIZE), header.KEY));

			WriteFile(outPath, entry.FNAME, data);

			br.Seek(here);

			std::cout << "Extracted: " << entry.FNAME << std::endl;
		}
	}


	void Protect(const std::string& pakPath, const std::string& outPath)
	{
		auto Buf = ReadFile(pakPath);
		BinaryReader br(Buf);

		uint8_t OG_KEY = br.Read<uint8_t>();
		if (OG_KEY != 0xC0 && OG_KEY != 0x00)
		{
			std::cout << "This file has been purposely encrypted by the developer, the operation will abort.";
			std::exit(5);
		}

		uint32_t FILE_COUNT = br.Read<uint32_t>();

		std::vector<uint8_t> outBuf;//destination file

		std::srand(static_cast<unsigned>(std::time(nullptr)));//new seed
		uint8_t NEW_KEY = std::rand() % 256;

		while (NEW_KEY == 0xC0 || NEW_KEY == 0x00)
		{
			NEW_KEY = std::rand() % 256;
			std::cout << "Regen triggered!" << std::endl;
		}

		WriteBuf(outBuf, NEW_KEY);
		WriteBuf(outBuf, FILE_COUNT);


		for (uint32_t i = 0; i < FILE_COUNT; ++i)
		{
			//read and decrypt an original entry
			uint32_t FNAME_LEN = br.Read<uint32_t>();
			uint64_t OFFSET = br.Read<uint64_t>();
			uint64_t SIZE = br.Read<uint64_t>();

			auto eFNAME_RAW_og = br.ReadBytes(FNAME_LEN);
			auto dFNAME_RAW_og = Xor(eFNAME_RAW_og, OG_KEY);

			//re write the entry to the out buffer 

			WriteBuf(outBuf, FNAME_LEN);
			WriteBuf(outBuf, OFFSET);
			WriteBuf(outBuf, SIZE);

			ExtendBuf(outBuf, Xor(dFNAME_RAW_og, NEW_KEY));

			std::cout << "Encrypting entry: " << std::string(dFNAME_RAW_og.begin(), dFNAME_RAW_og.end()) << std::endl;

		}
		std::cout << "Encrypting asset payload..."<< std::endl;

		//calulate the size of the payload buffer
		auto siz = Buf.size() - br.Tell();
		
		auto e_ogPl = br.ReadBytes(siz); //read og payload encrypted
		auto ogPl = Xor(e_ogPl, OG_KEY); //dexor og payload

		//re xor with the new key and add to output buffer
		ExtendBuf(outBuf, Xor(ogPl, NEW_KEY));
		WriteFile(outPath, "Data.pak", outBuf);
		std::cout << "Operation completed!! The new .pak file has been saved to " << outPath<<std::endl;
		
	}
}



//Helpers
void WriteFile(const std::string& outputDir,
	const std::string& fName,
	const std::vector<uint8_t>& data)
{
	namespace fs = std::filesystem;

	fs::path fPath = fs::path(outputDir) / fName;

	fs::create_directories(fPath.parent_path());

	std::ofstream out(fPath, std::ios::binary);
	if (!out)
		throw std::runtime_error("Failed to open output file: " + fPath.string());

	out.write(reinterpret_cast<const char*>(data.data()),
		static_cast<std::streamsize>(data.size()));

	if (!out)
		throw std::runtime_error("Failed to write output file: " + fPath.string());
}



//all from here is kinda shitty but does the job
template<typename T>
void WriteBuf(std::vector<uint8_t>& buf, T value)
{
	for (size_t i = 0; i < sizeof(T); ++i)
		buf.push_back((uint8_t)((value >> (8 * i)) & 0xFF));
}

void ExtendBuf(std::vector<uint8_t>& dest,const std::vector<uint8_t>& src)
{
	dest.insert(dest.end(), src.begin(), src.end());
}


