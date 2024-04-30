//
// Game.h
//

#pragma once

#include "StepTimer.h"
#include <list>
#include "CommonStates.h"
#include "../DirectXTK/Inc/Effects.h" //this clashes with a SDK file so must explitily state it
#include "Keyboard.h"
#include "Mouse.h"
#include "Audio.h"
#include "CMOGO.h"
#include "Projectile.h"
#include "mainMenu.h"
#include "grid.h"
#include "Pathfinding.h"

using std::list;

// Forward declarations
struct GameData;
struct DrawData;
struct DrawData2D;
class GameObject;
class GameObject2D;
class Camera;
class TPSCamera;
class FPSCamera;
class Light;
class Sound;
class Player;
class Projectile;
class mainMenu;
class gird;
class Enemy;
class Trigger;
class TriggeringDoor;

// A basic game implementation that creates a D3D11 device and
// provides a game loop.
class Game
{
public:

    Game() noexcept;
    ~Game() = default;

    Game(Game&&) = default;
    Game& operator= (Game&&) = default;

    Game(Game const&) = delete;
    Game& operator= (Game const&) = delete;

    // Initialization and management
    void Initialize(HWND _window, int _width, int _height);

    // Basic game loop
    void Tick();

    // Messages
    void OnActivated();
    void OnDeactivated();
    void OnSuspending();
    void OnResuming();
    void OnWindowSizeChanged(int _width, int _height);

    // Properties
    void GetDefaultSize( int& _width, int& _height ) const noexcept;

private:

    void Update(DX::StepTimer const& _timer);
    void Render();

    void Clear();
    void Present();

    void CreateDevice();
    void CreateResources();

    void OnDeviceLost();

    // Device resources.
    HWND                                            m_window;
    int                                             m_outputWidth;
    int                                             m_outputHeight;

    D3D_FEATURE_LEVEL                               m_featureLevel;
    Microsoft::WRL::ComPtr<ID3D11Device1>           m_d3dDevice;
    Microsoft::WRL::ComPtr<ID3D11DeviceContext1>    m_d3dContext;

    Microsoft::WRL::ComPtr<IDXGISwapChain1>         m_swapChain;
    Microsoft::WRL::ComPtr<ID3D11RenderTargetView>  m_renderTargetView;
    Microsoft::WRL::ComPtr<ID3D11DepthStencilView>  m_depthStencilView;

    // Rendering loop timer.
    DX::StepTimer                                   m_timer;

    //Scarle Added stuff
    std::shared_ptr <GameData> m_GD = nullptr;			//Data to be shared to all Game Objects as they are ticked
    std::shared_ptr <DrawData> m_DD = nullptr;			//Data to be shared to all 3D Game Objects as they are drawn
    std::shared_ptr <DrawData2D> m_DD2D = nullptr;	    //Data to be passed by game to all 2D Game Objects via Draw 

    //Basic 3D renderers
    std::shared_ptr <Camera> m_cam = nullptr; //principle camera
    std::shared_ptr <TPSCamera> m_TPScam = nullptr;//TPS camB
    std::shared_ptr <FPSCamera> m_FPScam = nullptr;
    std::shared_ptr <Light> m_light = nullptr; //base light

    //required for the CMO model rendering system
    std::unique_ptr <DirectX::CommonStates> m_states = nullptr;
    DirectX::IEffectFactory* m_fxFactory = NULL;

    //basic keyboard and mouse input system
    void ReadInput(); //Get current Mouse and Keyboard states
    std::unique_ptr<DirectX::Keyboard> m_keyboard = nullptr;
    std::unique_ptr<DirectX::Mouse> m_mouse = nullptr;

    std::vector<std::shared_ptr<GameObject>> m_GameObjects; //data structure to hold pointers to the 3D Game Objects
    std::vector< std::shared_ptr<GameObject2D>> m_GameObjects2D; //data structure to hold pointers to the 2D Game Objects 

    std::vector< std::shared_ptr<GameObject2D>> m_MenuGameObjects2D;

    //list<CMOGO*> m_CMOGameObjects; //data structure to hold pointers to all 3D CMO Game Objects
    //list<CMOGO*> m_PhysicsObjects

    std::vector< std::shared_ptr<CMOGO>> m_ColliderObjects;
    std::vector< std::shared_ptr<CMOGO>> m_PhysicsObjects;
    std::vector<std::shared_ptr<Trigger>> m_TriggerObjects;
    std::vector<std::shared_ptr<Trigger>> m_ShootToTriggerObjects;

    std::vector< std::shared_ptr<CMOGO>>m_PlayerProjectiles;
    std::vector < std::shared_ptr<Projectile>> m_Projectiles;
    std::shared_ptr<Player> m_Player = nullptr;

    std::shared_ptr<Enemy>testEnemy = nullptr;

    std::shared_ptr<Trigger>m_goal = nullptr;

    std::shared_ptr<grid> m_mapGrid = nullptr;


    std::shared_ptr<TriggeringDoor> door = nullptr;
    std::shared_ptr<Trigger> doorTrigger = nullptr;

    //main menu
    std::shared_ptr<mainMenu> m_mainMenu = nullptr;
    std::shared_ptr<TextGO2D> m_textStart = nullptr;
    std::shared_ptr<TextGO2D> m_textExit = nullptr;
    bool startSelected = true;

    //game win 
    std::vector< std::shared_ptr<GameObject2D>> m_WinGameObjects2D;
    std::shared_ptr<TextGO2D> m_textWin = nullptr;
    std::shared_ptr<TextGO2D> m_retarttext = nullptr;
    std::shared_ptr<TextGO2D> m_retuenMenuText = nullptr;


    void CheckCollision();
    void CheckProjectileCollision();
    void CheckTriggerCollision();
    void CheckTriggerProjectileCollision();

    void ResetLevel();
                                         
    //sound stuff
	//This uses a simple system, but a better pipeline can be used using Wave Banks
	//See here: https://github.com/Microsoft/DirectXTK/wiki/Creating-and-playing-sounds Using wave banks Section
    std::unique_ptr<DirectX::AudioEngine> m_audioEngine = nullptr;
    std::vector<std::shared_ptr<Sound>>m_Sounds;
};
