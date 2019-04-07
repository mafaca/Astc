// AstcSource.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#define _CRT_SECURE_NO_WARNINGS // yes MSVC, I want to use fopen
#include "astc.h"

#define SOKOL_IMPL
#include <vector>
#include <stdio.h>
#include <string>
#include <chrono>
#include <iostream>

typedef std::vector<uint8_t> ByteArray;

static void ReadFile(const char* fileName, ByteArray& output)
{
	FILE* f = fopen(fileName, "rb");
	if (f)
	{
		fseek(f, 0, SEEK_END);
		size_t size = ftell(f);
		fseek(f, 0, SEEK_SET);
		size_t pos = output.size();
		output.resize(pos + size);
		fread(output.data() + pos, size, 1, f);
		fclose(f);
	}
}

static void WriteFile(const char* fileName, ByteArray& output)
{
	FILE* f = fopen(fileName, "wb");
	if (f)
	{
		fwrite(output.data(), output.size(), 1, f);
		fclose(f);
	}
}

int main(int argc, char *argv[])
{
	if (argc < 6)
	{
		std::cout << "Format: {fileName} {width} {height} {block_x_size} {block_y_size}" << std::endl;
		std::cin.get();
		return 0;
	}

	ByteArray input;
	char appDirectory[256];
	_splitpath(argv[0], nullptr, appDirectory, nullptr, nullptr);
	char* filePath = argv[1];
	char fileDirectory[256];
	char fileName[256];
	_splitpath(filePath, nullptr, fileDirectory, fileName, nullptr);
	int width = std::stoi(argv[2]);
	int height = std::stoi(argv[3]);
	int blockXSize = std::stoi(argv[4]);
	int blockYSize = std::stoi(argv[5]);
	std::string appDirStr = std::string(appDirectory);
	std::string inPath = fileDirectory[0] == 0 ? appDirStr + filePath : filePath;
	ReadFile(inPath.c_str(), input);

	ByteArray output;
	output.resize(width * height * 4);
	auto start = std::chrono::high_resolution_clock::now();
	decode_astc(input.data(), width, height, blockXSize, blockYSize, (uint32_t*)output.data());
	auto finish = std::chrono::high_resolution_clock::now();
	auto microseconds = std::chrono::duration_cast<std::chrono::milliseconds>(finish - start);
	std::cout << microseconds.count() << "ms\n";

	std::string outPath = appDirStr + fileName + ".data";
	WriteFile(outPath.c_str(), output);
}
