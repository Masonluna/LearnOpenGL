#include "Mesh.h"

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures)
{
	this->m_Vertices = vertices;
	this->m_Indices  = indices;
	this->m_Textures = textures;

	setupMesh();
}

void Mesh::Draw(Shader & shader)
{
    unsigned int diffuseNum = 1;
    unsigned int specularNum = 1;
    for (unsigned int i = 0; i < m_Textures.size(); i++)
    {
        GLCall(glActiveTexture(GL_TEXTURE0 + i)); // activate proper texture unit before binding
        // retrieve texture number (the N in diffuse_textureN)
        std::string number;
        std::string name = m_Textures[i].type;
        if (name == "texture_diffuse")
            number = std::to_string(diffuseNum++);
        else if (name == "texture_specular")
            number = std::to_string(specularNum++);

        shader.setInt(("material." + name + number).c_str(), i);
        GLCall(glBindTexture(GL_TEXTURE_2D, m_Textures[i].id));
    }
    GLCall(glActiveTexture(GL_TEXTURE0));

    // draw mesh
    m_VertexArray.Bind();
    GLCall(glDrawElements(GL_TRIANGLES, m_Indices.size(), GL_UNSIGNED_INT, 0));
    GLCall(glBindVertexArray(0));
}

void Mesh::setupMesh()
{
	m_VertexBuffer = VertexBuffer(this->m_Vertices);
	m_IndexBuffer = IndexBuffer(this->m_Indices);
	m_Layout.Push<float>(3); // Vertex
	m_Layout.Push<float>(3); // Normals
	m_Layout.Push<float>(2); // Texture Coords
	
	m_VertexArray.AddBuffer(m_VertexBuffer, m_Layout);
}
