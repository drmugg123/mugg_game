#ifndef SHADERPROGRAM_HPP
#define SHADERPROGRAM_HPP

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <memory>

#include <GL/glew.h>

#include "core/fileutils.hpp"
#include "graphics/graphicsdefs.hpp"
#include "graphics/shader.hpp"
#include "graphics/globject.hpp"

namespace mugg {
    namespace graphics {
        class ShaderProgram : public GLObject {
            private:
                bool linked, compiledSuccessfully;

                std::vector<GLuint> shaders;
            public:
                ShaderProgram();
                ~ShaderProgram();
                
                void CreateID();
                void DeleteID();

                void CheckForErrors();
                const char* GetLog();
                bool Link();
                
                bool AttachShader(mugg::graphics::Shader*);

                bool GetCompiledSuccessfully();

                bool Validate();

                int GetAttrib(const std::string&);
                int GetUniform(const std::string&);
                void BindAttrib(GLuint, const std::string&);

                int GetAttachedShaderCount();
                int GetActiveAttribCount();

                void Use();
        };
    }
}

#endif
