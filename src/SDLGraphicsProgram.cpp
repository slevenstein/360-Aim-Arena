#include "SDLGraphicsProgram.hpp"
#include "Camera.hpp"
#include "Terrain.hpp"
#include "Sphere.hpp"

#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <random>

// Initialization function
// Returns a true or false value based on successful completion of setup.
// Takes in dimensions of window.
SDLGraphicsProgram::SDLGraphicsProgram(float sens, int w, int h):m_sens(sens),m_w(w),m_h(h){
	// Initialization flag
	bool success = true;
	// String to hold any errors that occur.
	std::stringstream errorStream;
	// The window we'll be rendering to
	m_window = NULL;

	// Initialize SDL
	if(SDL_Init(SDL_INIT_VIDEO)< 0){
		errorStream << "SDL could not initialize! SDL Error: " << SDL_GetError() << "\n";
		success = false;
	}
	else{
		//Use OpenGL 3.3 core
		SDL_GL_SetAttribute( SDL_GL_CONTEXT_MAJOR_VERSION, 3 );
		SDL_GL_SetAttribute( SDL_GL_CONTEXT_MINOR_VERSION, 3 );
		SDL_GL_SetAttribute( SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE );
		// We want to request a double buffer for smooth updating.
		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
		SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

		//Create window
		// m_window = SDL_CreateWindow( "Lab",
        //                         SDL_WINDOWPOS_UNDEFINED,
        //                         SDL_WINDOWPOS_UNDEFINED,
        //                         w,
        //                         h,
        //                         SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN );

        SDL_CreateWindowAndRenderer(
                    w, h, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN,
                    &m_window, &m_sdl_renderer);

		// Check if Window did not create.
		if( m_window == NULL ){
			errorStream << "Window could not be created! SDL Error: " << SDL_GetError() << "\n";
			success = false;
		}

		//Create an OpenGL Graphics Context
		m_openGLContext = SDL_GL_CreateContext( m_window );
		if( m_openGLContext == NULL){
			errorStream << "OpenGL context could not be created! SDL Error: " << SDL_GetError() << "\n";
			success = false;
		}

		// Initialize GLAD Library
		if(!gladLoadGLLoader(SDL_GL_GetProcAddress)){
			errorStream << "Failed to iniitalize GLAD\n";
			success = false;
		}

		//Initialize OpenGL
		if(!InitGL()){
			errorStream << "Unable to initialize OpenGL!\n";
			success = false;
		}
  	}

    // If initialization did not work, then print out a list of errors in the constructor.
    if(!success){
        errorStream << "SDLGraphicsProgram::SDLGraphicsProgram - Failed to initialize!\n";
        std::string errors=errorStream.str();
        SDL_Log("%s\n",errors.c_str());
    }else{
        SDL_Log("SDLGraphicsProgram::SDLGraphicsProgram - No SDL, GLAD, or OpenGL, errors detected during initialization\n\n");
    }

	// SDL_LogSetAllPriority(SDL_LOG_PRIORITY_WARN); // Uncomment to enable extra debug support!
	GetOpenGLVersionInfo();


    // Setup our Renderer
    m_renderer = new Renderer(w,h);    

    // sets mouse invisible and locks to window
    SDL_SetRelativeMouseMode(SDL_TRUE);
}


// Proper shutdown of SDL and destroy initialized objects
SDLGraphicsProgram::~SDLGraphicsProgram(){
    if(m_renderer!=nullptr){
        delete m_renderer;
    }


    //Destroy window
	SDL_DestroyWindow( m_window );
	// Point m_window to NULL to ensure it points to nothing.
	m_window = nullptr;
	//Quit SDL subsystems
	SDL_Quit();
}

float getRandCoordinateXZ(float min, float max) {
    float range = max - min;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dist(-range/2, range/2);
    float rand = dist(gen);

    if (rand > 0) {
        rand += min;
    }
    else {
        rand -= min;
    }
    return rand;
}

float getRandCoordinateY(float max) {
    return static_cast <float> (rand()) / static_cast <float> (RAND_MAX) * max / 4;
}


// Initialize OpenGL
// Setup any of our shaders here.
bool SDLGraphicsProgram::InitGL(){
	//Success flag
	bool success = true;

	return success;
}

// ====================== Create the planets =============
// NOTE: I will admit it is a little lazy to have these as globals,
//       we could build on our ObjectManager structure to manage the
//       each object that is inserted and automatically create a 'SceneNode'
//       for it for example. Or otherwise, build a 'SceneTree' class that
//       manages creating nodes and setting their type.
//       (e.g. A good use of the Factory Design Pattern if you'd like to
//             invetigate further--but it is beyond the scope of this assignment).

// Create the Moon
Object* sphere;
SceneNode* Eye;

// ====================== Create the planets =============

