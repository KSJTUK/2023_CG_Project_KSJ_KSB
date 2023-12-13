#include "pch.h"
#include "StaticModel.h"
#include "Graphics/Shader.h"


Static::Mesh::Mesh(std::vector<Vertex>& vertices, std::vector<GLuint>& indices, std::vector<Texture> textures) :
m_vertices(vertices), m_indices(indices), m_textures(textures){
	SetupMesh();
}

Static::Mesh::~Mesh() {

}


void Static::Mesh::SetupMesh() {
	SHADER->UseProgram(ShaderType::StaticShader);

	glGenVertexArrays(1, &m_vao);
	glBindVertexArray(m_vao);
	

	glGenBuffers(1, &m_vbo);
	glBindBuffer(GL_ARRAY_BUFFER,m_vbo);
	glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(Vertex), &m_vertices[0], GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, position));
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, normal));
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, uv));
	
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);


	glGenBuffers(1, &m_ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,m_ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size() * sizeof(UINT), &m_indices[0], GL_STATIC_DRAW);

	glBindVertexArray(0);

	SHADER->UnuseProgram();

}

void Static::Mesh::Draw() {
	int diffuse = 1;
	int specular = 1;


	for (int i = 0; i < m_textures.size(); ++i) {
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, m_textures[i].id);

		std::string number{};
		std::string name{ m_textures[i].type };


		if (name == "texture_diffuse") {
			number = std::to_string(diffuse++);
		}
		else if (name == "texture_specular") {
			number = std::to_string(specular++);
		}

		SHADER->GetActivatedShader()->SetUniformInt(("material." + name + number), i);



	}



	glBindVertexArray(m_vao);
	glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(m_indices.size()), GL_UNSIGNED_INT, (const void*)0);
	glBindVertexArray(0);

	for (int i = 0; i < m_textures.size(); ++i) {
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, 0);
	}



}


Static::Model::Model() {
	m_scene = nullptr;
}


Static::Model::Model(std::string& fp) {
	m_scene = nullptr;
}

Static::Model::~Model() {
	m_importer.FreeScene();
}

void Static::Model::LoadModel(const std::string& path) {
	m_scene = m_importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

	
	if (!m_scene or m_scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE or !m_scene->mRootNode) {
		std::cerr << "Error assimp : " << m_importer.GetErrorString() << std::endl;
		return exit(EXIT_FAILURE);
	}


	m_directory = path.substr(0, path.find_last_of('/'));
	std::cout << "scene->mNumMeshes : " << m_scene->mNumMeshes << std::endl;
	ShowNodeName(m_scene->mRootNode);

	ProcessNode(m_scene->mRootNode);


}

void Static::Model::Render(glm::mat4& matrix) {

	SHADER->GetActivatedShader()->SetUniformMat4("M_matrix", GL_FALSE, &matrix[0][0]);
	SHADER->GetActivatedShader()->SetUniformMat4("TIM_matrix", GL_TRUE, &glm::inverse(matrix)[0][0]);



	for (int i = 0; i < m_meshes.size(); ++i) {
		m_meshes[i].Draw();
	}

}


void Static::Model::ShowNodeName(const aiNode* node) {
	printf("%s\n", node->mName.data);

	for (UINT i = 0; i < node->mNumChildren; ++i) {
		ShowNodeName(node->mChildren[i]);
	}

}

void Static::Model::ProcessNode(aiNode* node) {
	Mesh mesh{};

	for (UINT i = 0; i < m_scene->mNumMeshes; ++i) {
		aiMesh* aimesh = m_scene->mMeshes[i];
		mesh = ProcessMesh(aimesh);
		m_meshes.push_back(mesh);
	}



}

