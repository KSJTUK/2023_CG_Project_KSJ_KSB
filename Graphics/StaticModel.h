#pragma once
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <assimp/anim.h>


namespace Static {

	struct Vertex {
		glm::vec3 position{};
		glm::vec3 normal{};
		glm::vec2 uv{};
	};

	struct Texture {
		GLuint id{};
		std::string type{};
		aiString path{};
	};


	class Mesh {
	public:
		Mesh(std::vector<Vertex>& vertices, std::vector<GLuint>& indices, std::vector<Texture> textures);
		Mesh() {};
		~Mesh();

		void Draw();

	private:

		std::vector<Vertex> m_vertices{};
		std::vector<UINT> m_indices{};
		std::vector<Texture> m_textures{};
		

		UINT m_vao{};
		UINT m_vbo{};
		UINT m_ebo{};


		void SetupMesh();
	};


	class Model {
	public:
		Model();
		Model(std::string& fp);
		~Model();

		void LoadModel(const std::string& path);
		void Render(glm::mat4& matrix);
		
	private :
		Assimp::Importer m_importer{};
		const aiScene* m_scene;

		std::vector<Mesh> m_meshes{};

		std::string m_filepath{};
		std::string m_directory{};


	private:

		void ShowNodeName(const aiNode* node);

		void ProcessNode(aiNode* node);
		Mesh ProcessMesh(aiMesh* mesh);


		std::vector<Texture> LoadMaterial(aiMaterial* material, aiTextureType type, std::string Typename);
	};



}