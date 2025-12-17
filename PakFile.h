#pragma once

#include <iostream>
#include <filesystem>
#include <fstream>
#include <vector>
#include <string>
#include <stdexcept>
namespace Pakfile
{
	struct Header
	{
		uint8_t KEY;
		uint32_t FILE_COUNT;
	};

	struct FileEntry
	{
		uint32_t FNAME_LEN;
		uint64_t OFFSET;
		uint64_t SIZE;
		std::string FNAME;
	};

	void OutDict(const std::string& pakPath);
	void Unpack(const std::string& pakPath, const std::string& outPath);
	void Protect(const std::string& pakPath, const std::string& outPath);




}

void WriteFile(const std::string& outputDir,
	const std::string& filename,
	const std::vector<uint8_t>& data);
template<typename T>
void WriteBuf(std::vector<uint8_t>& dest, T value);
void ExtendBuf(std::vector<uint8_t>& dest, const std::vector<uint8_t>& src);

