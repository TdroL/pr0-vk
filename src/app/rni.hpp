#pragma once

#if defined(APP_RNI_USE_NULL)

#include <rn/nulli/context.hpp>
// #include <rn/nulli/resources.hpp>

namespace app {

namespace rni = rn::nulli;

}

#else

#include <rn/vki/context.hpp>
// #include <rn/vki/resources.hpp>

namespace app {

namespace rni = rn::vki;

}

#endif