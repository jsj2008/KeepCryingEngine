#include "Mesh.h"

#include <vector>

#include "Globals.h"

using namespace std;

Mesh::Mesh(const MeshIdentifier& meshIdentifier) :
	Asset(meshIdentifier, AssetType::Mesh)
{
}

Mesh::~Mesh()
{
	glDeleteBuffers(1, &indicesBufferId);
	glDeleteBuffers(1, &vertexBufferId);
}

const AABB & Mesh::GetAABB() const
{
	return aabb;
}

GLuint Mesh::GetVertexBufferId() const
{
	return vertexBufferId;
}

GLuint Mesh::GetIndicesBufferId() const
{
	return indicesBufferId;
}

GLuint Mesh::GetBoneIndicesBufferId() const
{
	return boneIndicesBufferId;
}

GLuint Mesh::GetBoneWeightsBufferId() const
{
	return boneWeightsBufferId;
}

GLuint Mesh::GetTangentBufferId() const
{
	return tangentBufferId;
}

GLsizei Mesh::GetVerticesNumber() const
{
	return nVertices;
}

GLsizei Mesh::GetIndicesNumber() const
{
	return nIndices;
}

GLenum Mesh::GetDrawMode() const
{
	return drawMode;
}

void Mesh::SetMeshData(const vector<Vertex>& vertices, const vector<GLushort>& indices, const vector<Bone>& bonesconst,const std::vector<float3>& tangents, GLenum drawMode)
{
	GenerateBuffers(vertices, indices,tangents);
	CalculateAABBForMesh(vertices);
	this->drawMode = drawMode;
	this->vertices = vertices;
	this->originalVertices = vertices;
	this->indices = indices;
	this->bones = bones;

	if(!bones.empty())
	{
		GenerateBoneBuffers();
	}
}

const std::vector<Vertex>& Mesh::GetVertices() const
{
	return vertices;
}

const std::vector<GLushort>& Mesh::GetIndices() const
{
	return indices;
}

const std::vector<Vertex>& Mesh::GetOriginalVertices() const
{
	return originalVertices;
}

void Mesh::UpdateVertices(const std::vector<Vertex>& vertices)
{
	assert(dynamicDraw);
	assert(vertices.size() == nVertices);

	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferId);
	glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(Vertex), &vertices[0]);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	this->vertices = vertices;
}

const std::vector<Bone>& Mesh::GetBones() const
{
	return bones;
}

void Mesh::SetDynamicDraw(bool dynamicDraw)
{
	if(this->dynamicDraw != dynamicDraw)
	{
		glDeleteBuffers(1, &vertexBufferId);

		const Vertex * verticesPointer = &vertices[0];
		glGenBuffers(1, &vertexBufferId);
		glBindBuffer(GL_ARRAY_BUFFER, vertexBufferId);
		glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * nVertices, verticesPointer, dynamicDraw ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	this->dynamicDraw = dynamicDraw;
}

void Mesh::GenerateBuffers(const vector<Vertex>& vertices, const vector<GLushort>& indices, const std::vector<float3>& tangents)
{
	assert(vertices.size() > 0);
	assert(indices.size() > 0);
	assert(tangents.size() > 0);

	nVertices = vertices.size();
	nIndices = indices.size();

	//Generate Vertex buffer
	const Vertex * verticesPointer = &vertices[0];
	glGenBuffers(1, &vertexBufferId);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferId);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * nVertices, verticesPointer, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	//Generate Indices buffer
	const GLushort* indicesPointer = &indices[0];
	glGenBuffers(1, &indicesBufferId);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indicesBufferId);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLushort) * nIndices, indicesPointer, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	float nVertices = tangents.size();

	//Generate Vertex buffer
	const float3 * tangentsPointer = &tangents[0];
	glGenBuffers(1, &tangentBufferId);
	glBindBuffer(GL_ARRAY_BUFFER, tangentBufferId);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float3) * nVertices, tangentsPointer, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Mesh::GenerateBoneBuffers()
{
	vector<vector<int>> bIndices;
	vector<vector<float>> bWeights;

	bIndices.resize(vertices.size());
	bWeights.resize(vertices.size());

	for(size_t i = 0; i < bones.size(); ++i)
	{
		for(size_t j = 0; j < bones[i].weights.size(); ++j)
		{
			bIndices[bones[i].weights[j].vertex].push_back(i);
			bWeights[bones[i].weights[j].vertex].push_back(bones[i].weights[j].weight);
		}
	}

	for(size_t i = 0; i < bIndices.size(); ++i)
	{
		assert(bIndices[i].size() <= 4);
		assert(bWeights[i].size() <= 4);

		while(bIndices[i].size() < 4)
		{
			bIndices[i].push_back(0);
			bWeights[i].push_back(0.0f);
		}
	}

	int* bIndicesArray = (int*)malloc(bIndices.size() * 4 * sizeof(int));
	float* bWeightsArray = (float*)malloc(bWeights.size() * 4 * sizeof(float));

	for(size_t i = 0; i < bIndices.size(); i++)
	{
		for(size_t j = 0; j < 4; j++)
		{
			bIndicesArray[i * 4 + j] = bIndices[i][j];
			bWeightsArray[i * 4 + j] = bWeights[i][j];
		}
	}

	//Generate Vertex buffer
	const int * boneIndicesPointer = bIndicesArray;
	glGenBuffers(1, &boneIndicesBufferId);
	glBindBuffer(GL_ARRAY_BUFFER, boneIndicesBufferId);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GL_INT) * 4 * bIndices.size(), boneIndicesPointer, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	//Generate Vertex buffer
	const float * boneWeightsPointer = bWeightsArray;
	glGenBuffers(1, &boneWeightsBufferId);
	glBindBuffer(GL_ARRAY_BUFFER, boneWeightsBufferId);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GL_FLOAT) * 4 * bWeights.size(), boneWeightsPointer, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	free(bIndicesArray);
	free(bWeightsArray);
}

void Mesh::CalculateAABBForMesh(const vector<Vertex> &vertices)
{
	float3 * positions = new float3[vertices.size()];
	for (size_t i = 0; i < vertices.size(); i++)
	{
		positions[i] = vertices[i].position;
	}

	aabb.SetNegativeInfinity();
	aabb.Enclose(positions, vertices.size());
	RELEASE_ARRAY(positions);
}

bool MeshIdentifier::operator<(const MeshIdentifier & other) const
{
	return name < other.name || (name == other.name && path.string() < other.path.string());
}
