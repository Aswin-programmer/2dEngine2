#include "pch.h"

#include "WINDOW/Window.h"

#include "UTILITIES/Time.h"
#include "RENDERER/UTILS/OpenGLDebugger.h"

// Initialize static members
unsigned int Window::m_width = 0;
unsigned int Window::m_height = 0;
const char* Window::m_title = nullptr;
GLFWwindow* Window::m_window = nullptr;
float Window::beginTime = 0.0f;
float Window::endTime = 0.0f;
float Window::dt = 0.0f;
float Window::r = 0.0f;
float Window::g = 0.0f;
float Window::b = 0.0f;
float Window::a = 1.0f;

bool Window::init( const char* title )
{
	// Initialize GLFW
	if ( !glfwInit() )
	{
		std::cerr << "Failed to initialize GLFW" << std::endl;
		return false;
	}

	// Configure GLFW
	glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 4 );
	glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 3 );
	glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE );

	glfwWindowHint( GLFW_RESIZABLE, GLFW_TRUE ); // allow resizing
	glfwWindowHint( GLFW_MAXIMIZED, GLFW_TRUE ); // start maximized :contentReference[oaicite:0]{index=0}

	GLFWmonitor* primary = glfwGetPrimaryMonitor();

	// Getting the other monitors
	int count;
	GLFWmonitor** monitors = glfwGetMonitors( &count );
	GLFWmonitor* secondMonitor = monitors[1];

	const GLFWvidmode* mode = glfwGetVideoMode( primary ); // get width, height :contentReference[oaicite:1]{index=1}

	// Initialize variables:
	m_width = mode->width;
	m_height = mode->height;
	m_title = title;

	// Create the GLFW window
	m_window = glfwCreateWindow( m_width, m_height, m_title, nullptr, nullptr );
	if ( !m_window )
	{
		std::cerr << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return false;
	}

	/*glfwSetWindowPos(m_window,
		(mode->width - mode->width) / 2,
		(mode->height - mode->height) / 2
	);*/

	glfwMakeContextCurrent( m_window );
	// glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	// Enable v-sync { Locks the FPS to the monitor refreshing rate! }
	glfwSwapInterval( 1 );

	// glad: load all OpenGL function pointers
	// ---------------------------------------
	GLenum err = glewInit();
	if ( GLEW_OK != err )
	{
		/* Problem: glewInit failed, something is seriously wrong. */
		std::cout << "Failed to initialize glew" << std::endl;
		glfwTerminate();
		return false;
	}

	// Enable OpenGL features
	glEnable( GL_BLEND );
	glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

	// Set viewport and resize callback
	glfwSetFramebufferSizeCallback( m_window, framebufferSizeCallback );
	////Setup of MouseListener:
	// glfwSetCursorPosCallback(m_window, MouseListener::mousePosCallback);
	// glfwSetMouseButtonCallback(m_window, MouseListener::mouseButtonCallback);
	// glfwSetScrollCallback(m_window, MouseListener::mouseScrollCallback);
	////Setup of KeyListener:
	// glfwSetKeyCallback(m_window, KeyListener::keyCallback);

	glEnable( GL_DEPTH_TEST );

	// Set initial viewport
	glViewport( 0, 0, m_width, m_height );

	OpenGLDebugger::init();
	OpenGLDebugger::breakOnError( false );
	OpenGLDebugger::breakOnWarning( false );

	// Delta Time Initialization:
	beginTime = Time::getTime();
	endTime;
	dt = -1.0f;

	return true;
}

void Window::processInput()
{
	/*if (glfwGetKey(m_window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(m_window, true);
	}*/
}

bool Window::shouldClose()
{
	return glfwWindowShouldClose( m_window );
}

void Window::update()
{
	endTime = Time::getTime();
	dt = endTime - beginTime;
	beginTime = endTime;

	glfwSwapBuffers( m_window );
	glfwPollEvents();
}

void Window::clearScreen()
{
	glClearColor( r, g, b, a );
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
}

GLFWwindow* Window::getGLFWWindow()
{
	return m_window;
}

void Window::getFPS()
{
	std::cout << " " << ( 1.0f / dt ) << "FPS" << std::endl;
}

void Window::cleanup()
{
	glfwTerminate();
}

void Window::framebufferSizeCallback( GLFWwindow* window, int width, int height )
{
	m_width = width;
	m_height = height;
	glViewport( 0, 0, width, height );
}
