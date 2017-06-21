#pragma once

namespace Skuld
{
#pragma pack(push, 1)

	struct float2
	{
		float x;
		float y;
		float2() : float2(0.0f, 0.0f) {};
		float2(float x, float y) : x(x), y(y) {}
	};

	struct float3
	{
		float x;
		float y;
		float z;
		float3() : float3(0.0f, 0.0f, 0.0f) {}
		float3(float x, float y, float z) : x(x), y(y), z(z) {}
	};

	struct float4
	{
		float x;
		float y;
		float z;
		float w;
		float4() : float4(0.0f, 0.0f, 0.0f, 0.0f) {}
		float4(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {}
	};

	struct int2
	{
		int x;
		int y;
		int2() : int2(0, 0) {}
		int2(int x, int y) : x(x), y(y) {}
	};

	struct int3
	{
		int x;
		int y;
		int z;
		int3() : int3(0, 0, 0) {}
		int3(int x, int y, int z) : x(x), y(y), z(z) {}
	};

	struct int4
	{
		int x;
		int y;
		int z;
		int w;
		int4() : int4(0, 0, 0, 0) {}
		int4(int x, int y, int z, int w) : x(x), y(y), z(z), w(w) {}
	};

	typedef unsigned int uint;

	struct uint2
	{
		uint x;
		uint y;
		uint2() : uint2(0, 0) {}
		uint2(uint x, uint y) : x(x), y(y) {}
	};

	struct uint3
	{
		uint x;
		uint y;
		uint z;
		uint3() : uint3(0, 0, 0) {}
		uint3(uint x, uint y, uint z) : x(x), y(y), z(z) {}
	};

	struct uint4
	{
		uint x;
		uint y;
		uint z;
		uint w;
		uint4() : uint4(0, 0, 0, 0) {}
		uint4(uint x, uint y, uint z, uint w) : x(x), y(y), z(z), w(w) {}
	};
#pragma pack(pop)
}