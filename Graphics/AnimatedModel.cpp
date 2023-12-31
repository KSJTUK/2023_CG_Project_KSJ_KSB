#include "pch.h"
#include "AnimatedModel.h"
#include "Graphics/Shader.h"
#include "Util/Math.h"





Animated::Mesh::Mesh(std::vector<Vertex>& vertices, std::vector<GLuint>& indices, std::vector<Texture> textures, std::vector<VertexBoneData> boneIDWieghts) {
	m_vertices = vertices;
	m_indices = indices;
	m_textures = textures;
	m_bonesIDWeightsEachVertex = boneIDWieghts;


	m_vertexarray = new glm::vec3[m_indices.size()+1];
	m_boneDataArray = new VertexBoneData[m_indices.size() + 1];



	for (UINT i = 0; i < m_indices.size(); ++i) {
		m_vertexarray[i] = m_vertices[m_indices[i]].position;
		m_boneDataArray[i] = m_bonesIDWeightsEachVertex[m_indices[i]];
	}

	if (m_indices.size() % 3 !=  0) {
		std::cerr << "ERROR : POLYGON FAIL!" << std::endl;
		exit(EXIT_FAILURE);
	}

	SetupMesh();
}


Animated::Mesh::~Mesh()
{
}


void Animated::VertexBoneData::addBoneData(UINT bondID, float weight) {
	for (UINT i = 0; i < MAX_BONES_PER_VERTEX; ++i) {
		if (weights[i] == 0.f) {
			ids[i] = bondID;
			weights[i] = weight;
			return;
		}
	}
}


void Animated::Mesh::Draw() {
	int diffuse = 1;
	int specular = 1;

	for (int i = 0; i < m_textures.size(); ++i) {
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, m_textures[i].id);

		std::string number{};
		std::string name{m_textures[i].type};

		if (name == "textureDiffuse") {
			number = std::to_string(diffuse++);
		}
		else if (name == "textureSpecular") {
			number = std::to_string(specular++);
		}

		SHADER->GetActivatedShader()->SetUniformInt(("material." + name + number), i);
	}

	glBindVertexArray(m_vao);
	glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(m_indices.size()), GL_UNSIGNED_INT, (const void*)0);
	glBindVertexArray(0);

	for (int i = 0; i < m_textures.size(); ++i) {
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D,0);
	}
}

bool Animated::Mesh::RayCasting(const glm::vec3& RayOrigin, const glm::vec3& RayDirection,const glm::mat4& World, const std::vector<aiMatrix4x4>& BoneMat) const {
	glm::vec3 v0, v1, v2;

	aiMatrix4x4 boneTransform{};
	glm::mat4 bt{};

	for (UINT i = 0; i < m_indices.size() / 3; ++i) {
		boneTransform =					BoneMat[m_boneDataArray[i].ids[0]] * m_boneDataArray[i].weights[0];
		boneTransform = boneTransform + BoneMat[m_boneDataArray[i].ids[1]] * m_boneDataArray[i].weights[1];
		boneTransform = boneTransform + BoneMat[m_boneDataArray[i].ids[2]] * m_boneDataArray[i].weights[2];
		boneTransform = boneTransform + BoneMat[m_boneDataArray[i].ids[3]] * m_boneDataArray[i].weights[3];

		bt = animatToGLM(boneTransform);
		v0 = World * bt * glm::vec4(m_vertexarray[i],   1.f);

		boneTransform =					BoneMat[m_boneDataArray[i+1].ids[0]] * m_boneDataArray[i+1].weights[0];
		boneTransform = boneTransform + BoneMat[m_boneDataArray[i+1].ids[1]] * m_boneDataArray[i+1].weights[1];
		boneTransform = boneTransform + BoneMat[m_boneDataArray[i+1].ids[2]] * m_boneDataArray[i+1].weights[2];
		boneTransform = boneTransform + BoneMat[m_boneDataArray[i+1].ids[3]] * m_boneDataArray[i+1].weights[3];

		bt = animatToGLM(boneTransform);
		v1 = World * bt * glm::vec4(m_vertexarray[i + 1], 1.f);

		boneTransform =					BoneMat[m_boneDataArray[i + 2].ids[0]] * m_boneDataArray[i + 2].weights[0];
		boneTransform = boneTransform + BoneMat[m_boneDataArray[i+2].ids[1]] * m_boneDataArray[i + 2].weights[1];
		boneTransform = boneTransform + BoneMat[m_boneDataArray[i+2].ids[2]] * m_boneDataArray[i + 2].weights[2];
		boneTransform = boneTransform + BoneMat[m_boneDataArray[i+2].ids[3]] * m_boneDataArray[i + 2].weights[3];

		bt = animatToGLM(boneTransform);
		v2 = World * bt * glm::vec4(m_vertexarray[i+2], 1.f);

		if (triangleRayCasting(RayOrigin, RayDirection, v0, v1, v2)) {
			return true;
		}
	}

	return false;
}

