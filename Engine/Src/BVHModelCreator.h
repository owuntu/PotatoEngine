#ifndef POTATOENGINE_BVH_MODEL_CREATOR_H_
#define POTATOENGINE_BVH_MODEL_CREATOR_H_

#include "ModelCreator.h"

namespace PotatoEngine
{
	class BVHModelCreator : public ModelCreator
	{
	protected:
		virtual std::shared_ptr<Model> DoCreateModel(const Type t);
	};

} // namespace PotatoEngine

#endif // POTATOENGINE_BVH_MODEL_CREATOR_H_
