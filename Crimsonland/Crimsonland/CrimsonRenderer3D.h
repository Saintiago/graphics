#pragma once
#include "CrimsonlandLib.h"
#include "IRenderer3D.h"

class CCrimsonProgramContext;

class CCrimsonRenderer3D : public IRenderer3D
{
public:
    CCrimsonRenderer3D(CCrimsonProgramContext &context);
    ~CCrimsonRenderer3D();

    // IRenderer3D interface
    void SetTexCoord2DOffset(size_t offset, size_t stride) override;
    void SetPosition3DOffset(size_t offset, size_t stride) override;
    void SetNormalOffset(size_t offset, size_t stride) override;

private:
    CCrimsonProgramContext &m_context;
    CVertexAttribute m_vertexAttr;
    CVertexAttribute m_normalAttr;
    CVertexAttribute m_texCoordAttr;
};
