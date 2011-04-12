#include <iostream>
#include <irrlicht/irrlicht.h>


using namespace std;
using namespace irr;
using namespace core;
using namespace scene;
using namespace video;

int main()
{
    IrrlichtDevice *device = createDevice(EDT_OPENGL, dimension2d<u32>(800,600), 32, false, true);

    IVideoDriver *driver = device->getVideoDriver();

    ISceneManager *sceneManager = device->getSceneManager();

    device->getCursorControl ()-> setVisible (false); // curseur invisible


    // Cube

    scene::IMeshSceneNode* cube = sceneManager->addCubeSceneNode(10.0f, 0, -1, vector3df(0.0f, 100.0f, 20.0f));

    cube->setMaterialFlag(EMF_WIREFRAME, true);

    // Camera

    SKeyMap keyMap[5];
    keyMap[0].Action = EKA_MOVE_FORWARD;   // avancer
    keyMap[0].KeyCode = KEY_KEY_Z;
    keyMap[1].Action = EKA_MOVE_BACKWARD;  // reculer
    keyMap[1].KeyCode = KEY_KEY_S;
    keyMap[2].Action = EKA_STRAFE_LEFT;    // a gauche
    keyMap[2].KeyCode = KEY_KEY_Q;
    keyMap[3].Action = EKA_STRAFE_RIGHT;   // a droite
    keyMap[3].KeyCode = KEY_KEY_D;
    keyMap[4].Action = EKA_JUMP_UP;        // saut
    keyMap[4].KeyCode = KEY_SPACE;

    sceneManager->addCameraSceneNodeFPS (0, 100.0f, 0.1f, -1, keyMap, 5);

    // MD2 Character

    //loads the mesh
    IAnimatedMesh* mesh = sceneManager->getMesh("sydney.md2");

    // add the mesh to the scene graph
    IAnimatedMeshSceneNode* sydney = sceneManager->addAnimatedMeshSceneNode(mesh);

    sydney->setPosition(vector3df(0,100,0));

    // animation
    sydney->setMD2Animation(EMAT_STAND);

    // light
    sydney->setMaterialFlag(EMF_LIGHTING, false);

    // texture
    sydney->setMaterialTexture( 0, driver->getTexture("sydney.bmp") );

    // fog
    //sydney->setMaterialFlag(EMF_FOG_ENABLE, true);

    //driver->setFog(SColor(0, 255, 255, 255),EFT_FOG_LINEAR, 20.0f, 100.0f, 0.01f, true, true);

    // ROOM

    IAnimatedMesh *room = sceneManager->getMesh("room.3ds");
    IMeshSceneNode *roomNode =  sceneManager->addMeshSceneNode(room->getMesh(0));
    roomNode->setMaterialFlag(EMF_LIGHTING, true);

    sceneManager->getMeshManipulator()-> makePlanarTextureMapping(room->getMesh(0), 0.004f);
    roomNode->setMaterialTexture(0, driver->getTexture("rockwall.jpg") );

    // AMBIENT LIGHT
    sceneManager->setAmbientLight(SColorf(0.2, 0.2, 0.2, 0.0));

    // DIFFUSE LIGHT
    sceneManager->addLightSceneNode (0, vector3df(0,0,0), SColorf(0.4f,0.4f,0.6f,0.0f), 100.0f);



    // render loop
    while(device->run()) {
    driver->beginScene (true, true, video::SColor(255,255,255,255));
    sceneManager->drawAll();
    driver->endScene();
    }

    device->drop();

    return 0;
}
