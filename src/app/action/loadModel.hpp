#pragma once

#include <rn/db/modelCollection.hpp>

#include "../rni.hpp"

namespace app::action {

struct LoadModel {
	rn::db::ModelCollection<app::rni::Context>::Handle modelHandle;
};

} // app::action
