#include "MainApplication.h"
#include <OgreRectangle2D.h>
#include <OgreRenderSystem.h>
#include <OgreAxisAlignedBox.h>
#include <OgreMaterial.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_LEFT -720
#define MAX_DOWN -450
#define MAX_UP 450
#define MAX_RIGHT 720
#define MAX_ENEMY 30

int timer = 0;
//uint number = 0;
uint enemyCounter = 0;
uint shotCounter = 0;
//Ogre::String number2 = Ogre::StringConverter::toString(shotCounter + 1); 
 
MainApplication::MainApplication(void)
{
}
 
MainApplication::~MainApplication(void)
{
    mSceneMgr -> destroyQuery(mRaySceneQuery);
    mSceneMgr -> destroyQuery(mRaySceneQuery2);
}
 
//-------------------------------------------------------------------------------------
bool MainApplication::frameRenderingQueued(const Ogre::FrameEvent& evt)
{
    if(mWindow->isClosed())
    return false;

    if(mShutDown)
        return false;

    //Need to capture/update each device
    mKeyboard->capture();
    mMouse->capture();

    Ogre::Vector3 shotsSpeed(500, 0, 0);
    Ogre::Vector3 enemySpeed(-500, rand() % 10 - 5, 0);

    Ogre::Vector3 playerPosition = mSceneMgr -> getSceneNode("NinjaNode")->getPosition();
    Ogre::Vector3 playerDirection = mSceneMgr -> getSceneNode("NinjaNode") -> getOrientation() * Ogre::Vector3(0,0,1);
    //Ogre::Vector3 direction = mSceneMgr -> getSceneNode("NinjaNode") ->_getDerivedOrientation() * Ogre::Vector3::NEGATIVE_UNIT_Z;

    if(playerCollision(playerPosition, playerDirection))
    {
        mSceneMgr -> getSceneNode("NinjaNode") -> setPosition(Ogre::Vector3(0, 0, -500));
    }


    mTrayMgr->frameRenderingQueued(evt);
    mSceneMgr -> getSceneNode("NinjaNode") -> translate(mDirection * evt.timeSinceLastFrame, Ogre::Node::TS_WORLD);
    if(shotCounter)
    {
        //Ogre::String number2 = Ogre::StringConverter::toString(shotCounter); 
        for(int i = 0; i < shots.size(); i++)
        {
            Ogre::Vector3 shotPosition = mSceneMgr -> getSceneNode(shots[i])->getPosition();
            Ogre::Vector3 shotDirection = mSceneMgr -> getSceneNode(shots[i]) -> getOrientation() * Ogre::Vector3(0,0,1);
            if(bulletCollision(shotPosition, shotDirection))
            {
                mSceneMgr -> getSceneNode("NinjaNode") -> setPosition(Ogre::Vector3(0, 0, 0));
                //mDistance = Ogre::Vector3::ZERO;
               //mShutDown = true;
            }
            mSceneMgr -> getSceneNode(shots[i]) -> translate(shotsSpeed * evt.timeSinceLastFrame, Ogre::Node::TS_WORLD);
        }
    }
    if(enemyCounter)
    {
        //Ogre::String number = Ogre::StringConverter::toString(enemyCounter); 
        for(int i = 0; i < enemies.size(); i++)
        {
            mSceneMgr -> getSceneNode(enemies[i]) -> translate(enemySpeed * evt.timeSinceLastFrame, Ogre::Node::TS_WORLD);
        }
    }
    //enemytranslate
    mDistance = mSceneMgr-> getSceneNode("NinjaNode")->getPosition();
    if(mDistance.x >= MAX_RIGHT) 
    {
        mDirection.x = 0;
        mSceneMgr -> getSceneNode("NinjaNode") -> setPosition(Ogre::Vector3(MAX_RIGHT, mDistance.y, mDistance.z));
    }
    if(mDistance.x <= MAX_LEFT)
    {
        mDirection.x = 0;
        mSceneMgr -> getSceneNode("NinjaNode") -> setPosition(Ogre::Vector3(MAX_LEFT, mDistance.y, mDistance.z));
    }
    if(mDistance.y >= MAX_UP) 
    {
        mDirection.y = 0;
        mSceneMgr -> getSceneNode("NinjaNode") -> setPosition(Ogre::Vector3(mDistance.x, MAX_UP, mDistance.z));
    }
    if(mDistance.y <= MAX_DOWN) 
    {
        mDirection.y = 0;
        mSceneMgr -> getSceneNode("NinjaNode") -> setPosition(Ogre::Vector3(mDistance.x, MAX_DOWN, mDistance.z));
    }
    timer++;
    if(timer%20 == 0)
    {
        spawn_enemy();
    }
    return true;
}

