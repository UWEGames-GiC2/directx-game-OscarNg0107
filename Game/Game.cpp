//
// Game.cpp
//

#include "pch.h"
#include "Game.h"
#include <time.h>

#include <iostream>

//Scarle Headers
#include "GameData.h"
#include "GameState.h"
#include "DrawData.h"
#include "DrawData2D.h"
#include "ObjectList.h"

#include "CMOGO.h"
#include <DirectXCollision.h>
#include "Collision.h"
#include "GridLocation.h"

extern void ExitGame() noexcept;

using namespace DirectX;

using Microsoft::WRL::ComPtr;

Game::Game() noexcept :
    m_window(nullptr),
    m_outputWidth(1920),
    m_outputHeight(600),
    m_featureLevel(D3D_FEATURE_LEVEL_11_0)
{
}

// Initialize the Direct3D resources required to run.
void Game::Initialize(HWND _window, int _width, int _height)
{
    m_window = _window;
    m_outputWidth = std::max(_width, 1);
    m_outputHeight = std::max(_height, 1);

    CreateDevice();

    CreateResources();

    // TODO: Change the timer settings if you want something other than the default variable timestep mode.
    // e.g. for 60 FPS fixed timestep update logic, call:
    /*
    m_timer.SetFixedTimeStep(true);
    m_timer.SetTargetElapsedSeconds(1.0 / 60);
    */

    //seed the random number generator
    srand((UINT)time(NULL));

    //set up keyboard and mouse system
    //documentation here: https://github.com/microsoft/DirectXTK/wiki/Mouse-and-keyboard-input
    m_keyboard = std::make_unique<Keyboard>();
    m_mouse = std::make_unique<Mouse>();
    m_mouse->SetWindow(_window);
    m_mouse->SetMode(Mouse::MODE_RELATIVE);
    //Hide the mouse pointer
    ShowCursor(false);

    //create GameData struct and populate its pointers
    m_GD = std::make_shared<GameData>();
    m_GD->m_GS = GS_MAINMENU;
    m_GD->m_CamUsing = GS_PLAY_MAIN_CAM;

    //set up systems for 2D rendering
    m_DD2D = std::make_shared<DrawData2D>();
    m_DD2D->m_Sprites.reset(new SpriteBatch(m_d3dContext.Get()));
    m_DD2D->m_Font.reset(new SpriteFont(m_d3dDevice.Get(), L"..\\Assets\\italic.spritefont"));
    m_states = std::make_unique<CommonStates>(m_d3dDevice.Get());

    //set up DirectXTK Effects system
    m_fxFactory = new EffectFactory(m_d3dDevice.Get());
    //Tell the fxFactory to look to the correct build directory to pull stuff in from
    ((EffectFactory*)m_fxFactory)->SetDirectory(L"..\\Assets");
    //init render system for VBGOs
    VBGO::Init(m_d3dDevice.Get());

    //set audio system
    AUDIO_ENGINE_FLAGS eflags = AudioEngine_Default;
#ifdef _DEBUG
    eflags = eflags | AudioEngine_Debug;
#endif
    m_audioEngine = std::make_unique<AudioEngine>(eflags);

    //create a set of dummy things to show off the engine

    //create a base light
    m_light = std::make_unique<Light>(Vector3(0.0f, 100.0f, 160.0f), Color(1.0f, 1.0f, 1.0f, 1.0f), Color(0.4f, 0.1f, 0.1f, 1.0f));
    m_GameObjects.push_back(m_light);

    //find how big my window is to correctly calculate my aspect ratio
    float AR = (float)_width / (float)_height;

    //example basic 3D stuff

    m_mapGrid = std::make_shared<grid>(20.0f, 20.0f, 20, 20);
    std::cout << m_mapGrid->m_gridmap[10].GetCentre().x << std::endl;


    for(int i=0 ; i< m_mapGrid->m_walls.size(); ++i)
    {
        std::shared_ptr<Terrain>wall = std::make_shared<Terrain>("cube3", m_d3dDevice.Get(), m_fxFactory, m_mapGrid->m_walls[i].GetCentre(), 0.0f, 0.0f, 0.0f, Vector3(0.2f, 2.0f, 0.2f));
        m_GameObjects.push_back(wall);
        m_ColliderObjects.push_back(wall);
    }

    for (int i = 0; i < m_mapGrid->m_floor.size(); ++i)
    {
        std::shared_ptr<Terrain>floor = std::make_shared<Terrain>("cube3", m_d3dDevice.Get(), m_fxFactory, m_mapGrid->m_floor[i].GetCentre(), 0.0f, 0.0f, 0.0f, Vector3(0.2f, 0.2f, 0.2f));
        m_GameObjects.push_back(floor);
        m_ColliderObjects.push_back(floor);
    }

    std::shared_ptr<Terrain>floor = std::make_shared<Terrain>("cube3", m_d3dDevice.Get(), m_fxFactory, m_mapGrid->GetCentre(), 0.0f, 0.0f, 0.0f, Vector3(10.0f, 0.1f, 10.0f));
    m_GameObjects.push_back(floor);
    m_ColliderObjects.push_back(floor);

 
    std::shared_ptr<MovingObjects>Shuriken1 = std::make_shared<MovingObjects>("Shuriken2_low", m_d3dDevice.Get(), m_fxFactory, Vector3(m_mapGrid->m_gridmap[650].GetPos().x, 40.0f, m_mapGrid->m_gridmap[650].GetPos().z), 0.0f, 0.0f, 0.0f, Vector3(0.03f, 0.03f, 0.03f), Vector3(320.0f, 0.0f, 0.0f));
    m_GameObjects.push_back(Shuriken1);
    m_DamageObjects.push_back(Shuriken1);

    for(size_t i =0; i < 10; i++)
    {
        std::shared_ptr<Projectile> pProjectile = std::make_shared<Projectile>("Shuriken_low", m_d3dDevice.Get(), m_fxFactory, 3.0f, 40.0f);
        pProjectile->SetActive(false);
        pProjectile->SetScale(0.01f);
        m_GameObjects.push_back(pProjectile);
        m_Projectiles.push_back(pProjectile);
    }

    /*std::shared_ptr<Trigger> testend = std::make_shared<Trigger>("cube3", m_d3dDevice.Get(), m_fxFactory, Vector3(m_mapGrid->m_gridmap[1579].GetCentre().x, 20.0f, m_mapGrid->m_gridmap[1579].GetCentre().z), 0.0f, 0.0f, 0.0f, Vector3(1.0f, 1.0f, 1.0f));
    m_TriggerObjects.push_back(testend);
    m_GameObjects.push_back(testend);*/

    m_goal = std::make_shared<Trigger>("cube3", m_d3dDevice.Get(), m_fxFactory, Vector3(m_mapGrid->m_gridmap[32].GetPos().x, 20.0f, m_mapGrid->m_gridmap[32].GetPos().z), 0.0f, 0.0f, 0.0f, Vector3(1.0f, 1.0f, 1.0f));
    m_GameObjects.push_back(m_goal);

    door = std::make_shared<TriggeringDoor>(m_d3dDevice.Get(), m_fxFactory, Vector3(m_mapGrid->m_gridmap[1260].GetPos().x, 40.0f, m_mapGrid->m_gridmap[1260].GetPos().z), 0.0f, 0.0f, 0.0f, Vector3(1.0f, 1.0f, 1.0f), false);
    door->SetYaw(door->GetYaw() + 80.0f);
    std::cout << door->GetYaw();
    m_GameObjects.push_back(door);
    m_ColliderObjects.push_back(door);

    doorTrigger = std::make_shared<Trigger>("cube3", m_d3dDevice.Get(), m_fxFactory, Vector3(m_mapGrid->m_gridmap[1301].GetPos().x, 50.0f, m_mapGrid->m_gridmap[1301].GetPos().z), 0.0f, 0.0f, 0.0f, Vector3(0.1f, 0.1f, 0.1f));
    //m_ShootToTriggerObjects.push_back(doorTrigger);
    m_GameObjects.push_back(doorTrigger);

    //create a base camera
    m_cam = std::make_shared<Camera>(0.25f * XM_PI, AR, 1.0f, 10000.0f, Vector3::UnitY, Vector3::Zero);
    m_cam->SetPos(Vector3(0.0f, 200.0f, 200.0f));
    m_GameObjects.push_back(m_cam);

    //add Player
    m_Player = std::make_shared<Player>("botan", m_d3dDevice.Get(), m_fxFactory, AR, m_mapGrid->GetTileWidth(), m_mapGrid->GetTileDepth());
    m_GameObjects.push_back(m_Player);
    std::cout << m_Player->GetPos().z;
    //m_PhysicsObjects.push_back(m_Player);
    m_Player->projectiles = m_Projectiles;

    camtarget = std::make_shared<CamTarget>("table", m_d3dDevice.Get(), m_fxFactory, 10.0f, 10.0f, m_Player);
    m_GameObjects.push_back(camtarget);
    m_Player->AddCamTargetChild(camtarget);
    //add a secondary camera
    m_TPScam = std::make_shared<TPSCamera>(0.25f * XM_PI, AR, 1.0f, 10000.0f, m_Player, Vector3::UnitY, Vector3(0.0f, 10.0f, 30.0f));
    m_GameObjects.push_back(m_TPScam);

    m_FPScam = std::make_shared<FPSCamera>(0.25f * XM_PI, AR, 1.0f, 10000.0f, camtarget,m_Player, Vector3::UnitY, Vector3(0.0f, 0.0f, 0.1f));
    m_GameObjects.push_back(m_FPScam);
    m_Player->AddCameraChild(m_FPScam);
   
    //create DrawData struct and populate its pointers
    m_DD = std::make_unique<DrawData>();
    m_DD->m_pd3dImmediateContext = nullptr;
    m_DD->m_states = m_states.get();
    m_DD->m_cam = m_FPScam.get();
    m_DD->m_light = m_light.get();

    std::shared_ptr<ImageGO2D> Crosshair = std::make_shared<ImageGO2D>("Crosshair", m_d3dDevice.Get());
    Crosshair->SetPos(Vector2(m_outputWidth / 2, m_outputHeight / 2));
    Crosshair->SetScale(0.2f);
    m_GameObjects2D.push_back(Crosshair);

    //example basic 2D stuff
    /*std::shared_ptr<ImageGO2D> logo = std::make_shared<ImageGO2D>("logo_small", m_d3dDevice.Get());
    logo->SetPos(200.0f * Vector2::One);
    m_GameObjects2D.push_back(logo);
    std::shared_ptr<ImageGO2D> bug_test = std::make_shared<ImageGO2D>("bug_test", m_d3dDevice.Get());
    bug_test->SetPos(300.0f * Vector2::One);
    m_GameObjects2D.push_back(bug_test);*/

   /* std::shared_ptr<TextGO2D> text = std::make_shared<TextGO2D>("SBTest Text");
    text->SetPos(Vector2(100, 10));
    text->SetColour(Color((float*)&Colors::Yellow));
    m_GameObjects2D.push_back(text);*/


    std::shared_ptr<TextGO2D> textTitle = std::make_shared<TextGO2D>("Title");
    textTitle->SetScale(5.0f);
    textTitle->SetPos(Vector2(m_outputWidth / 2 - XMVectorGetX(m_DD2D->m_Font.get()->MeasureString(textTitle->GetString().data(), true)) * 5 / 2, m_outputHeight / 5));
    textTitle->SetColour(Color((float*)&Colors::Black));
    m_MenuGameObjects2D.push_back(textTitle);

    m_textStart = std::make_shared<TextGO2D>("Start");
    m_textStart->SetScale(2.0f);
    m_textStart->SetPos(Vector2(m_outputWidth / 2 - XMVectorGetX(m_DD2D->m_Font.get()->MeasureString(m_textStart->GetString().data(), true)) * 2 / 2, m_outputHeight / 2));
    m_textStart->SetColour(Color((float*)&Colors::Black));
    m_MenuGameObjects2D.push_back(m_textStart);

    m_textExit = std::make_shared<TextGO2D>("Exit");
    m_textExit->SetScale(2.0f);
    m_textExit->SetPos(Vector2(m_outputWidth / 2 - XMVectorGetX(m_DD2D->m_Font.get()->MeasureString(m_textExit->GetString().data(), true)) * 2 / 2, m_outputHeight / 2 - -XMVectorGetX(m_DD2D->m_Font.get()->MeasureString(m_textStart->GetString().data(), true))));
    m_textExit->SetColour(Color((float*)&Colors::Black));
    m_MenuGameObjects2D.push_back(m_textExit);


    //Game Win

    m_textWin = std::make_shared<TextGO2D>("You Win!");
    m_textWin->SetScale(5.0f);
    m_textWin->SetPos(Vector2(m_outputWidth / 2 - XMVectorGetX(m_DD2D->m_Font.get()->MeasureString(m_textWin->GetString().data(), true)) * 5 / 2, m_outputHeight / 5));
    m_textWin->SetColour(Color((float*)&Colors::Black));
    m_WinGameObjects2D.push_back(m_textWin);

    m_retarttext = std::make_shared<TextGO2D>("Restart");
    m_retarttext->SetScale(2.0f);
    m_retarttext->SetPos(Vector2(m_outputWidth / 2 - XMVectorGetX(m_DD2D->m_Font.get()->MeasureString(m_retarttext->GetString().data(), true)) * 2 / 2, m_outputHeight / 2));
    m_retarttext->SetColour(Color((float*)&Colors::Black));
    m_WinGameObjects2D.push_back(m_retarttext);

    m_retuenMenuText = std::make_shared<TextGO2D>("Back To Menu");
    m_retuenMenuText->SetScale(2.0f);
    m_retuenMenuText->SetPos(Vector2(m_outputWidth / 2 - XMVectorGetX(m_DD2D->m_Font.get()->MeasureString(m_retuenMenuText->GetString().data(), true)) * 2 / 2, m_outputHeight / 2 - -XMVectorGetX(m_DD2D->m_Font.get()->MeasureString(m_retarttext->GetString().data(), true))));
    m_retuenMenuText->SetColour(Color((float*)&Colors::Black));
    m_WinGameObjects2D.push_back(m_retuenMenuText);

    //Test Sounds
    std::shared_ptr <Loop> loop = std::make_shared<Loop>(m_audioEngine.get(), "NightAmbienceSimple_02");
    loop->SetVolume(0.1f);
    loop->Play();
    m_Sounds.push_back(loop);

    /*TestSound* TS = new TestSound(m_audioEngine.get(), "Explo1");
    m_Sounds.push_back(TS);*/
    m_mainMenu = std::make_shared<mainMenu>(m_DD2D);
    m_mainMenu->init(m_outputWidth, m_outputHeight);

}

