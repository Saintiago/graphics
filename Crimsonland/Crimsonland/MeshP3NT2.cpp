#include "stdafx.h"
#include "MeshP3NT2.h"
#include "Lights.h"

namespace
{
/// Привязывает вершины к состоянию OpenGL,
/// затем вызывает 'callback'.
template <class T>
void DoWithBindedArrays(const std::vector<SVertexP3NT2> &vertices, T && callback)
{
    // Включаем режим vertex array и normal array.
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);

    // Выполняем привязку vertex array и normal array
    const size_t stride = sizeof(SVertexP3NT2);
    glNormalPointer(GL_FLOAT, stride, glm::value_ptr(vertices[0].normal));
    glVertexPointer(3, GL_FLOAT, stride, glm::value_ptr(vertices[0].position));
    glTexCoordPointer(2, GL_FLOAT, stride, glm::value_ptr(vertices[0].texCoord));

    // Выполняем внешнюю функцию.
    callback();

    // Выключаем режим vertex array и normal array.
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);
}

GLenum GetPrimitiveType(MeshType type)
{
    switch (type)
    {
    case MeshType::Triangles:
        return GL_TRIANGLES;
    case MeshType::TriangleFan:
        return GL_TRIANGLE_FAN;
    case MeshType::TriangleStrip:
        return GL_TRIANGLE_STRIP;
    default:
        throw std::logic_error("Unhandled mesh type");
    }
}
}

void SMeshP3NT2::Clear(MeshType meshType)
{
    m_meshType = meshType;
    m_indicies.clear();
    m_vertices.clear();
}

void SMeshP3NT2::Draw() const
{
    DoWithBindedArrays(m_vertices, [this] {
		/*glBegin(GL_POINTS);
		for (auto & vertex : m_vertices)
		{
			glVertex3f(vertex.position.x, vertex.position.y, vertex.position.z);
		}
		glEnd();*/
        GLenum primitive = GetPrimitiveType(m_meshType);
        glDrawElements(primitive, GLsizei(m_indicies.size()),
                       GL_UNSIGNED_INT, m_indicies.data());
    });

	glBegin(GL_LINES);

	const glm::vec4 RED_RGBA = { 1, 0, 0, 1 };
	const float MATERIAL_SHININESS = 30.f;
	const glm::vec4 FADED_WHITE_RGBA = { 0.3f, 0.3f, 0.3f, 1.f };

	for (auto & vertex : m_vertices)
	{
		glVertex3f(vertex.position.x, vertex.position.y, vertex.position.z);
		glm::vec3 normalEnd = (vertex.position + vertex.normal * -0.2f) ;
		glVertex3f(normalEnd.x, normalEnd.y, normalEnd.z);
	}
	glEnd();
}
