#include "Application.h"

#include <memory>

#define GL_GLEXT_PROTOTYPES
#include <GLES2/gl2.h>

class VertexShader : public tinygles::Application {
public:
	VertexShader() = default;
	virtual ~VertexShader() {

	}

	tinygles::ContextAttribs GetContextAttribs() override {
		static tinygles::ContextAttribs sAttributes;

		sAttributes.mRequiredApi = tinygles::Api::OpenGLES2;
		sAttributes.mDepthBPP = 32;
		sAttributes.mStencilBPP = 0;
		sAttributes.mRedBits = 8;
		sAttributes.mGreenBits = 8;
		sAttributes.mBlueBits = 8;
		sAttributes.mAlphaBits = 8;

		return sAttributes;
	}

	void InitApplication() override {

	}

	void InitView() override {
		initializeBuffer(mVertexBuffer);
		initializeShaders(mFragmentShader, mVertexShader, mShaderProgram);
	}

	void RenderFrame() override {
		GLenum lastError;

		glClearColor(0.00f, 0.70f, 0.67f, 1.0f);

		//	Clears the color buffer.
		//	glClear is used here with the Color Buffer to clear the color. It can also be used to clear the depth or stencil buffer using
		//	GL_DEPTH_BUFFER_BIT or GL_STENCIL_BUFFER_BIT, respectively.
		glClear(GL_COLOR_BUFFER_BIT);

		// Get the location of the transformation matrix in the shader using its name
		int matrixLocation = glGetUniformLocation(mShaderProgram, "transformationMatrix");

		// Matrix used to specify the orientation of the triangle on screen.
		const float transformationMatrix[] =
		{
			1.0f, 0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f
		};

		// Pass the transformationMatrix to the shader using its location
		glUniformMatrix4fv(matrixLocation, 1, GL_FALSE, transformationMatrix);
		lastError = glGetError();
		if (lastError != GL_NO_ERROR) { return; }

		// Enable the user-defined vertex array
		glEnableVertexAttribArray(mVertexArray);

		// Sets the vertex data to this attribute index, with the number of floats in each position
		glVertexAttribPointer(mVertexArray, 3, GL_FLOAT, GL_FALSE, 0, 0);
		lastError = glGetError();
		if (lastError != GL_NO_ERROR) { return; }

		//	Draw the triangle
		//	glDrawArrays is a draw call, and executes the shader program using the vertices and other state set by the user. Draw calls are the
		//	functions which tell OpenGL ES when to actually draw something to the framebuffer given the current state.
		//	glDrawArrays causes the vertices to be submitted sequentially from the position given by the "first" argument until it has processed
		//	"count" vertices. Other draw calls exist, notably glDrawElements which also accepts index data to allow the user to specify that
		//	some vertices are accessed multiple times, without copying the vertex multiple times.
		//	Others include versions of the above that allow the user to draw the same object multiple times with slightly different data, and
		//	a version of glDrawElements which allows a user to restrict the actual indices accessed.
		glDrawArrays(GL_TRIANGLES, 0, 3);
		lastError = glGetError();
		if (lastError != GL_NO_ERROR) { return; }

		glFinish();
	}

	void ReleaseView() override {
		glDeleteShader(mFragmentShader);
		glDeleteShader(mVertexShader);
		glDeleteProgram(mShaderProgram);

		// Delete the VBO as it is no longer needed
		glDeleteBuffers(1, &mVertexBuffer);
	}

	void ReleaseApplication() override {

	}

private:
	bool initializeBuffer(GLuint& vertexBuffer) {
		//	Concept: Vertices
		//	When rendering a polygon or model to screen, OpenGL ES has to be told where to draw the object, and more fundamentally what shape
		//	it is. The data used to do this is referred to as vertices, points in 3D space which are usually collected into groups of three
		//	to render as triangles. Fundamentally, any advanced 3D shape in OpenGL ES is constructed from a series of these vertices - each
		//	vertex representing one corner of a polygon.


		//  Concept: Buffer Objects
		//	To operate on any data, OpenGL first needs to be able to access it. The GPU maintains a separate pool of memory it uses independent
		//	of the CPU. Whilst on many embedded systems these are in the same physical memory, the distinction exists so that they can use and
		//	allocate memory without having to worry about synchronising with any other processors in the device.
		//	To this end, data needs to be uploaded into buffers, which are essentially a reserved bit of memory for the GPU to use. By creating
		//	a buffer and giving it some data we can tell the GPU how to render a triangle.


		// Vertex data containing the positions of each point of the triangle
		GLfloat vertexData[] = {
			-0.4f, -0.4f, 0.0f, // Bottom Left
			0.4f, -0.4f, 0.0f, // Bottom Right
			0.0f, 0.4f, 0.0f
		}; // Top Middle

		   // Generate a buffer object
		glGenBuffers(1, &vertexBuffer);

		// Bind buffer as an vertex buffer so we can fill it with data
		glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);