// Executes the basic game loop.
void Game::Tick()
{
    m_timer.Tick([&]()
    {
        Update(m_timer);
    });

    Render();
}

// Updates the world.
void Game::Update(DX::StepTimer const& _timer)
{
    float elapsedTime = float(_timer.GetElapsedSeconds());
    m_GD->m_dt = elapsedTime;

    //this will update the audio engine but give us chance to do somehting else if that isn't working
    if (!m_audioEngine->Update())
    {
        if (m_audioEngine->IsCriticalError())
        {
            // We lost the audio device!
        }
    }
    else
    {
        //update sounds playing
        for (std::vector<std::shared_ptr<Sound>>::iterator it = m_Sounds.begin(); it != m_Sounds.end(); it++)
        {
            (*it)->Tick(m_GD.get());
        }
    }

    ReadInput();

    if(m_GD->m_GS == GS_MAINMENU)
    {
        /* for (std::vector<std::shared_ptr<GameObject2D>>::iterator it = m_mainMenu->GetGameObject2DList().begin(); it != m_mainMenu->GetGameObject2DList().end(); it++)
        {
        (*it)->Tick(m_GD.get());
        }*/

        for (std::vector<std::shared_ptr<GameObject2D>>::iterator it = m_MenuGameObjects2D.begin(); it != m_MenuGameObjects2D.end(); it++)
        {
            (*it)->Tick(m_GD.get());
        }

        if (m_GD->m_KBS.Up || m_GD->m_KBS.Down)
        {
            startSelected = !startSelected;
        }

        if(startSelected)
        {
            m_textStart->SetColour(Color((float*)&Colors::Red));
            m_textExit->SetColour(Color((float*)&Colors::Black));
        }

        else
        {
            m_textStart->SetColour(Color((float*)&Colors::Black));
            m_textExit->SetColour(Color((float*)&Colors::Red));
        }
        if (m_GD->m_KBS_tracker.pressed.Enter)
        {
            if(startSelected)
            {
                ResetLevel();
                m_GD->m_GS = GS_PLAY_MAIN_CAM;
            }
            else
            {
                ExitGame();
            }
        }

    }

    else if(m_GD->m_GS == GS_PLAY_MAIN_CAM)
    {
       
    //upon space bar switch camera state
        //see docs here for what's going on: https://github.com/Microsoft/DirectXTK/wiki/Keyboard
        if (m_GD->m_KBS_tracker.pressed.C)
        {
            if (m_GD->m_CamUsing == GS_PLAY_MAIN_CAM)
            {
                m_GD->m_CamUsing = GS_PLAY_TPS_CAM;
            }
            else
            {
               m_GD->m_CamUsing = GS_PLAY_MAIN_CAM;
            }
        }

        //std::cout << "X" << m_Player->GetGridPos().x << std::endl;
        //std::cout << "Y" << m_Player->GetGridPos().y << std::endl;
        //update all objects
        for (std::vector<std::shared_ptr<GameObject>>::iterator it = m_GameObjects.begin(); it != m_GameObjects.end(); it++)
        {
            (*it)->Tick(m_GD.get()); 
        }
        for (std::vector<std::shared_ptr<GameObject2D>>::iterator it = m_GameObjects2D.begin(); it != m_GameObjects2D.end(); it++)
        {
            (*it)->Tick(m_GD.get());
        }
    
        for (std::vector<std::shared_ptr<Projectile>>::iterator it = m_Projectiles.begin(); it != m_Projectiles.end(); it++)
        {
            (*it)->Tick(m_GD.get());
        }

        if(m_Player.get()->Intersects(*m_goal))
        {
            m_GD->m_GS = GS_GAME_WIN;
            ResetLevel();
        }
        CheckCollision();
        CheckProjectileCollision();
        CheckTriggerCollision();
        CheckDamagingCollision();
    }

    else if(m_GD->m_GS == GS_GAME_WIN)
    {
        if (m_GD->m_KBS.Up || m_GD->m_KBS.Down)
        {
            startSelected = !startSelected;
        }

        if (startSelected)
        {
            m_retarttext->SetColour(Color((float*)&Colors::Red));
            m_retuenMenuText->SetColour(Color((float*)&Colors::Black));
        }

        else
        {
            m_retarttext->SetColour(Color((float*)&Colors::Black));
            m_retuenMenuText->SetColour(Color((float*)&Colors::Red));
        }
        if (m_GD->m_KBS_tracker.pressed.Enter)
        {
            if (startSelected)
            {
                ResetLevel();
                m_GD->m_GS = GS_PLAY_MAIN_CAM;
            }
            else
            {
                m_GD->m_GS = GS_MAINMENU;
            }
        }
    }

   
}

