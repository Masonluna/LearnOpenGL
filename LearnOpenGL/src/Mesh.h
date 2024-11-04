#pragma once
#include <glm/glm.hpp>
#include <string>
#include <vector>
#include "Shader.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "VertexLayout.h"


class Mesh
{
public:
	std::vector<Vertex>       m_Vertices;
	std::vector<unsigned int> m_Indices;
	std::vector<Texture>      m_Textures;

	Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures);
	void Draw(Shader& shader);

private:
	VertexArray m_VertexArray;
	VertexBuffer m_VertexBuffer;
	IndexBuffer m_IndexBuffer;

	VertexBufferLayout m_Layout;

	void setupMesh();
};