void Animated::Mesh::SetupMesh() {
	SHADER->UseProgram(ShaderType::AnimatedShader);
	//vertices data
	glGenBuffers(1, &m_vboVertices);
	glBindBuffer(GL_ARRAY_BUFFER, m_vboVertices);
	glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(m_vertices[0]), &m_vertices[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	//bones data
	glGenBuffers(1, &m_vboBones);
	glBindBuffer(GL_ARRAY_BUFFER, m_vboBones);
	glBufferData(GL_ARRAY_BUFFER, m_bonesIDWeightsEachVertex.size() * sizeof(m_bonesIDWeightsEachVertex[0]), &m_bonesIDWeightsEachVertex[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	//numbers for sequence indices
	glGenBuffers(1, &m_ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size() * sizeof(GLuint), &m_indices[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	// create VAO and binding data from buffers to shaders
	glGenVertexArrays(1, &m_vao);
	glBindVertexArray(m_vao);
	glBindBuffer(GL_ARRAY_BUFFER, m_vboVertices);

	//vertex position
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, normal));
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, uv));
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1); // offsetof(Vertex, normal) = returns the byte offset of that variable from the start of the struct
	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	//bones
	glBindBuffer(GL_ARRAY_BUFFER, m_vboBones);
	glEnableVertexAttribArray(3);
	glVertexAttribIPointer(3, 4, GL_INT, sizeof(VertexBoneData), (GLvoid*)0); // for INT Ipointer
	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(VertexBoneData), (GLvoid*)offsetof(VertexBoneData, weights));
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	//indices
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
	glBindVertexArray(0);

	SHADER->UnuseProgram();
}

Animated::Model::Model() {
	m_scene = nullptr;
}

Animated::Model::Model(std::string& fp) {
	m_scene = nullptr;
}

Animated::Model::~Model() {
	m_importer.FreeScene();
}

void Animated::Model::Render(const glm::mat4& matrix, int animationindex, float animationcounter) {
	m_currentAnimationID = animationindex;

	SHADER->GetActivatedShader()->SetUniformMat4("M_matrix", GL_FALSE, &matrix[0][0]);
	SHADER->GetActivatedShader()->SetUniformMat4("TIM_matrix", GL_TRUE, &glm::inverse(matrix)[0][0]);

	UpdateBoneTransform(animationcounter, m_transformBuffer);

	for (UINT i = 0; i < m_transformBuffer.size(); ++i) {
		SHADER->GetActivatedShader()->SetUniformMat4(("bones[" + std::to_string(i) + "]"), GL_TRUE, (const float*)&m_transformBuffer[i]);
	}
	
	for (int i = 0; i < m_meshes.size(); ++i) {
		m_meshes[i].Draw();
	}
}

void Animated::Model::ChangeAnimation(int index) { }

bool Animated::Model::RayCasting(const glm::vec3& RayOrigin, const glm::vec3& RayDirection, const glm::mat4& World) const {
	for (const auto& mesh : m_meshes) {
		if (m_transformBuffer.size() == 0) return false;

		if (mesh.RayCasting(RayOrigin, RayDirection,World,m_transformBuffer)) {
			return true;
		}
	}

	return false;
}

void Animated::Model::ShowNodeName(const aiNode* node) {
	printf("%s\n", node->mName.data);

	for (UINT i = 0; i < node->mNumChildren; ++i) {
		ShowNodeName(node->mChildren[i]);
	}
}

void Animated::Model::ProcessNode(aiNode* node) {
	Mesh mesh{};

	for (UINT i = 0; i < m_scene->mNumMeshes; ++i) {
		aiMesh* aimesh = m_scene->mMeshes[i];
		mesh = ProcessMesh(aimesh);
		m_meshes.push_back(mesh);
	}
}

