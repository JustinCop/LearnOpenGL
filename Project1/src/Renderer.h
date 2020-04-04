#pragma once
#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader.h"


void ValidateOglApiCall(const char *apiCallStr, const char *file, unsigned int line);

#define CALL_GL_API(apiCall) apiCall;\
    ValidateOglApiCall(#apiCall, __FILE__, __LINE__);

class Renderer
{
private:

public:
    Renderer();
    ~Renderer();

    void Clear();
    void Draw(VertexArray *pVao, IndexBuffer *pEbo, Shader *pShader);
};