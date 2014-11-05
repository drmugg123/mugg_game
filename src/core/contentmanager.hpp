#ifndef CONTENTMANAGER_HPP
#define CONTENTMANAGER_HPP

#include <iostream>
#include <fstream>
#include <ostream>
#include <string>
#include <vector>
#include <algorithm>

#include <GL/glew.h>

#include <FreeImage.h>

#include "graphics/graphicsdefs.hpp"
#include "graphics/texture.hpp"
#include "graphics/shader.hpp"
#include "graphics/shaderprogram.hpp"
#include "graphics/mesh.hpp"

#include "core/logger.hpp"

namespace mugg {
    namespace core {
        class Engine;
        
        class ContentManager {
            private:
                std::ifstream inStream;
                std::ofstream outStream;
           
                std::vector<graphics::Texture*> textures;
                std::vector<graphics::Shader*> shaders;
                std::vector<graphics::ShaderProgram*> shaderPrograms;

                void DeleteShaderID(GLuint);
                void DeleteTextureID(GLuint);
                void DeleteShaderProgramID(GLuint);

                bool SearchForID(std::vector<GLuint>&, GLuint, int&);
                GLint maxTextureSize;
            
                mugg::core::Engine* parent;
            public:
                ContentManager(mugg::core::Engine* Engine);
                ~ContentManager();

                int GetMaxTextureSize();

                mugg::graphics::Texture* CreateTexture(const std::string&, bool);
                
                bool GetTextFile(const std::string, std::string&);
                
                mugg::graphics::ShaderProgram* CreateShaderProgram();

                mugg::graphics::Shader* CreateShader(mugg::graphics::ShaderType, const std::string&);
        };
    }
}

#endif