Animated::Mesh Animated::Model::ProcessMesh(aiMesh* mesh) {
	printf("Bones: %d	|	Vertices : %d\n", mesh->mNumBones, mesh->mNumVertices);

	std::vector<Vertex> vertices{};
	std::vector<GLuint> indices{};
	std::vector<Texture> textures{};
	std::vector<VertexBoneData> bonesIDWeightsEachVertex{};

	vertices.reserve(mesh->mNumVertices);
	indices.reserve(mesh->mNumFaces);

	bonesIDWeightsEachVertex.resize(mesh->mNumVertices);

	//vertices
	for (UINT i = 0; i < mesh->mNumVertices; i++) {
		Vertex vertex;
		glm::vec3 vector{};
		vector.x = mesh->mVertices[i].x;
		vector.y = mesh->mVertices[i].y;
		vector.z = mesh->mVertices[i].z;
		vertex.position = vector;

		if (mesh->mNormals != NULL) {
			vector.x = mesh->mNormals[i].x;
			vector.y = mesh->mNormals[i].y;
			vector.z = mesh->mNormals[i].z;
			vertex.normal = vector;
			//std::cout << vector.x << " " << vector.y << " " << vector.z << std::endl;
		}
		else {
			vertex.normal = glm::vec3(1.f);
		}

		// in assimp model can have 8 different texture coordinates
		// we only care about the first set of texture coordinates
		if (mesh->mTextureCoords[0]) {
			glm::vec2 vec;
			vec.x = mesh->mTextureCoords[0][i].x;
			vec.y = mesh->mTextureCoords[0][i].y;
			vertex.uv = vec;
		}
		else {
			vertex.uv = glm::vec2(0.0f, 0.0f);
		}
		vertices.push_back(vertex);
	}

	//indices
	for (UINT i = 0; i < mesh->mNumFaces; i++) {
		aiFace face = mesh->mFaces[i];
		indices.push_back(face.mIndices[0]);
		indices.push_back(face.mIndices[1]);
		indices.push_back(face.mIndices[2]);
	}

	//material
	if (mesh->mMaterialIndex >= 0) {
		//all pointers created in assimp will be deleted automaticaly when we call import.FreeScene();
		aiMaterial* material = m_scene->mMaterials[mesh->mMaterialIndex];
		std::vector<Texture> diffuseMaps = LoadMaterial(material, aiTextureType_DIFFUSE, "texture_diffuse");
		bool exist = false;
		for (int i = 0; (i < textures.size()) && (diffuseMaps.size() != 0); i++) {
			if (textures[i].path == diffuseMaps[0].path) {
				exist = true;
			}
		}
		if (!exist && diffuseMaps.size() != 0) textures.push_back(diffuseMaps[0]);

		std::vector<Texture> specularMaps = LoadMaterial(material, aiTextureType_SPECULAR, "texture_specular");
		exist = false;
		for (int i = 0; (i < textures.size()) && (specularMaps.size() != 0); i++) {
			if (textures[i].path == specularMaps[0].path) {
				exist = true;
			}
		}
		if (!exist && specularMaps.size() != 0) textures.push_back(specularMaps[0]);
	}

	// load bones
	for (UINT i = 0; i < mesh->mNumBones; i++) {
		UINT boneIndex = 0;
		std::string boneName(mesh->mBones[i]->mName.data);

		std::cout << mesh->mBones[i]->mName.data << std::endl;

		if (m_boneDict.find(boneName) == m_boneDict.end()) {
			// Allocate an index for a new bone
			boneIndex = m_boneNumber;
			m_boneNumber++;
			BoneMatrix bi;
			m_boneMatrices.push_back(bi);
			m_boneMatrices[boneIndex].offsetMatrix = mesh->mBones[i]->mOffsetMatrix;
			m_boneDict[boneName] = boneIndex;

			//cout << "bone_name: " << bone_name << "			 bone_index: " << bone_index << endl;
		}
		else {
			boneIndex = m_boneDict[boneName];
		}

		for (UINT j = 0; j < mesh->mBones[i]->mNumWeights; j++) {
			UINT vertexID = mesh->mBones[i]->mWeights[j].mVertexId;
			float weight = mesh->mBones[i]->mWeights[j].mWeight;
			bonesIDWeightsEachVertex[vertexID].addBoneData(boneIndex, weight);


		}
	}

	return Mesh(vertices, indices, textures, bonesIDWeightsEachVertex);
}



