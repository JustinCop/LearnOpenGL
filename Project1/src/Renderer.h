#pragma once
void ValidateOglApiCall(const char *apiCallStr, const char *file, unsigned int line);

#define CALL_GL_API(apiCall) apiCall;\
    ValidateOglApiCall(#apiCall, __FILE__, __LINE__);