// Draws the scene.
void Game::Render()
{
    // Don't try to render anything before the first Update.
    if (m_timer.GetFrameCount() == 0)
    {
        return;
    }

    Clear();
    
    //set immediate context of the graphics device
    m_DD->m_pd3dImmediateContext = m_d3dContext.Get();

    //set which camera to be used
    m_DD->m_cam = m_FPScam.get();
    if (m_GD->m_CamUsing == GS_PLAY_TPS_CAM)
    {
        m_DD->m_cam = m_TPScam.get();
    }

    //update the constant buffer for the rendering of VBGOs
    VBGO::UpdateConstantBuffer(m_DD.get());

    if(m_GD->m_GS == GS_MAINMENU)
    {   
       /* for (std::vector<std::shared_ptr<GameObject2D>>::iterator it = m_mainMenu->GetGameObject2DList().begin(); it != m_mainMenu->GetGameObject2DList().end(); it++)
        {
            if ((*it).get()->IsActive())
            {
                (*it)->Draw(m_DD2D.get());
            }
        }*/
        m_DD2D->m_Sprites->Begin(SpriteSortMode_Deferred, m_states->NonPremultiplied());
        for (std::vector<std::shared_ptr<GameObject2D>>::iterator it = m_MenuGameObjects2D.begin(); it != m_MenuGameObjects2D.end(); it++)
        {
            if ((*it).get()->IsActive())
            {
                (*it)->Draw(m_DD2D.get());
            }
        }
        m_DD2D->m_Sprites->End();
    }

    else if (m_GD->m_GS == GS_PLAY_MAIN_CAM)
    {
     //Draw 3D Game Obejects
        for (std::vector<std::shared_ptr<GameObject>>::iterator it = m_GameObjects.begin(); it != m_GameObjects.end(); it++)
        {
            if((*it).get()->GetIsActive())
            {
                (*it)->Draw(m_DD.get());
            } 
        }

        // Draw sprite batch stuff 
        m_DD2D->m_Sprites->Begin(SpriteSortMode_Deferred, m_states->NonPremultiplied());
        for (std::vector<std::shared_ptr<GameObject2D>>::iterator it = m_GameObjects2D.begin(); it != m_GameObjects2D.end(); it++)
        {
            if ((*it).get()->IsActive())
            {
                (*it)->Draw(m_DD2D.get());
            }
        }
    
        m_DD2D->m_Sprites->End();
    }

    else if (m_GD->m_GS == GS_GAME_WIN)
    {
        m_DD2D->m_Sprites->Begin(SpriteSortMode_Deferred, m_states->NonPremultiplied());
        for (std::vector<std::shared_ptr<GameObject2D>>::iterator it = m_WinGameObjects2D.begin(); it != m_WinGameObjects2D.end(); it++)
        {
            if ((*it).get()->IsActive())
            {
                (*it)->Draw(m_DD2D.get());
            }
        }
        m_DD2D->m_Sprites->End();
    }
  


    //drawing text screws up the Depth Stencil State, this puts it back again!
    m_d3dContext->OMSetDepthStencilState(m_states->DepthDefault(), 0);

    Present();
}

