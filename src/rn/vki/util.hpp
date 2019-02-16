#pragma once

#include <cstdint>

#include "../types.hpp"

namespace rn::vki::util {

constexpr size_t sizeOf(rn::DataFormat format) {
	switch (format) {
		default:
		case rn::DataFormat::Undefined: return 0;
		case rn::DataFormat::R8SInt: return 1 * 1;
		case rn::DataFormat::R8SNorm: return 1 * 1;
		case rn::DataFormat::R8UInt: return 1 * 1;
		case rn::DataFormat::R8UNorm: return 1 * 1;
		case rn::DataFormat::R8G8SInt: return 1 * 2;
		case rn::DataFormat::R8G8SNorm: return 1 * 2;
		case rn::DataFormat::R8G8UInt: return 1 * 2;
		case rn::DataFormat::R8G8UNorm: return 1 * 2;
		case rn::DataFormat::R8G8B8A8SInt: return 1 * 4;
		case rn::DataFormat::R8G8B8A8SNorm: return 1 * 4;
		case rn::DataFormat::R8G8B8A8UInt: return 1 * 4;
		case rn::DataFormat::R8G8B8A8UNorm: return 1 * 4;
		case rn::DataFormat::R16Float: return 2 * 1;
		case rn::DataFormat::R16SInt: return 2 * 1;
		case rn::DataFormat::R16SNorm: return 2 * 1;
		case rn::DataFormat::R16UInt: return 2 * 1;
		case rn::DataFormat::R16UNorm: return 2 * 1;
		case rn::DataFormat::R16G16Float: return 2 * 2;
		case rn::DataFormat::R16G16SInt: return 2 * 2;
		case rn::DataFormat::R16G16SNorm: return 2 * 2;
		case rn::DataFormat::R16G16UInt: return 2 * 2;
		case rn::DataFormat::R16G16UNorm: return 2 * 2;
		case rn::DataFormat::R16G16B16A16Float: return 2 * 4;
		case rn::DataFormat::R16G16B16A16SInt: return 2 * 4;
		case rn::DataFormat::R16G16B16A16SNorm: return 2 * 4;
		case rn::DataFormat::R16G16B16A16UInt: return 2 * 4;
		case rn::DataFormat::R16G16B16A16UNorm: return 2 * 4;
		case rn::DataFormat::R32Float: return 4 * 1;
		case rn::DataFormat::R32SInt: return 4 * 1;
		case rn::DataFormat::R32UInt: return 4 * 1;
		case rn::DataFormat::R32G32Float: return 4 * 2;
		case rn::DataFormat::R32G32SInt: return 4 * 2;
		case rn::DataFormat::R32G32UInt: return 4 * 2;
		case rn::DataFormat::R32G32B32Float: return 4 * 3;
		case rn::DataFormat::R32G32B32SInt: return 4 * 3;
		case rn::DataFormat::R32G32B32UInt: return 4 * 3;
		case rn::DataFormat::R32G32B32A32Float: return 4 * 4;
		case rn::DataFormat::R32G32B32A32SInt: return 4 * 4;
		case rn::DataFormat::R32G32B32A32UInt: return 4 * 4;
	}
}

}