std::vector<Animated::Texture> Animated::Model::LoadMaterial(aiMaterial* material, aiTextureType type, std::string Typename) {
	std::vector<Animated::Texture> result{};

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
		glBindTexture(GL_TEXTURE_2D,tex.id);
		
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		stbi_image_free(data);
		glBindTexture(GL_TEXTURE_2D,0);

		result.push_back(tex);
	}

	return result;
}

UINT Animated::Model::FindPosition(float AnimationTime, const aiNodeAnim* animnode) {
	for (UINT i = 0; i < animnode->mNumPositionKeys - 1; i++) {
		if (AnimationTime < (float)animnode->mPositionKeys[i + 1].mTime) {
			return i;
		}
	}
	std::cerr << "ERROR : Disable to fine next position " << std::endl;
	exit(EXIT_FAILURE);
	return -1;
}

UINT Animated::Model::FindRotation(float AnimationTime, const aiNodeAnim* animnode) {
	for (UINT i = 0; i < animnode->mNumRotationKeys - 1; i++) {
		if (AnimationTime < (float)animnode->mRotationKeys[i + 1].mTime) {
			return i;
		}
	}
	std::cerr << "ERROR : Disable to fine next rotation " << std::endl;
	exit(EXIT_FAILURE);
	return -1;
}

UINT Animated::Model::FindScaling(float AnimationTime, const aiNodeAnim* animnode) {
	for (UINT i = 0; i < animnode->mNumScalingKeys - 1; i++) {
		if (AnimationTime < (float)animnode->mScalingKeys[i + 1].mTime) {
			return i;
		}
	}
	std::cerr << "ERROR : Disable to find next scale " << std::endl;
	exit(EXIT_FAILURE);
	return -1;
}

const aiNodeAnim* Animated::Model::FindNodeAnimation(const aiAnimation* animation, const std::string& nodename) {
	for (UINT i = 0;i <  animation->mNumChannels; ++i) {
		const aiNodeAnim* animnode = animation->mChannels[i];
		if (strcmp(animnode->mNodeName.data, nodename.c_str()) == 0) {
			return animnode;
		}
	}

	return nullptr;
}

aiVector3D Animated::Model::CalculatePolatedPosition(float AnimationTime, const aiNodeAnim* animnode) {
	if (animnode->mNumPositionKeys == 1) {
		return animnode->mPositionKeys[0].mValue;
	}

	UINT positionNext = FindPosition(AnimationTime, animnode);
	UINT nextPositionIndex = positionNext + 1;
	float deltaTime = (float)(animnode->mPositionKeys[nextPositionIndex].mTime - animnode->mPositionKeys[positionNext].mTime);
	float factor = (AnimationTime - (float)animnode->mPositionKeys[positionNext].mTime) / deltaTime;

	aiVector3D start = animnode->mPositionKeys[positionNext].mValue;
	aiVector3D end = animnode->mPositionKeys[nextPositionIndex].mValue;
	aiVector3D delta = end - start;

	return start + factor * delta;
}

aiQuaternion Animated::Model::CalculatePolatedRotation(float AnimationTime, const aiNodeAnim* animnode) {
	if (animnode->mNumRotationKeys == 1) {
		return animnode->mRotationKeys[0].mValue;
	}

	UINT rotationIndex = FindRotation(AnimationTime, animnode);
	UINT nextRotationIndex = rotationIndex + 1;
	float deltaTime = (float)(animnode->mRotationKeys[nextRotationIndex].mTime - animnode->mRotationKeys[rotationIndex].mTime);
	float factor = (AnimationTime - (float)animnode->mRotationKeys[rotationIndex].mTime) / deltaTime;
	assert(factor >= 0.0f && factor <= 1.0f);
	aiQuaternion start_quat = animnode->mRotationKeys[rotationIndex].mValue;
	aiQuaternion end_quat = animnode->mRotationKeys[nextRotationIndex].mValue;

	return nlerp(start_quat, end_quat, factor);
}

aiVector3D Animated::Model::CalculatePolatedScailing(float AnimationTime, const aiNodeAnim* animnode) {
	if (animnode->mNumScalingKeys == 1) {
		return animnode->mScalingKeys[0].mValue;
	}

	UINT scalingIndex = FindScaling(AnimationTime, animnode);
	UINT nextScalingIndex = scalingIndex + 1;
	float deltaTime = (float)(animnode->mScalingKeys[nextScalingIndex].mTime - animnode->mScalingKeys[scalingIndex].mTime);
	float  factor = (AnimationTime - (float)animnode->mScalingKeys[scalingIndex].mTime) / deltaTime;
	aiVector3D start = animnode->mScalingKeys[scalingIndex].mValue;
	aiVector3D end = animnode->mScalingKeys[nextScalingIndex].mValue;
	aiVector3D delta = end - start;

	return start + factor * delta;
}

