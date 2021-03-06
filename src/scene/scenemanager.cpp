#include "scene/scenemanager.hpp"

mugg::scene::SceneManager::SceneManager() {
    this->posAttribName = "v_position";
    this->uvAttribName = "v_uv";
    this->normalAttribName = "v_normal";
}
mugg::scene::SceneManager::~SceneManager() {
    for(unsigned int i = 0; i < this->sceneNodes.size(); i++) {
        if(this->sceneNodes[i]) {
            delete this->sceneNodes[i];
        }
    }

    if(this->shaderProgram) {
        this->shaderProgram->DeleteID();
        delete this->shaderProgram;
    }
    if(this->vertexShader) {
        this->vertexShader->DeleteID();
        delete this->vertexShader;
    }
    if(this->fragmentShader) {
        this->fragmentShader->DeleteID();
        delete this->fragmentShader;
    }
}

bool mugg::scene::SceneManager::Initialize() {
    this->vertexShader = new mugg::graphics::Shader(graphics::ShaderType::VertexShader);
    this->fragmentShader = new mugg::graphics::Shader(graphics::ShaderType::FragmentShader);
    this->shaderProgram = new mugg::graphics::ShaderProgram();

    this->vertexShader->CreateID();
    this->fragmentShader->CreateID();
    this->shaderProgram->CreateID();

    this->vertexShader->SetData(this->vertexShaderSrc);
    this->fragmentShader->SetData(this->fragmentShaderSrc);

    if(!this->vertexShader->Compile()) {
        mugg::core::Log(mugg::core::LogLevel::Error, "SceneManager vertex shader failed to compile!");
        return false;
    }

    if(!this->fragmentShader->Compile()) {
        mugg::core::Log(mugg::core::LogLevel::Error, "SceneManager fragment shader failed to compile!");
        return false;
    }

    this->shaderProgram->AttachShader(this->vertexShader);
    this->shaderProgram->AttachShader(this->fragmentShader);
    
    if(!this->shaderProgram->Link()) {
        mugg::core::Log(mugg::core::LogLevel::Error, "SceneManager shaderprogram failed to link!");
        return false;
    }

    this->posLocation    = this->shaderProgram->GetAttrib(this->posAttribName);
    this->uvLocation     = this->shaderProgram->GetAttrib(this->uvAttribName);
    this->normalLocation = this->shaderProgram->GetAttrib(this->normalAttribName);

    return true;
}

mugg::scene::SceneNode* mugg::scene::SceneManager::CreateSceneNode() {
    mugg::scene::SceneNode* node = new mugg::scene::SceneNode(this);

    this->sceneNodes.push_back(node);

    return node;
}
std::size_t mugg::scene::SceneManager::GetNumberOfSceneNodes() {
    return this->sceneNodes.size();
}
bool mugg::scene::SceneManager::GetSceneNodeByIndex(int index, mugg::scene::SceneNode*& out_node) {
    if(index < 0 || index >= this->sceneNodes.size() || this->sceneNodes.empty()) {
        std::cout << "Tried to get scenenode from scenemanager, with out of bounds index!\n";
        return false;
    }

    out_node = this->sceneNodes[index];
    return true;
}

void mugg::scene::SceneManager::SetShaderProgram(mugg::graphics::ShaderProgram* program) {
    this->shaderProgram = program;
    
    this->posLocation = this->shaderProgram->GetAttrib(this->posAttribName);
    this->uvLocation = this->shaderProgram->GetAttrib(this->uvAttribName);
    this->normalLocation = this->shaderProgram->GetAttrib(this->normalAttribName);
}
GLuint mugg::scene::SceneManager::GetShaderProgramID() {
    return this->shaderProgram->GetID();
}

//TODO:PLACEHOLDER
void mugg::scene::SceneManager::Render() {
    for(unsigned int i= 0; i < this->sceneNodes.size(); i++) {
        for(unsigned int u = 0; u < this->sceneNodes[i]->GetNumberOfMeshes(); u++) {
            mugg::graphics::Mesh* mesh = nullptr;
            
            if(!this->sceneNodes[i]->GetMeshByIndex(u, mesh)) {
                std::cout << "ERR: SCENEMANAGER: FAILED TO GET MESH BY INDEX\n";
            }
            
            if(mesh == nullptr) {
                std::cout << "ERR: SCENEMANAGER: We got a mesh but it's still a pointer to nothing\n";
            }

            if(glIsProgram(this->shaderProgram->GetID()) == GL_FALSE) {
                std::cout << "ERR: SCENEMANAGER: Invalid shaderprogram!\n";
            } else {
                glUseProgram(this->shaderProgram->GetID());
            }

            GLint modelMatrixLocation = glGetUniformLocation(this->shaderProgram->GetID(), "u_model");
            GLint projectionMatrixLocation = glGetUniformLocation(this->shaderProgram->GetID(), "u_projection");

            glm::mat4 modelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(this->sceneNodes[i]->GetPosition()));
            glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, glm::value_ptr(modelMatrix));

            //FOV is calulated in radians, currently 90 degrees
            glm::mat4 projectionMatrix = glm::perspective(3.1415f / 2.0f, 4.0f / 3.0f, 0.001f, 100000.0f);
            glUniformMatrix4fv(projectionMatrixLocation, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
            
            glBindVertexArray(mesh->GetVAOID());

            glEnableVertexAttribArray(this->posLocation);
            glEnableVertexAttribArray(this->uvLocation);
            glEnableVertexAttribArray(this->normalLocation);


            glBindBuffer(GL_ARRAY_BUFFER, mesh->GetPositionBufferID());
            glVertexAttribPointer(this->posLocation, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
            
            glBindBuffer(GL_ARRAY_BUFFER, mesh->GetUVBufferID());
            glVertexAttribPointer(this->uvLocation, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
            
            glBindBuffer(GL_ARRAY_BUFFER, mesh->GetNormalBufferID());
            glVertexAttribPointer(this->normalLocation, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
            
            if(mesh->GetTexture() != nullptr) {
                mesh->GetTexture()->Bind();
            }

            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->GetElementBufferID());
            glDrawElements(GL_TRIANGLES, mesh->GetIndexCount(), GL_UNSIGNED_SHORT, nullptr);

            glDisableVertexAttribArray(this->posLocation);
            glDisableVertexAttribArray(this->uvLocation);
            glDisableVertexAttribArray(this->normalLocation);
        }
    }
}
