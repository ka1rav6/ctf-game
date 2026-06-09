//
// Created by kairav on 6/9/26.
//

#include "../include/Scene.h"
#include "../include/Entity.h"
Scene::Scene() {
    // auto entity = reg.create();
    

}

Entity Scene::createEntity(){
    auto e = reg.create();
    return Entity(e, this);
}
