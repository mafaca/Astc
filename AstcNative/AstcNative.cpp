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

int main()
{
	const int Width = 512;
	const int Height = 512;
	const int BlockSize = 5;

	ByteArray input;
	ReadFile(std::string("test.astc").c_str(), input);

	ByteArray output;
	output.resize(Width * Height * 4);
	auto start = std::chrono::high_resolution_clock::now();
	decode_astc(input.data(), Width, Height, BlockSize, BlockSize, (uint32_t*)output.data());
	auto finish = std::chrono::high_resolution_clock::now();
	auto microseconds = std::chrono::duration_cast<std::chrono::milliseconds>(finish - start);
	std::cout << microseconds.count() << "ms\n";

	WriteFile("test.data", output);
}
