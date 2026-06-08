//
// Created by kairav on 6/5/26.
//

#ifndef MAIN_GAME_SHADER_H
#define MAIN_GAME_SHADER_H
#include "common.h"


class Shader{
public:
    unsigned int ID;
    Shader(const std::string&, const std::string&);
    void use() const;
    void setBool(const std::string &name, bool value) const;
    void setInt(const std::string &name, int value) const;
    void setFloat(const std::string &name, float value) const;
    void setMat4(const std::string &name, const glm::mat4 &mat) const;
    void setVec3(const std::string &name, float v1, float v2, float v3) const;
private:
    static void checkCompileErrors(unsigned int, std::string);
};
    


#endif //MAIN_GAME_SHADER_H