		//	Set the buffer's size, data and usage
		//	Note the last argument - GL_STATIC_DRAW. This tells the driver that we intend to read from the buffer on the GPU, and don't intend
		//	to modify the data until we're done with it.
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData, GL_STATIC_DRAW);

		GLenum lastError = glGetError();
		if (lastError != GL_NO_ERROR) { return false; }

		return true;
	}

	/*!*********************************************************************************************************************
	\param[out]	fragmentShader   Handle to a fragment shader
	\param[out]	vertexShader     Handle to a vertex shader
	\param[out]	shaderProgram    Handle to a shader program containing the fragment and vertex shader
	\param		nativeWindow     A native window, used to display error messages
	\return	Whether the function succeeds or not.
	\brief	Initializes shaders, buffers and other state required to begin rendering with OpenGL ES
	***********************************************************************************************************************/
	bool initializeShaders(GLuint& fragmentShader, GLuint& vertexShader, GLuint& shaderProgram) {

		//	Concept: Shaders
		//	OpenGL ES 2.0 uses what are known as shaders to determine how to draw objects on the screen. Instead of the fixed function
		//	pipeline in early OpenGL or OpenGL ES 1.x, users can now programmatically define how vertices are transformed on screen, what
		//	data is used where, and how each pixel on the screen is colored.
		//	These shaders are written in GL Shading Language ES: http://www.khronos.org/registry/gles/specs/2.0/GLSL_ES_Specification_1.0.17.pdf
		//	which is usually abbreviated to simply "GLSL ES".
		//	Each shader is compiled on-device and then linked into a shader program, which combines a vertex and fragment shader into a form
		//	that the OpenGL ES implementation can execute.


		//	Concept: Fragment Shaders
		//	In a final buffer of image data, each individual point is referred to as a pixel. Fragment shaders are the part of the pipeline
		//	which determine how these final pixels are colored when drawn to the framebuffer. When data is passed through here, the positions
		//	of these pixels is already set, all that's left to do is set the final color based on any defined inputs.
		//	The reason these are called "fragment" shaders instead of "pixel" shaders is due to a small technical difference between the two
		//	concepts. When you color a fragment, it may not be the final color which ends up on screen. This is particularly true when
		//	performing blending, where multiple fragments can contribute to the final pixel color.

		const char* const fragmentShaderSource = "\
											 void main (void)\
											 {\
												gl_FragColor = vec4(1.0, 0.66, 1.0 ,1.0);\
											 }";

		// Create a fragment shader object
		fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

		// Load the source code into it
		glShaderSource(fragmentShader, 1, (const char**)&fragmentShaderSource, (const GLint*)0);

		// Compile the source code
		glCompileShader(fragmentShader);

		// Check that the shader compiled
		GLint isShaderCompiled;
		glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &isShaderCompiled);
		if (!isShaderCompiled)
		{
			// If an error happened, first retrieve the length of the log message
			int infoLogLength, charactersWritten;
			glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &infoLogLength);

			// Allocate enough space for the message and retrieve it
			char* infoLog = new char[infoLogLength];
			glGetShaderInfoLog(fragmentShader, infoLogLength, &charactersWritten, infoLog);
			delete[] infoLog;

			return false;
		}

		//	Concept: Vertex Shaders
		//	Vertex shaders primarily exist to allow a developer to express how to orient vertices in 3D space, through transformations like
		//	Scaling, Translation or Rotation. Using the same basic layout and structure as a fragment shader, these take in vertex data and
		//	output a fully transformed set of positions. Other inputs are also able to be used such as normals or texture coordinates, and can
		//	also be transformed and output alongside the position data.

		// Vertex shader code
		const char* const vertexShaderSource = "\
										   attribute highp vec4	myVertex;\
										   uniform mediump mat4	transformationMatrix;\
										   void main(void)\
										   {\
										   gl_Position = transformationMatrix * myVertex;\
										   }";

		// Create a vertex shader object
		vertexShader = glCreateShader(GL_VERTEX_SHADER);

		// Load the source code into the shader
		glShaderSource(vertexShader, 1, (const char**)&vertexShaderSource, (const GLint*)0);

		// Compile the shader
		glCompileShader(vertexShader);

		// Check the shader has compiled
		glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &isShaderCompiled);
		if (!isShaderCompiled)
		{
			// If an error happened, first retrieve the length of the log message
			int infoLogLength, charactersWritten;
			glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &infoLogLength);

			// Allocate enough space for the message and retrieve it
			char* infoLog = new char[infoLogLength];
			glGetShaderInfoLog(vertexShader, infoLogLength, &charactersWritten, infoLog);

			// Display the error in a dialog box
			delete[] infoLog;
			return false;
		}

		// Create the shader program
		shaderProgram = glCreateProgram();

		// Attach the fragment and vertex shaders to it
		glAttachShader(shaderProgram, fragmentShader);
		glAttachShader(shaderProgram, vertexShader);

		// Bind the vertex attribute "myVertex" to location VERTEX_ARRAY (0)
		glBindAttribLocation(shaderProgram, mVertexArray, "myVertex");

		// Link the program
		glLinkProgram(shaderProgram);

		// Check if linking succeeded in the same way we checked for compilation success
		GLint isLinked;
		glGetProgramiv(shaderProgram, GL_LINK_STATUS, &isLinked);
		if (!isLinked)
		{
			// If an error happened, first retrieve the length of the log message
			int infoLogLength, charactersWritten;
			glGetProgramiv(shaderProgram, GL_INFO_LOG_LENGTH, &infoLogLength);

			// Allocate enough space for the message and retrieve it
			char* infoLog = new char[infoLogLength];
			glGetProgramInfoLog(shaderProgram, infoLogLength, &charactersWritten, infoLog);
			delete[] infoLog;

			return false;
		}

		//	Use the Program
		//	Calling glUseProgram tells OpenGL ES that the application intends to use this program for rendering. Now that it's installed into
		//	the current state, any further glDraw* calls will use the shaders contained within it to process scene data. Only one program can
		//	be active at once, so in a multi-program application this function would be called in the render loop. Since this application only
		//	uses one program it can be installed in the current state and left there.

		glUseProgram(shaderProgram);

		GLenum lastError = glGetError();
		if (lastError != GL_NO_ERROR) { return false; }

		return true;
	}


private:
	GLuint mVertexBuffer;
	GLuint mFragmentShader;
	GLuint mVertexShader;
	GLuint mShaderProgram;

	GLuint mVertexArray;
};


extern "C" tinygles::Application * CreateApplication() {
	return new VertexShader();
}