void MainApplication::createFrameListener(void)
{
    BaseApplication::createFrameListener();

    mRaySceneQuery = mSceneMgr -> createRayQuery(Ogre::Ray());
    mRaySceneQuery2 = mSceneMgr -> createRayQuery(Ogre::Ray());

    // Populate the camera container
    mCamNode = mCamera->getParentSceneNode();

    // set the rotation and move speed
    mRotate = 0.13;
    mMove = 250;

    mDirection = Ogre::Vector3::ZERO;
    mDistance = Ogre::Vector3::ZERO;
}

bool MainApplication::keyReleased( const OIS::KeyEvent& arg )
{
    switch (arg.key)
    {
        case OIS::KC_UP:
        case OIS::KC_W:
            mDirection.y = 0;
            break;

        case OIS::KC_DOWN:
        case OIS::KC_S:
            mDirection.y = 0;
            break;

        case OIS::KC_LEFT:
        case OIS::KC_A:
            mDirection.x = 0;
            break;

        case OIS::KC_RIGHT:
        case OIS::KC_D:
            mDirection.x = 0;
            break;

        case OIS::KC_SPACE:
            shoot();
            break;

        default:
            break;
        }
        return true;
}

bool MainApplication::keyPressed( const OIS::KeyEvent& arg )
{
    switch (arg.key)
    {
        case OIS::KC_UP:
        case OIS::KC_W:
            mDirection.y = mMove;
            break;

        case OIS::KC_DOWN:
        case OIS::KC_S:
            mDirection.y = -mMove;
            break;

        case OIS::KC_LEFT:
        case OIS::KC_A:
            mDirection.x = -mMove;
            break;

        case OIS::KC_RIGHT:
        case OIS::KC_D:
            mDirection.x = mMove;
            break;

        case OIS::KC_ESCAPE: 
            mShutDown = true;
            break;
        default:
            break;
    }
    return true;
}

void MainApplication::createScene(void)
{
    mSceneMgr->setAmbientLight(Ogre::ColourValue(0.25, 0.25, 0.25));

    // add the ninja
    ent = mSceneMgr->createEntity("Ninja", "ogrehead.mesh");
    node = mSceneMgr->getRootSceneNode()->createChildSceneNode("NinjaNode", Ogre::Vector3(0.0f, 0.0f, 25.0f));
    node->attachObject(ent);
    node->yaw(Ogre::Degree(90));
    node -> setPosition(Ogre::Vector3(0, 0, -500));

    Ogre::SceneNode *sceneNode = mSceneMgr -> getRootSceneNode() -> createChildSceneNode("CanNode", Ogre::Vector3(0, 0, 500));
    sceneNode = sceneNode -> createChildSceneNode("Pitchnode");
    sceneNode -> attachObject(mCamera);

    // create the light
    Ogre::Light *light = mSceneMgr->createLight("Light1");
    light->setType(Ogre::Light::LT_POINT);
    light->setPosition(Ogre::Vector3(250, 150, 250));
    light->setDiffuseColour(Ogre::ColourValue::White);
    light->setSpecularColour(Ogre::ColourValue::White);

    
   // Create the scene node
    //node = mSceneMgr->getRootSceneNode()->createChildSceneNode("CamNode1", Ogre::Vector3(-400, 200, 400));

    // Make it look towards the ninja
    //node -> setPosition(Ogre::Vector3(0,0,1000));

    // Create the pitch node
    //node = node->createChildSceneNode("PitchNode1");
    //node->attachObject(mCamera);
}