// Helper method to clear the back buffers.
void Game::Clear()
{
    // Clear the views.
    m_d3dContext->ClearRenderTargetView(m_renderTargetView.Get(), Colors::CornflowerBlue);
    m_d3dContext->ClearDepthStencilView(m_depthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

    m_d3dContext->OMSetRenderTargets(1, m_renderTargetView.GetAddressOf(), m_depthStencilView.Get());

    // Set the viewport.
    CD3D11_VIEWPORT viewport(0.0f, 0.0f, static_cast<float>(m_outputWidth), static_cast<float>(m_outputHeight));
    m_d3dContext->RSSetViewports(1, &viewport);
}

// Presents the back buffer contents to the screen.
void Game::Present()
{
    // The first argument instructs DXGI to block until VSync, putting the application
    // to sleep until the next VSync. This ensures we don't waste any cycles rendering
    // frames that will never be displayed to the screen.
    HRESULT hr = m_swapChain->Present(1, 0);

    // If the device was reset we must completely reinitialize the renderer.
    if (hr == DXGI_ERROR_DEVICE_REMOVED || hr == DXGI_ERROR_DEVICE_RESET)
    {
        OnDeviceLost();
    }
    else
    {
        DX::ThrowIfFailed(hr);
    }
}

// Message handlers
void Game::OnActivated()
{
    // TODO: Game is becoming active window.
}

void Game::OnDeactivated()
{
    // TODO: Game is becoming background window.
}

void Game::OnSuspending()
{
    // TODO: Game is being power-suspended (or minimized).
}

void Game::OnResuming()
{
    m_timer.ResetElapsedTime();

    // TODO: Game is being power-resumed (or returning from minimize).
}

void Game::OnWindowSizeChanged(int _width, int _height)
{
    m_outputWidth = std::max(_width, 1);
    m_outputHeight = std::max(_height, 1);

    CreateResources();

    // TODO: Game window is being resized.
}

// Properties
void Game::GetDefaultSize(int& _width, int& _height) const noexcept
{
    // TODO: Change to desired default window size (note minimum size is 320x200).
    _width = 1920;
    _height = 1080;
}

// These are the resources that depend on the device.
void Game::CreateDevice()
{
    UINT creationFlags = 0;

#ifdef _DEBUG
    //creationFlags |= D3D11_CREATE_DEVICE_DEBUG;
    //something missing on the machines in 2Q28
    //this should work!
#endif

    static const D3D_FEATURE_LEVEL featureLevels [] =
    {
        // TODO: Modify for supported Direct3D feature levels
        D3D_FEATURE_LEVEL_11_1,
        D3D_FEATURE_LEVEL_11_0,
        D3D_FEATURE_LEVEL_10_1,
        D3D_FEATURE_LEVEL_10_0,
        D3D_FEATURE_LEVEL_9_3,
        D3D_FEATURE_LEVEL_9_2,
        D3D_FEATURE_LEVEL_9_1,
    };

    // Create the DX11 API device object, and get a corresponding context.
    ComPtr<ID3D11Device> device;
    ComPtr<ID3D11DeviceContext> context;
    DX::ThrowIfFailed(D3D11CreateDevice(
        nullptr,                            // specify nullptr to use the default adapter
        D3D_DRIVER_TYPE_HARDWARE,
        nullptr,
        creationFlags,
        featureLevels,
        static_cast<UINT>(std::size(featureLevels)),
        D3D11_SDK_VERSION,
        device.ReleaseAndGetAddressOf(),    // returns the Direct3D device created
        &m_featureLevel,                    // returns feature level of device created
        context.ReleaseAndGetAddressOf()    // returns the device immediate context
        ));

#ifndef NDEBUG
    ComPtr<ID3D11Debug> d3dDebug;
    if (SUCCEEDED(device.As(&d3dDebug)))
    {
        ComPtr<ID3D11InfoQueue> d3dInfoQueue;
        if (SUCCEEDED(d3dDebug.As(&d3dInfoQueue)))
        {
#ifdef _DEBUG
            d3dInfoQueue->SetBreakOnSeverity(D3D11_MESSAGE_SEVERITY_CORRUPTION, true);
            d3dInfoQueue->SetBreakOnSeverity(D3D11_MESSAGE_SEVERITY_ERROR, true);
#endif
            D3D11_MESSAGE_ID hide [] =
            {
                D3D11_MESSAGE_ID_SETPRIVATEDATA_CHANGINGPARAMS,
                // TODO: Add more message IDs here as needed.
            };
            D3D11_INFO_QUEUE_FILTER filter = {};
            filter.DenyList.NumIDs = static_cast<UINT>(std::size(hide));
            filter.DenyList.pIDList = hide;
            d3dInfoQueue->AddStorageFilterEntries(&filter);
        }
    }
#endif

    DX::ThrowIfFailed(device.As(&m_d3dDevice));
    DX::ThrowIfFailed(context.As(&m_d3dContext));

    // TODO: Initialize device dependent objects here (independent of window size).
}

// Allocate all memory resources that change on a window SizeChanged event.
void Game::CreateResources()
{
    // Clear the previous window size specific context.
    ID3D11RenderTargetView* nullViews [] = { nullptr };
    m_d3dContext->OMSetRenderTargets(static_cast<UINT>(std::size(nullViews)), nullViews, nullptr);
    m_renderTargetView.Reset();
    m_depthStencilView.Reset();
    m_d3dContext->Flush();

    const UINT backBufferWidth = static_cast<UINT>(m_outputWidth);
    const UINT backBufferHeight = static_cast<UINT>(m_outputHeight);
    const DXGI_FORMAT backBufferFormat = DXGI_FORMAT_B8G8R8A8_UNORM;
    const DXGI_FORMAT depthBufferFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;
    constexpr UINT backBufferCount = 2;

    // If the swap chain already exists, resize it, otherwise create one.
    if (m_swapChain)
    {
        HRESULT hr = m_swapChain->ResizeBuffers(backBufferCount, backBufferWidth, backBufferHeight, backBufferFormat, 0);

        if (hr == DXGI_ERROR_DEVICE_REMOVED || hr == DXGI_ERROR_DEVICE_RESET)
        {
            // If the device was removed for any reason, a new device and swap chain will need to be created.
            OnDeviceLost();

            // Everything is set up now. Do not continue execution of this method. OnDeviceLost will reenter this method 
            // and correctly set up the new device.
            return;
        }
        else
        {
            DX::ThrowIfFailed(hr);
        }
    }
    else
    {
        // First, retrieve the underlying DXGI Device from the D3D Device.
        ComPtr<IDXGIDevice1> dxgiDevice;
        DX::ThrowIfFailed(m_d3dDevice.As(&dxgiDevice));

        // Identify the physical adapter (GPU or card) this device is running on.
        ComPtr<IDXGIAdapter> dxgiAdapter;
        DX::ThrowIfFailed(dxgiDevice->GetAdapter(dxgiAdapter.GetAddressOf()));

        // And obtain the factory object that created it.
        ComPtr<IDXGIFactory2> dxgiFactory;
        DX::ThrowIfFailed(dxgiAdapter->GetParent(IID_PPV_ARGS(dxgiFactory.GetAddressOf())));

        // Create a descriptor for the swap chain.
        DXGI_SWAP_CHAIN_DESC1 swapChainDesc = {};
        swapChainDesc.Width = backBufferWidth;
        swapChainDesc.Height = backBufferHeight;
        swapChainDesc.Format = backBufferFormat;
        swapChainDesc.SampleDesc.Count = 1;
        swapChainDesc.SampleDesc.Quality = 0;
        swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
        swapChainDesc.BufferCount = backBufferCount;

        DXGI_SWAP_CHAIN_FULLSCREEN_DESC fsSwapChainDesc = {};
        fsSwapChainDesc.Windowed = TRUE;

        // Create a SwapChain from a Win32 window.
        DX::ThrowIfFailed(dxgiFactory->CreateSwapChainForHwnd(
            m_d3dDevice.Get(),
            m_window,
            &swapChainDesc,
            &fsSwapChainDesc,
            nullptr,
            m_swapChain.ReleaseAndGetAddressOf()
            ));

        // This template does not support exclusive fullscreen mode and prevents DXGI from responding to the ALT+ENTER shortcut.
        DX::ThrowIfFailed(dxgiFactory->MakeWindowAssociation(m_window, DXGI_MWA_NO_ALT_ENTER));
    }

    // Obtain the backbuffer for this window which will be the final 3D rendertarget.
    ComPtr<ID3D11Texture2D> backBuffer;
    DX::ThrowIfFailed(m_swapChain->GetBuffer(0, IID_PPV_ARGS(backBuffer.GetAddressOf())));

    // Create a view interface on the rendertarget to use on bind.
    DX::ThrowIfFailed(m_d3dDevice->CreateRenderTargetView(backBuffer.Get(), nullptr, m_renderTargetView.ReleaseAndGetAddressOf()));

    // Allocate a 2-D surface as the depth/stencil buffer and
    // create a DepthStencil view on this surface to use on bind.
    CD3D11_TEXTURE2D_DESC depthStencilDesc(depthBufferFormat, backBufferWidth, backBufferHeight, 1, 1, D3D11_BIND_DEPTH_STENCIL);

    ComPtr<ID3D11Texture2D> depthStencil;
    DX::ThrowIfFailed(m_d3dDevice->CreateTexture2D(&depthStencilDesc, nullptr, depthStencil.GetAddressOf()));

    CD3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc(D3D11_DSV_DIMENSION_TEXTURE2D);
    DX::ThrowIfFailed(m_d3dDevice->CreateDepthStencilView(depthStencil.Get(), &depthStencilViewDesc, m_depthStencilView.ReleaseAndGetAddressOf()));

    // TODO: Initialize windows-size dependent objects here.
}

void Game::OnDeviceLost()
{
    // TODO: Add Direct3D resource cleanup here.

    m_depthStencilView.Reset();
    m_renderTargetView.Reset();
    m_swapChain.Reset();
    m_d3dContext.Reset();
    m_d3dDevice.Reset();

    CreateDevice();

    CreateResources();
}

void Game::ReadInput()
{
    m_GD->m_KBS = m_keyboard->GetState();
    m_GD->m_KBS_tracker.Update(m_GD->m_KBS);
    //quit game on hiting escape
    if (m_GD->m_KBS.Escape)
    {
        ExitGame();
    }

    m_GD->m_MS = m_mouse->GetState();

    //lock the cursor to the centre of the window
    RECT window;
    GetWindowRect(m_window, &window);
    SetCursorPos((window.left + window.right) >> 1, (window.bottom + window.top) >> 1);
}

void Game::CheckCollision()
{
    float dis =0.0f;
    int collision_count = 0;
    for (int j = 0; j < m_ColliderObjects.size(); j++) {

        if(m_Player->Intersects((*m_ColliderObjects[j])))
        {
            XMFLOAT3 eject_vect = Collision::ejectionCMOGO(*m_Player, *m_ColliderObjects[j]);
            auto pos = m_Player->GetPos();
            m_Player->SetPos(pos - eject_vect);


            // if the the hit vect is smaller than 0, which means the player is colliding the ground that is facing up
            if (eject_vect.y < 0)
            {
                collision_count++;
                m_Player->SetJumpCount(0);
            }
        }
        for (int i = 0; i < m_PhysicsObjects.size(); i++)
        {
            if (m_PhysicsObjects[i]->Intersects(*m_ColliderObjects[j])) //std::cout << "Collision Detected!" << std::endl;
            {
                XMFLOAT3 eject_vect = Collision::ejectionCMOGO(*m_PhysicsObjects[i], *m_ColliderObjects[j]);
                auto pos = m_PhysicsObjects[i]->GetPos();
                m_PhysicsObjects[i]->SetPos(pos - eject_vect);
               


                /* std::cout <<"x" << eject_vect.x << std::endl;
                 std::cout << "y" << eject_vect.y << std::endl;
                 std::cout << "z" << eject_vect.z << std::endl;*/
            }

            /*if(m_ColliderObjects[j]->getCollider().Intersects(m_Player->test.position, m_Player->test.direction, dis))
            {
                std::cout << j << '\n' << dis << std::endl;
            }
            else
            {
                std::cout << "Hello" << std::endl;
            }*/


        }
    }
    //std::cout << collision_count << std::endl;
    if(collision_count > 0)
    {
        m_Player.get()->SetisFalling(false);
    }
    else
    {
        m_Player.get()->SetisFalling(true);
    }
}

void Game::CheckProjectileCollision()
{
    for (int i = 0; i < m_Projectiles.size(); i++) for (int j = 0; j < m_ColliderObjects.size(); j++)
    {
        if (m_Projectiles[i]->GetIsActive() && m_Projectiles[i]->Intersects(*m_ColliderObjects[j])) //std::cout << "Collision Detected!" << std::endl;
        {
            //std::cout << "collision" << std::endl;
            m_Projectiles[i]->SetActive(false);
        }
    }
}

void Game::CheckTriggerCollision()
{
    for (int j = 0; j < m_TriggerObjects.size(); j++)
    {
        if (m_Player->Intersects(*m_TriggerObjects[j]))
        {
            m_TriggerObjects[j]->OnIntersect();
        }
    }

    for (int i = 0; i < m_Projectiles.size(); i++)
    {
        if (m_Projectiles[i]->Intersects(*doorTrigger))
        {
            //std::cout << "hi";
            door.get()->Triggered();
        }
        for (int j = 0; j < m_ShootToTriggerObjects.size(); j++)
        {
            if (m_Projectiles[i]->GetIsActive() && m_Projectiles[i]->Intersects(*m_ShootToTriggerObjects[j]))
            {
                m_ShootToTriggerObjects[j]->OnIntersect();
                m_Projectiles[i]->SetActive(false);
            }
        }
    }
}

void Game::CheckTriggerProjectileCollision()
{
    for (int i = 0; i < m_ShootToTriggerObjects.size(); i++)
        {
        for (int j = 0; j < m_Projectiles.size(); j++)
        {
            if (m_Projectiles[j]->GetIsActive() && m_Projectiles[j]->Intersects(*m_ShootToTriggerObjects[i]))
            {
                m_ShootToTriggerObjects[i]->OnIntersect();
                m_Projectiles[j]->SetActive(false);
            }
        }
    }
}

void Game::CheckDamagingCollision()
{
    for (int j = 0; j < m_DamageObjects.size(); j++)
    {
        if (m_Player->Intersects(*m_DamageObjects[j]))
        {
            m_Player->Respawn();
        }
    }
}

void Game::ResetLevel()
{
    m_Player.get()->SetPos(Vector3(760.0f, 10.0f, 600.0f));
    m_Player->SetCheckPoint(Vector3(760.0f, 10.0f, 600.0f));
    m_Player.get()->SetAcceleration(Vector3::Zero);
}