Static::Mesh Static::Model::ProcessMesh(aiMesh* mesh) {
	printf("Vertices : %d\n", mesh->mNumVertices);

	std::vector<Vertex> vertices{};
	std::vector<GLuint> indices{};
	std::vector<Texture> textures{};


	vertices.reserve(mesh->mNumVertices);
	indices.reserve(mesh->mNumFaces);



	//vertices
	for (UINT i = 0; i < mesh->mNumVertices; i++)
	{
		Vertex vertex;
		glm::vec3 vector{};
		vector.x = mesh->mVertices[i].x;
		vector.y = mesh->mVertices[i].y;
		vector.z = mesh->mVertices[i].z;
		vertex.position = vector;

		if (mesh->mNormals != NULL)
		{
			vector.x = mesh->mNormals[i].x;
			vector.y = mesh->mNormals[i].y;
			vector.z = mesh->mNormals[i].z;
			vertex.normal = vector;
			//std::cout << vector.x << " " << vector.y << " " << vector.z << std::endl;
		}
		else
		{
			vertex.normal = glm::vec3(1.f);
		}


		// in assimp model can have 8 different texture coordinates
		// we only care about the first set of texture coordinates
		if (mesh->mTextureCoords[0])
		{
			glm::vec2 vec;
			vec.x = mesh->mTextureCoords[0][i].x;
			vec.y = mesh->mTextureCoords[0][i].y;
			vertex.uv = vec;
		}
		else
		{
			vertex.uv = glm::vec2(0.0f, 0.0f);
		}
		vertices.push_back(vertex);
	}

	//indices
	for (UINT i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		indices.push_back(face.mIndices[0]);
		indices.push_back(face.mIndices[1]);
		indices.push_back(face.mIndices[2]);
	}

	//material
	if (mesh->mMaterialIndex >= 0)
	{
		//all pointers created in assimp will be deleted automaticaly when we call import.FreeScene();
		aiMaterial* material = m_scene->mMaterials[mesh->mMaterialIndex];
		std::vector<Texture> diffuse_maps = LoadMaterial(material, aiTextureType_DIFFUSE, "textureDiffuse");
		bool exist = false;
		for (int i = 0; (i < textures.size()) && (diffuse_maps.size() != 0); i++)
		{
			if (textures[i].path == diffuse_maps[0].path)
			{

				exist = true;
			}
		}
		if (!exist && diffuse_maps.size() != 0) textures.push_back(diffuse_maps[0]);
		//textures.insert(textures.end(), diffuse_maps.begin(), diffuse_maps.end());

		std::vector<Texture> specular_maps = LoadMaterial(material, aiTextureType_SPECULAR, "textureSpecular");
		exist = false;
		for (int i = 0; (i < textures.size()) && (specular_maps.size() != 0); i++)
		{
			if (textures[i].path == specular_maps[0].path)
			{
				exist = true;
			}
		}
		if (!exist && specular_maps.size() != 0) textures.push_back(specular_maps[0]);
		//textures.insert(textures.end(), specular_maps.begin(), specular_maps.end());

	}


	return Mesh{ vertices,indices,textures };

}


std::vector<Static::Texture> Static::Model::LoadMaterial(aiMaterial* material, aiTextureType type, std::string Typename) {
	std::vector<Static::Texture> result{};


	for (UINT i = 0; i < material->GetTextureCount(type); ++i) {
		aiString str{};

		material->GetTexture(type, i, &str);

		std::string filename = std::string(str.C_Str());
		filename = m_directory + '/' + filename;

		std::cout << "Texture File Name : " << filename << std::endl;


		Texture tex{};
		tex.type = Typename;
		tex.path = str;
		tex.id = 0;

		int width, height, nrChannels;


		byte* data = stbi_load(filename.c_str(), &width, &height, &nrChannels, 4);
		if (!data) { std::cerr << "ERROR : Disable to Read Image : " << filename << std::endl; exit(EXIT_FAILURE); }


		glGenTextures(1, &tex.id);
		glBindTexture(GL_TEXTURE_2D, tex.id);


		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		stbi_image_free(data);
		glBindTexture(GL_TEXTURE_2D, 0);

		result.push_back(tex);

	}


	return result;
}


