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
    roomNode->getMaterial(0).Shininess = 20;
    roomNode->getMaterial(0).EmissiveColor = SColor(255,0,255,255);

    // AMBIENT LIGHT
    sceneManager->setAmbientLight(SColorf(0.2, 0.2, 0.2, 0.0));

    // POINT LIGHT
    ILightSceneNode* diffuseLight = sceneManager->addLightSceneNode (0, vector3df(100,100,200), SColorf(0.4f,0.4f,0.6f,0.0f), 100.0f);
    diffuseLight->getLightData().DiffuseColor = SColorf(0.6, 1.0, 1.0, 1);
    diffuseLight->getLightData().SpecularColor = SColorf(0.6, 0.0, 0.0, 1);


    // PARTICLES
    IParticleSystemSceneNode* particleSystem = sceneManager->addParticleSystemSceneNode(false);
    particleSystem->setPosition(vector3df(0, 100, -20));

    IParticleEmitter* emitter = particleSystem->createBoxEmitter(
        aabbox3d<irr::f32>(-3,0,-3,3,1,3),
        vector3df(0.0f,0.06f,0.0f),
        80,100,
        SColor(0,0,0,0),
        SColor(0,255,255,255),
        600, 1000,
        0,
        dimension2df(6.0f,6.0f),
        dimension2df(8.0f,8.0f)
    );

    particleSystem->setEmitter(emitter);
    emitter->drop();

    particleSystem->setMaterialFlag(EMF_LIGHTING, true);
    particleSystem->setMaterialFlag(EMF_ZWRITE_ENABLE, false);
    particleSystem->setMaterialTexture(0, driver->getTexture("fire.bmp"));
    particleSystem->setMaterialType(EMT_TRANSPARENT_VERTEX_ALPHA);

    IParticleAffector* affector = particleSystem->createFadeOutParticleAffector(SColor(0,0,0,0), 1200);
    particleSystem->addAffector(affector);
    affector->drop();


    wchar_t title[100];

    // render loop
    while(device->run())
    {
        swprintf(title, 100, L"FPS : %d", driver->getFPS());
        device->setWindowCaption(title);
        driver->beginScene (true, true, video::SColor(255,255,255,255));
        sceneManager->drawAll();
        driver->endScene();
    }

    device->drop();

    return 0;
}