void MainApplication::shoot()
{
    
    Ogre::String number2 = Ogre::StringConverter::toString(shotCounter + 1); 
    shots.push_back("manual1_node" + number2);
    /*
    Ogre::Entity* ogreHead = mSceneMgr -> createEntity("ogrehead.mesh");
    Ogre::SceneNode *headNode = mSceneMgr -> getRootSceneNode() -> createChildSceneNode("manual1_node" + number2);
    Ogre::Vector3 pos1(mSceneMgr-> getSceneNode("NinjaNode")->getPosition());
    headNode -> setPosition(pos1);
    headNode -> attachObject(ogreHead);
    */
    Ogre::ManualObject* myManualObject =  mSceneMgr->createManualObject("manual1" + number2); 
    Ogre::SceneNode* myManualObjectNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("manual1_node" + number2); 
     
    // NOTE: The second parameter to the create method is the resource group the material will be added to.
    // If the group you name does not exist (in your resources.cfg file) the library will assert() and your program will crash
    Ogre::MaterialPtr myManualObjectMaterial = Ogre::MaterialManager::getSingleton().create("manual1Material","General"); 
    myManualObjectMaterial->setReceiveShadows(false); 
    myManualObjectMaterial->getTechnique(0)->setLightingEnabled(true); 
    myManualObjectMaterial->getTechnique(0)->getPass(0)->setDiffuse(0,0,1,0); 
    myManualObjectMaterial->getTechnique(0)->getPass(0)->setAmbient(0,0,1); 
    myManualObjectMaterial->getTechnique(0)->getPass(0)->setSelfIllumination(0,0,1); 
    //myManualObjectMaterial->dispose();  // dispose pointer, not the material
     
    Ogre::Vector3 pos1(mSceneMgr-> getSceneNode("NinjaNode")->getPosition());//Get position of player
     
    myManualObject->begin("manual1Material", Ogre::RenderOperation::OT_LINE_LIST); 
    myManualObject->position(pos1.x + 20, pos1.y, pos1.z); 
    myManualObject->position(pos1.x + 50, pos1.y, pos1.z); 
    // etc 
    myManualObject->end(); 
    myManualObjectNode->attachObject(myManualObject);
    shotCounter++;
    
}

void MainApplication::spawn_enemy()
{
   // if(enemyCounter < MAX_ENEMY)
    //{
        Ogre::String number = Ogre::StringConverter::toString(enemyCounter + 1); 
        enemies.push_back("HeadNode" + number);
        Ogre::Entity* ogreHead = mSceneMgr->createEntity("ogrehead.mesh");
        Ogre::SceneNode* headNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("HeadNode" + number);
        Ogre::Vector3 pos1(mSceneMgr-> getSceneNode("NinjaNode")->getPosition());
        headNode -> setPosition(Ogre::Vector3(1000, rand() % 800 - 400, pos1.z));
        headNode -> attachObject(ogreHead);
        headNode->yaw(Ogre::Degree(-90));
        enemyCounter++;
   // }
}

bool MainApplication::playerCollision(const Ogre::Vector3& position, const Ogre::Vector3 &direction)
{

    Ogre::Ray ray(position, direction);
    mRaySceneQuery->setRay(ray);
    Ogre::RaySceneQueryResult &result = mRaySceneQuery->execute();
    Ogre::RaySceneQueryResult::iterator itr;
    for (itr = result.begin(); itr != result.end(); itr++) 
    {
        if(enemyCounter)
        {
            Ogre::String name = itr -> movable -> getName();
            //Ogre::String number = Ogre::StringConverter::toString(enemyCounter); 
            for(int i = 0; i < enemies.size(); i++)
            {
                if (itr -> movable -> getName().compare("Ninja") != 0 && itr -> distance <= 10) 
                {
                    std::cout << name << std::endl;
                    return true;
                }
            }
        }
    }
    return false;
}

bool MainApplication::bulletCollision(const Ogre::Vector3& position, const Ogre::Vector3 &direction)
{
    Ogre::Ray ray(position, direction);
    mRaySceneQuery2->setRay(ray);
    Ogre::RaySceneQueryResult &result = mRaySceneQuery2->execute();
    Ogre::RaySceneQueryResult::iterator itr;
    for (itr = result.begin(); itr != result.end(); itr++) 
    {
        if(enemyCounter)
        {
            Ogre::String name = itr -> movable -> getName();
            //Ogre::String number = Ogre::StringConverter::toString(enemyCounter); 
                if(itr -> movable -> getName() != "Ninja")
                {
                    std::cout << name << std::endl;
                    return true;
                }
        }
    }
    return false;
}
    
#ifdef __cplusplus
extern "C" {
#endif

    int main(int argc, char *argv[])
    {
            srand(time(NULL));
        // Create application object
        MainApplication app;

        try {
            app.go();
        } catch( Ogre::Exception& e ) {
            std::cerr << "An exception has occured: " <<
                e.getFullDescription().c_str() << std::endl;
        }

        return 0;
    }

#ifdef __cplusplus
}
#endif
