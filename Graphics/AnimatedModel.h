#pragma once
#include <assimp/postprocess.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/anim.h>


namespace Animated {
#define MAX_BONES_PER_VERTEX 4
	constexpr UINT MAX_BONES = 100;



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


	struct BoneMatrix {
		aiMatrix4x4 offsetMatrix{};
		aiMatrix4x4 finalWorldTranform{};
	};


	struct VertexBoneData {
		UINT ids[MAX_BONES_PER_VERTEX];
		float weights[MAX_BONES_PER_VERTEX];
		VertexBoneData() {
			ZeroMemory(ids, sizeof(ids));
			ZeroMemory(weights, sizeof(weights));
		}


		void addBoneData(UINT bondID, float weight);
	};

	struct Properties {
		glm::vec3 position{ 0.f,0.f,0.f };
		glm::vec3 rotation{ glm::radians(-90.f),0.f,0.f };
		glm::vec3 scale{ 1.f,1.f,1.f };
	};

	class Mesh {
	public:
		Mesh(std::vector<Vertex>& vertices, std::vector<GLuint>& indices, std::vector<Texture> textures, std::vector<VertexBoneData> boneIDWieghts);
		Mesh() {};
		~Mesh();

		void Draw();
	private:

		std::vector<Vertex> m_vertices{};
		std::vector<UINT> m_indices{};
		std::vector<Texture> m_textures{};
		std::vector<VertexBoneData> m_bonesID_Weights_eachVertex{};

		UINT m_vao{};
		UINT m_vboVertices{};
		UINT m_vboBones{};
		UINT m_ebo{};

		
		void SetupMesh();


	};




	class Model {
	public:
		Model();
		Model(std::string& fp);
		~Model();



		void LoadModel(const std::string& path);
		
		void Update(float DeltaTime);

		void Render();

		
		void ChangeAnimation(int index);

	private:


		Assimp::Importer m_importer;
		const aiScene* m_scene;

		std::vector<Mesh> m_meshes{};

		std::string m_filepath{};
		std::string m_directory{};


		std::unordered_map<std::string, UINT> m_boneDict{};
		UINT m_boneNumber = 0;

		std::vector<BoneMatrix> m_boneMatrices{};

		aiMatrix4x4 m_globalInverseTransfrom{};



		float m_ticksPerSecond = 0.f;
		int m_currentAnimationID = 0;
		
		std::vector<aiMatrix4x4> m_transformBuffer{};




	private:

		void ProcessNode(aiNode* node);




	};



}

