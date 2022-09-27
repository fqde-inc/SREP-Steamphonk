#include <iostream>
#include <vector>
#include <fstream>

#include "sre/Texture.hpp"
#include "sre/Renderer.hpp"
#include "sre/Material.hpp"
#include "sre/SDLRenderer.hpp"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtc/matrix_transform.hpp>


using namespace sre;

class SpinningCubeExample {
public:
    SpinningCubeExample(){
        r.init();

        camera.lookAt({0,0,3},{0,0,0},{0,1,0});
        camera.setPerspectiveProjection(60,0.1,100);

        material = Shader::getUnlit()->createMaterial();
        material->setColor({1.0f,1.0f,1.0f,1.0f});
        material->setTexture(Texture::create().withFile("texture/test.png").withGenerateMipmaps(false).build());

        mesh = Mesh::create().withCube().build();

        r.frameRender = [&](){
            render();
        };

        r.startEventLoop();
    }

    void render(){
        auto renderPass = RenderPass::create()
                .withCamera(camera)
                .withClearColor(true, {1, 0, 0, 1})
                .build();
        renderPass.draw(mesh, glm::eulerAngleY(glm::radians((float)i)), material);
        i++;
    }
private:
    SDLRenderer r;
    Camera camera;
    std::shared_ptr<Mesh> mesh;
    std::shared_ptr<Material> material;
    int i=0;
};

int main() {
    new SpinningCubeExample();
    return 0;
}
