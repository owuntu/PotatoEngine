// Model interface class
#ifndef POTATOENGINE_MODEL_H_
#define POTATOENGINE_MODEL_H_

#include <glm/glm.hpp>

#include <string>

struct aiNode;
struct aiScene;
struct aiMesh;

namespace PotatoEngine
{
	class ModelCreator;
	class ShaderProgram;

	class Model
	{
	public:
		Model() :
			m_color(1.f, 1.f, 1.f, 1.f),
			m_transformation(1.f)
		{}

		virtual ~Model() {}

		void Draw(ShaderProgram* pShader = nullptr) const;
		void DrawVertices(ShaderProgram* pShader = nullptr) const;

		void LoadModel(const std::string& objModelPath);

		void SetColor(const glm::vec4& c) { m_color = c; }
		void SetColor(const glm::vec3& c) { m_color = glm::vec4(c, 1.0f); }
		const glm::vec4& GetColor() const { return m_color; }

	protected:
		void ProcessNode(aiNode* pNode, const aiScene* pScene);

		std::string m_directory;
		glm::vec4 m_color;
		glm::mat4 m_transformation;
	private:
		virtual void ProcessMesh(aiMesh* pMesh) = 0;
		virtual void PostSetup() {}

		virtual void DoDraw() const = 0;
		virtual void DoDrawVertices() const = 0;

		friend class ModelCreator;
	};
} // namespace PotatoEngine

#endif // POTATOENGINE_MODEL_H_