//Loops forever!
void SDLGraphicsProgram::Loop(){

    // 800 = 20 seconds
    int timer = 800;
    bool timerStarted = false;
    int score = 0;



    // ================== Initialize the planets ===============
    static float rotate = 1.0f;
    // max distance of each coordinate (X,Y,Z) from camera
    static float maxCoord = 20.0f;
    // min distance of X and Z coordinate from camera
    static float minCoord = 10.0f;

    float randX = 1.0f;
    float randY = 0.2f;
    float randZ = 0.0f;

    Object* root = new Object;
    SceneNode* rootNode = new SceneNode(root);

    // Create new geometry for Earth's Moon
    sphere = new Sphere();
    sphere->LoadTexture("eye.ppm");
    Eye = new SceneNode(sphere);

    Object* grass = new Object;
    grass->MakeTexturedQuad("grass.ppm");
    SceneNode* Grass = new SceneNode(grass);

    Sphere* sphere2 = new Sphere();
    sphere2->LoadTexture("crosshair.ppm");
    SceneNode* Crosshair = new SceneNode(sphere2);

    m_renderer->setRoot(rootNode);

    rootNode->AddChild(Crosshair);
    rootNode->AddChild(Eye);
    rootNode->AddChild(Grass);

    //SET INITIAL TRANSFORMS
    Grass->GetLocalTransform().LoadIdentity();		
    Grass->GetLocalTransform().Translate(0.0f, -3.0f, 0.0f);
    Grass->GetLocalTransform().Rotate(glm::radians(90.0f),1.0f,0.0f,0.0f);
    Grass->GetLocalTransform().Scale(50.0f,50.0f,50.0f);
    
    Eye->GetLocalTransform().LoadIdentity();		
    Eye->GetLocalTransform().Translate(randX, randY, randZ);
    Eye->GetLocalTransform().Scale(0.05f,0.05f,0.05f);
    
    Crosshair->GetLocalTransform().LoadIdentity();		
    Crosshair->GetLocalTransform().Translate(0.0f, 0.0f, 1.0f);
    Crosshair->GetLocalTransform().Scale(0.005f,0.005f,0.005f);

    // Set a default position for our camera
    m_renderer->GetCamera(0)->SetCameraEyePosition(0.0f,0.0f,0.0f);

    // Main loop flag
    // If this is quit = 'true' then the program terminates.
    bool quit = false;
    // Event handler that handles various events in SDL
    // that are related to input and output
    SDL_Event e;
    // Enable text input
    SDL_StartTextInput();

    // Set the camera speed for how fast we move.
    float cameraSpeed = 5.0f;

    // While application is running
    while(!quit){

        //Handle events on queue
        while(SDL_PollEvent( &e ) != 0){
            // User posts an event to quit
            // An example is hitting the "x" in the corner of the window.
            if(e.type == SDL_QUIT){
                quit = true;
            }
            // Handle keyboad input for the camera class
            if(e.type==SDL_MOUSEMOTION){
                // Handle mouse movements
                int mouseX = e.motion.xrel;
                int mouseY = e.motion.yrel;

                m_renderer->GetCamera(0)->MouseLook(m_sens, -mouseX, -mouseY);
                // listen... if it works, it works
                Crosshair->GetLocalTransform().LoadIdentity();
                Crosshair->GetLocalTransform().Scale(0.0005f,0.0005f,0.0005f);
                Crosshair->GetLocalTransform().Translate(m_renderer->GetCamera(0)->GetViewXDirection()*200,m_renderer->GetCamera(0)->GetViewYDirection()*200,m_renderer->GetCamera(0)->GetViewZDirection()*200);
            }
            switch(e.type){
                // Handle keyboard presses
                case SDL_KEYDOWN:
                    switch(e.key.keysym.sym){
                        case SDLK_q:
                            quit = true;
                            break;
                    }
                // handle mouse events
                case SDL_MOUSEBUTTONDOWN:
                    switch (e.button.button)
                    {
                        case SDL_BUTTON_LEFT:
                            if (m_renderer->GetCamera(0)->LookingAtEye((glm::vec3(randX, randY, randZ)))) {
                                randX = getRandCoordinateXZ(minCoord, maxCoord);
                                randZ = getRandCoordinateXZ(minCoord, maxCoord);
                                randY = getRandCoordinateY(maxCoord);
                                Eye->GetLocalTransform().LoadIdentity();
                                Eye->GetLocalTransform().Translate(randX, randY, randZ);
                                //std::cout << randX << " " << randY << " " << randZ << std::endl;
                                score++;
                                timerStarted = true;
                            }
                            break;
                    }
            }
        } // End SDL_PollEvent loop.

        Eye->GetLocalTransform().Rotate(0.3f, 0.0f, 1.0f, 0.0f);

        // Update our scene through our renderer
        m_renderer->Update();
        // Render our scene using our selected renderer
        m_renderer->Render();
        // Delay to slow things down just a bit!
        SDL_Delay(25);  // TODO: You can change this or implement a frame
                        // independent movement method if you like.
      	//Update screen of our specified window
      	SDL_GL_SwapWindow(GetSDLWindow());

        rotate += 0.01f;

        if (timerStarted) {
            timer--;
            std::cout << timer << std::endl;
            if (timer < 0) {
                quit = true;
            }
        }
	}

    std::cout << "GAME OVER" << std::endl;
    std::cout << "SCORE: " << score << std::endl;

    SDL_Delay(500);

    //Disable text input
    SDL_StopTextInput();
}


// Get Pointer to Window
SDL_Window* SDLGraphicsProgram::GetSDLWindow(){
  return m_window;
}

// Helper Function to get OpenGL Version Information
void SDLGraphicsProgram::GetOpenGLVersionInfo(){
	SDL_Log("(Note: If you have two GPU's, make sure the correct one is selected)");
	SDL_Log("Vendor: %s",(const char*)glGetString(GL_VENDOR));
	SDL_Log("Renderer: %s",(const char*)glGetString(GL_RENDERER));
	SDL_Log("Version: %s",(const char*)glGetString(GL_VERSION));
	SDL_Log("Shading language: %s",(const char*)glGetString(GL_SHADING_LANGUAGE_VERSION));
}
