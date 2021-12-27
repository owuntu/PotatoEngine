#ifndef POTATOENGINE_MODEL_H_
#define POTATOENGINE_MODEL_H_

#include <string>

struct aiNode;
struct aiScene;
struct aiMesh;

namespace PotatoEngine
{
	class Model
	{
	public:
		Model(const std::string& path)
		{
			m_directory = path;
		}
		virtual ~Model() {}

		virtual void Draw() const = 0;
		virtual void DrawVertices() const = 0;

		void LoadModel();

	protected:
		void ProcessNode(aiNode* pNode, const aiScene* pScene);

		std::string m_directory;
	private:
		virtual void ProcessMesh(aiMesh* pMesh) = 0;
		virtual void PostSetup() {}
	};
} // namespace PotatoEngine

#endif // POTATOENGINE_MODEL_H_