void Animated::Model::ReadNodeHierarchy(float Animationtime,const aiNode* node,const aiMatrix4x4& ParentTransform) {
	const aiAnimation* animation = m_scene->mAnimations[m_currentAnimationID];
	aiMatrix4x4 nodeTransform = node->mTransformation;

	const aiNodeAnim* animnode = FindNodeAnimation(animation, node->mName.data);

	if (animnode) {
		aiVector3D scaling = CalculatePolatedScailing(Animationtime, animnode);
		aiMatrix4x4 scaleMatrix{};
		aiMatrix4x4::Scaling(scaling, scaleMatrix);

		aiQuaternion rotate = CalculatePolatedRotation(Animationtime, animnode);
		aiMatrix4x4 rotateMatrix{ aiMatrix4x4{rotate.GetMatrix()} };

		aiVector3D translate = CalculatePolatedPosition(Animationtime, animnode);
		aiMatrix4x4 translateMatrix{ };
		aiMatrix4x4::Translation(translate, translateMatrix);

		nodeTransform = translateMatrix * rotateMatrix * scaleMatrix;
	}

	aiMatrix4x4 globalTransform = ParentTransform * nodeTransform;

	if (m_boneDict.find(node->mName.data) != m_boneDict.end()) {
		UINT boneIndex = m_boneDict[node->mName.data];
		m_boneMatrices[boneIndex].finalWorldTranform = m_globalInverseTransfrom * globalTransform * m_boneMatrices[boneIndex].offsetMatrix;	
	}

	for (UINT i = 0; i < node->mNumChildren; ++i) {
		ReadNodeHierarchy(Animationtime, node->mChildren[i], globalTransform);
	}
}

void Animated::Model::UpdateBoneTransform(double Elapsed, std::vector<aiMatrix4x4>& Transforms) {
	aiMatrix4x4 identity;

	double tick = Elapsed * m_ticksPerSecond;
	float animationTime = static_cast<float>(std::fmod(tick, m_scene->mAnimations[m_currentAnimationID]->mDuration));

	ReadNodeHierarchy(animationTime, m_scene->mRootNode, identity);

	Transforms.resize(m_boneNumber);

	for (UINT i = 0; i < m_boneNumber; ++i) {
		Transforms[i] = m_boneMatrices[i].finalWorldTranform;
	}
}

void Animated::Model::LoadModel(const std::string& path) {
	m_scene = m_importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

	if (!m_scene or m_scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE or !m_scene->mRootNode) {
		std::cerr << "Error assimp : " << m_importer.GetErrorString() << std::endl;
		return exit(EXIT_FAILURE);
	}

	m_globalInverseTransfrom = m_scene->mRootNode->mTransformation;
	m_globalInverseTransfrom.Inverse();

	m_ticksPerSecond = static_cast<float>(m_scene->mAnimations[m_currentAnimationID]->mTicksPerSecond);

	m_directory = path.substr(0, path.find_last_of('/'));

	std::cout << "scene->HasAnimations() 1: " << m_scene->HasAnimations() << std::endl;
	std::cout << "scene->mNumMeshes 1: " << m_scene->mNumMeshes << std::endl;
	std::cout << "scene->mAnimations[0]->mNumChannels 1: " << m_scene->mAnimations[0]->mNumChannels << std::endl;
	std::cout << "scene->mAnimations[0]->mDuration 1 : " << m_scene->mAnimations[0]->mDuration << std::endl;
	std::cout << "scene->mAnimations[0]->mTicksPerSecond 1 : " << m_scene->mAnimations[0]->mTicksPerSecond << std::endl;
	std::cout << "m_scene->mNumAnimations : " << m_scene->mNumAnimations << std::endl << std::endl;

	std::cout << "name nodes : " ;
	ShowNodeName(m_scene->mRootNode);
	std::cout << std::endl;

	std::cout << "name bones : ";
	ProcessNode(m_scene->mRootNode);

	std::cout << "name nodes animation : ";
	for (UINT i = 0; i < m_scene->mAnimations[0]->mNumChannels; i++) {
		std::cout << m_scene->mAnimations[0]->mChannels[i]->mNodeName.C_Str() << std::endl;
	}
	std::cout << std::endl;
}