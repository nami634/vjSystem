#include "SceneManager.h"

SceneManager::SceneManager(BasicInfos* g_info) {
    // init store scenes
    stored_scenes["ShaderArt"] = new ShaderArt(g_info);
    stored_scenes["TestScene"] = new TestScene(g_info);
    stored_scenes["VertexArt"] = new VertexArt(g_info);
    stored_scenes["Instancing"] = new Instancing(g_info);
    stored_scenes["FeedbackObject"] = new FeedbackObject(g_info);
    stored_scenes["MeshChain"] = new MeshChain(g_info);
    stored_scenes["RainDrop"] = new RainDrop(g_info);
    stored_scenes["TrainShadow"] = new TrainShadow(g_info);
    stored_scenes["KakkuriText"] = new KakkuriText(g_info);
    stored_scenes["PhysicsLyric"] = new PhysicsLyric(g_info);
    stored_scenes["PointShadowBox"] = new PointShadowBox(g_info);
    stored_scenes["SyphonReceiver"] = new SyphonReceiver(g_info);
    stored_scenes["RayMarching"] = new RayMarching(g_info);
    stored_scenes["ItanoCircus"] = new ItanoCircus(g_info);
    
    // init container[3]
    for (size_t i = 0; i < MAX_SCENES; ++i) {
        scene_containers[i] = new SceneContainer();
    }
    
    initOsc();
}

SceneManager::~SceneManager() {
    
}

void SceneManager::initOsc() {
    // set container scene
    ofxSubscribeOsc(OF_PORT, "/manager/set_scene", [=](const unsigned int i, const string &scene_name) {
        try {
            auto scene = getSceneByName(scene_name);
            setScene(i, scene);
        } catch (...) {
            
        }
        
    });
    
    // set container opacity
    ofxSubscribeOsc(OF_PORT, "/manager/set_opacity", [=](const unsigned int i, const float opacity) {
        scene_containers[i]->setOpacity(opacity);
    });
}

void SceneManager::update() {
    for (auto container : scene_containers) {
        container->update();
    }
}

void SceneManager::drawScene() {
    for (auto container : scene_containers) {
        container->drawScene();
    }
}

void SceneManager::drawFbo() {
    for (auto container : scene_containers) {
        container->drawFbo();
    }
}

void SceneManager::attachUniforms(ofShader& shader) {
    for (int i = 0; i < MAX_SCENES; ++i) {
        if (scene_containers[i]->judgeRender()) {
            shader.setUniformTexture("s_texture" + to_string(i), *scene_containers[i]->getFbo(), i);
            shader.setUniform1f("s_opacity" + to_string(i), scene_containers[i]->getOpacity());
        }
    }
}

ofFbo* SceneManager::getFbo(size_t i) {
    return scene_containers[i]->getFbo();
}

BaseScene * SceneManager::getSceneByName(const string name) {
    if (stored_scenes.count(name)) {
        return stored_scenes[name];
    } else {
        ofLogError() << name << " is not found in stored_scenes.";
        throw "scene not found";
    }
};

void SceneManager::setScene(size_t container_i, BaseScene *scene) {
    if (container_i < MAX_SCENES) {
//        delete scene_containers[container_i]->getScene();

        scene_containers[container_i]->setScene(scene);
        ofLogNotice() << scene->name << " is attatched to container " << container_i << ".";
    } else {
        cout << "SceneContainer index is out of range";
    }
}

void SceneManager::windowResized(glm::vec2 size) {
    for (auto scene : stored_scenes) {
        scene.second->windowResized(size);
    }
    
    for (auto container : scene_containers) {
        container->windowResized(size);
    }
}
