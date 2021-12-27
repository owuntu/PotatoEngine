#ifndef POTATOENGINE_MODEL_H_
#define POTATOENGINE_MODEL_H_

#include <string>

struct aiNode;
struct aiScene;
struct aiMesh;

namespace PotatoEngine
{
	class ModelCreator;

	class Model
	{
	public:
		Model()
		{}

		virtual ~Model() {}

		virtual void Draw() const = 0;
		virtual void DrawVertices() const = 0;

		void LoadModel(const std::string& path);

	protected:
		void ProcessNode(aiNode* pNode, const aiScene* pScene);

		std::string m_directory;
	private:
		virtual void ProcessMesh(aiMesh* pMesh) = 0;
		virtual void PostSetup() {}

		friend class ModelCreator;
	};
} // namespace PotatoEngine

#endif // POTATOENGINE_MODEL_H_
