#include "../../Common/Window.h"
#include "../../Common/TextureLoader.h"
#include "../../Common/Vector3.h"
#include "../../Common/Vector4.h"
#include "../../Common/MeshGeometry.h"
#include "../../Common/Maths.h"

#include "../../Plugins/OpenGLRendering/OGLRenderer.h"
#include "../../Plugins/OpenGLRendering/OGLMesh.h"
#include "../../Plugins/OpenGLRendering/OGLShader.h"
#include "../../Plugins/OpenGLRendering/OGLTexture.h"

#include "Renderer.h"
#include <cmath>
#include "../../Common/stb/stb_image.h"

using namespace NCL;
using namespace CSC3223;

// RGB 
const Vector3 RGB_VEC_RED = Vector3(1, 0, 0);
const Vector3 RGB_VEC_BLUE = Vector3(0, 0, 1);
const Vector3 RGB_VEC_WHITE = Vector3(1, 1, 1);
const Vector3 RGB_VEC_BLACK = Vector3(0, 0, 0);
const Vector3 RGB_VEC_ORANGE = Vector3(1, 0.65, 0);
const Vector3 RGB_VEC_YELLOW = Vector3(1, 1, 0);
const Vector3 RGB_VEC_LIME = Vector3(0, 1, 0);
const Vector3 RGB_VEC_GREEN = Vector3(0, 0.5, 0);
const Vector3 RGB_VEC_PINK = Vector3(1, 0, 1);
const Vector3 RGB_VEC_CYAN = Vector3(0, 1, 1);
const Vector3 RGB_VEC_PURPLE = Vector3(0.5, 0, 0.5);
const Vector3 RGB_VEC_SILVER = Vector3(0.75, 0.75, 0.75);
const Vector3 RGB_VEC_GREY = Vector3(0.5, 0.5, 0.5);

// opaque colours
const Vector4 VEC_RED = Vector4(1, 0, 0, 1);
const Vector4 VEC_BLUE = Vector4(0, 0, 1, 1);
const Vector4 VEC_WHITE = Vector4(1, 1, 1, 1);
const Vector4 VEC_BLACK = Vector4(0, 0, 0, 1);
const Vector4 VEC_ORANGE = Vector4(1, 0.65, 0, 1);
const Vector4 VEC_YELLOW = Vector4(1, 1, 0, 1);
const Vector4 VEC_LIME = Vector4(0, 1, 0, 1);
const Vector4 VEC_GREEN = Vector4(0, 0.5, 0, 1);
const Vector4 VEC_PINK = Vector4(1, 0, 1, 1);
const Vector4 VEC_CYAN = Vector4(0, 1, 1, 1);
const Vector4 VEC_PURPLE = Vector4(0.5, 0, 0.5, 1);
const Vector4 VEC_SILVER = Vector4(0.75, 0.75, 0.75, 1);
const Vector4 VEC_GREY = Vector4(0.5, 0.5, 0.5, 1);

// textures (references are in PDF)
const string texClean = "Gill_toyshop_counter01.PNG";
const string texDirty = "Gill_toyshop_counter02.PNG";
const string texBlood = "Gill_toyshop_counter03.PNG";

const string texBlank = "dispMapCrater04_blank.png";
const string texCrater = "dispMapCrater05.png";
const string texGlowDots = "glowDots.png";
const string transparentGrid = "transparentGrid.png";

const float skySCALE = 20.0f;
const vector<Vector3> skyboxVertices =
{
	Vector3(-skySCALE,  skySCALE, -skySCALE),
	Vector3(-skySCALE, -skySCALE, -skySCALE),
	Vector3(skySCALE, -skySCALE, -skySCALE),
	Vector3(skySCALE, -skySCALE, -skySCALE),
	Vector3(skySCALE,  skySCALE, -skySCALE),
	Vector3(-skySCALE,  skySCALE, -skySCALE),

	Vector3(-skySCALE, -skySCALE,  skySCALE),
	Vector3(-skySCALE, -skySCALE, -skySCALE),
	Vector3(-skySCALE,  skySCALE, -skySCALE),
	Vector3(-skySCALE,  skySCALE, -skySCALE),
	Vector3(-skySCALE,  skySCALE,  skySCALE),
	Vector3(-skySCALE, -skySCALE,  skySCALE),

	Vector3(skySCALE, -skySCALE, -skySCALE),
	Vector3(skySCALE, -skySCALE,  skySCALE),
	Vector3(skySCALE,  skySCALE,  skySCALE),
	Vector3(skySCALE,  skySCALE,  skySCALE),
	Vector3(skySCALE,  skySCALE, -skySCALE),
	Vector3(skySCALE, -skySCALE, -skySCALE),

	Vector3(-skySCALE, -skySCALE,  skySCALE),
	Vector3(-skySCALE,  skySCALE,  skySCALE),
	Vector3(skySCALE,  skySCALE,  skySCALE),
	Vector3(skySCALE,  skySCALE,  skySCALE),
	Vector3(skySCALE, -skySCALE,  skySCALE),
	Vector3(-skySCALE, -skySCALE,  skySCALE),

	Vector3(-skySCALE,  skySCALE, -skySCALE),
	Vector3(skySCALE,  skySCALE, -skySCALE),
	Vector3(skySCALE,  skySCALE,  skySCALE),
	Vector3(skySCALE,  skySCALE,  skySCALE),
	Vector3(-skySCALE,  skySCALE,  skySCALE),
	Vector3(-skySCALE,  skySCALE, -skySCALE),

	Vector3(-skySCALE, -skySCALE, -skySCALE),
	Vector3(-skySCALE, -skySCALE,  skySCALE),
	Vector3(skySCALE, -skySCALE, -skySCALE),
	Vector3(skySCALE, -skySCALE, -skySCALE),
	Vector3(-skySCALE, -skySCALE,  skySCALE),
	Vector3(skySCALE, -skySCALE,  skySCALE)
};

const vector<Vector3> skyboxTexCoords =
{
	Vector3(-1.0f,  1.0f, -1.0f),
	Vector3(-1.0f, -1.0f, -1.0f),
	Vector3(1.0f, -1.0f, -1.0f),
	Vector3(1.0f, -1.0f, -1.0f),
	Vector3(1.0f,  1.0f, -1.0f),
	Vector3(-1.0f,  1.0f, -1.0f),

	Vector3(-1.0f, -1.0f,  1.0f),
	Vector3(-1.0f, -1.0f, -1.0f),
	Vector3(-1.0f,  1.0f, -1.0f),
	Vector3(-1.0f,  1.0f, -1.0f),
	Vector3(-1.0f,  1.0f,  1.0f),
	Vector3(-1.0f, -1.0f,  1.0f),

	Vector3(1.0f, -1.0f, -1.0f),
	Vector3(1.0f, -1.0f,  1.0f),
	Vector3(1.0f,  1.0f,  1.0f),
	Vector3(1.0f,  1.0f,  1.0f),
	Vector3(1.0f,  1.0f, -1.0f),
	Vector3(1.0f, -1.0f, -1.0f),

	Vector3(-1.0f, -1.0f,  1.0f),
	Vector3(-1.0f,  1.0f,  1.0f),
	Vector3(1.0f,  1.0f,  1.0f),
	Vector3(1.0f,  1.0f,  1.0f),
	Vector3(1.0f, -1.0f,  1.0f),
	Vector3(-1.0f, -1.0f,  1.0f),

	Vector3(-1.0f,  1.0f, -1.0f),
	Vector3(1.0f,  1.0f, -1.0f),
	Vector3(1.0f,  1.0f,  1.0f),
	Vector3(1.0f,  1.0f,  1.0f),
	Vector3(-1.0f,  1.0f,  1.0f),
	Vector3(-1.0f,  1.0f, -1.0f),

	Vector3(-1.0f, -1.0f, -1.0f),
	Vector3(-1.0f, -1.0f,  1.0f),
	Vector3(1.0f, -1.0f, -1.0f),
	Vector3(1.0f, -1.0f, -1.0f),
	Vector3(-1.0f, -1.0f,  1.0f),
	Vector3(1.0f, -1.0f,  1.0f)
};

// right, left, top, bottom, back, front
const vector<string> skyboxCubeFaces =
{
	"mp_mercury/mercury_rt.PNG"
	, "mp_mercury/mercury_lf.PNG"
	, "mp_mercury/mercury_up.PNG"
	, "mp_mercury/mercury_dn.PNG"
	, "mp_mercury/mercury_bk.PNG"
	, "mp_mercury/mercury_ft.PNG"
};

GLuint loadCubemap(vector<string> faces)
{
	GLuint textureID;
	glGenTextures(1, &textureID);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

	int width, height, nrChannels, flag;
	for (unsigned int i = 0; i < faces.size(); i++)
	{
		char* texData;

		TextureLoader::LoadTexture(faces[i].c_str(), texData, width, height, nrChannels, flag);
		//unsigned char* data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
		if ((texData != NULL) && (texData[0] == '\0'))
		{
			std::cout << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
			free(texData);
		}
		else
		{
			int sourceType = GL_RGB;

			switch (nrChannels)
			{
			case 1: nrChannels = GL_RED; break;
			case 2: nrChannels = GL_RG; break;
			case 3: nrChannels = GL_RGB; break;
			case 4: nrChannels = GL_RGBA; break;
			}
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, nrChannels, GL_UNSIGNED_BYTE, texData);

			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

			free(texData);
		}
	}

	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

	return textureID;
}

RenderObject* Coursework2(Renderer& renderer)
{
	OGLMesh* m = new OGLMesh("cube.msh");
	m->SetPrimitiveType(GeometryPrimitive::Patches);
	m->UploadToGPU();

	RenderObject* object = new RenderObject(m);

	object->SetBaseTexture(OGLTexture::RGBATextureFromFilename(texCrater));
	object->SetSecondTexture(OGLTexture::RGBATextureFromFilename(texClean));
	object->SetCubeMapTexture(loadCubemap(skyboxCubeFaces));

	object->SetShader(new OGLShader("CW2_VertexShader.glsl", "CW2_FragmentShader.glsl", "", "CW2_TCS.glsl", "CW2_TES.glsl"));

	renderer.AddRenderObject(object);
	renderer.SetLightProperties
	(
		Vector3(0, 25, 10)
		, RGB_VEC_ORANGE
		, 200.0f
	);

	return object;
}

/*
	Generate a skybox using a Cubemap
*/
RenderObject* generateSkybox(Renderer& renderer)
{
	OGLMesh* m = new OGLMesh();
	m->SetVertexPositions(skyboxVertices);
	m->SetVertexTextureCoords(skyboxTexCoords);
	m->SetPrimitiveType(GeometryPrimitive::Triangles);
	m->UploadToGPU();

	RenderObject* object = new RenderObject(m);
	object->SetCubeMapTexture(loadCubemap(skyboxCubeFaces));
	object->SetShader(new OGLShader("CW2_Skybox_VertexShader.glsl", "CW2_Skybox_FragmentShader.glsl"));
	
	renderer.AddRenderObject(object);

	return object;
}

/*
	This is the laser that will destroy the cube in one of the effects. It is composed of two triangle strips.
*/
vector<RenderObject*> createLaserQuads(Renderer& renderer)
{
	OGLShader* lightingShader = new OGLShader("CW2_VertexShader_LaserQuads_Hide.glsl", "CW2_FragmentShader_LaserBase_Hide.glsl");

	OGLMesh* triMesh = new OGLMesh();
	vector<Vector3> verts =
	{
		Vector3(-1, 0, -1)
		, Vector3(-1, 0, 1)
		, Vector3(1, 0, -1)
		, Vector3(1, 0, 1)
	};

	vector<Vector2> texCoords =
	{
		Vector2(0.0f, 1.0f)
		, Vector2(0.0f, 0.0f)
		, Vector2(1.0f, 1.0f)
		, Vector2(1.0f, 0.0f)
	};

	triMesh->SetVertexPositions(verts);
	triMesh->SetVertexTextureCoords(texCoords);
	triMesh->SetPrimitiveType(GeometryPrimitive::TriangleStrip);
	triMesh->UploadToGPU();

	Matrix4 triMat = Matrix4::Translation(Vector3(0, 0, 10)) * Matrix4::Scale(Vector3(0.3, 1.0, 10));
	Matrix4 rotMat = Matrix4::Rotation(90, Vector3(0, 0, 1.0));

	RenderObject* triObject = new RenderObject(triMesh, triMat);
	triObject->SetBaseTexture(OGLTexture::RGBATextureFromFilename("laserTex.PNG"));
	triObject->SetShader(lightingShader);
	renderer.AddRenderObject(triObject);

	RenderObject* triObjectRot = new RenderObject(triMesh, rotMat * triMat);
	triObjectRot->SetBaseTexture(OGLTexture::RGBATextureFromFilename("laserTex.PNG"));
	triObjectRot->SetShader(lightingShader);
	renderer.AddRenderObject(triObjectRot);

	return { triObject, triObjectRot };
}

int main()
{
	Window* w = Window::CreateGameWindow("CSC3223 Coursework 2!");

	if (!w->HasInitialised()) {
		return -1;
	}
	Renderer* renderer = new Renderer(*w);
	glPatchParameteri(GL_PATCH_VERTICES, 3);

	// right, left, top, bottom, back, front
	vector<string> displacementCubeFaces =
	{
		texBlank
		, texBlank
		, texBlank
		, texBlank
		, texCrater
		, texBlank
	};

	RenderObject* skybox = generateSkybox(*renderer);
	RenderObject* cube = Coursework2(*renderer);
	vector<RenderObject*> vecLaserQuads = createLaserQuads(*renderer);

	Vector3 viewPosition(0, 0, 0);
	Vector2 vecMousePosition;
	w->ShowOSPointer(false);

	renderer->SetProjectionMatrix(Matrix4::Perspective(1, 1000, w->GetScreenAspect(), 45.0f));

	float mvmt = 1.0f;
	int MAX_X = w->GetScreenSize()[0] - (w->GetScreenSize()[0] / 20);
	int MAX_Y = w->GetScreenSize()[1] - (w->GetScreenSize()[1] / 20);

	float rotation = 0.0f;
	while (w->UpdateWindow() && !Window::GetKeyboard()->KeyDown(KEYBOARD_ESCAPE))
	{
		glEnable(GL_DEPTH_TEST);
		glClear(GL_COLOR_BUFFER_BIT);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

		float time = w->GetTimer()->GetDeltaTime();
		rotation += time * 30;
		renderer->Update(time);

		Matrix4 modelMat = Matrix4::Translation(Vector3(0, 0, -10));
		modelMat = modelMat * Matrix4::Rotation(rotation, Vector3(1, 1, 1));
		cube->SetTransform(modelMat);

		renderer->DrawString(">> CUBE DESTRUCTION <<", Vector2(MAX_X * 0.25, MAX_Y));

		renderer->DrawString("F1: Shrink!", Vector2(10, MAX_Y * 0.90));
		renderer->DrawString("F2: Blend!", Vector2(10, MAX_Y * 0.85));
		renderer->DrawString("F3: Fade!", Vector2(10, MAX_Y * 0.80));
		renderer->DrawString("F4: Jack out!", Vector2(10, MAX_Y * 0.75));
		renderer->DrawString("F5: Shatter!", Vector2(10, MAX_Y * 0.70));
		renderer->DrawString("F6: Laser!", Vector2(10, MAX_Y * 0.65));

		renderer->DrawString("R: RESET", Vector2(10, MAX_Y * 0.05));

		renderer->Render();

		if (Window::GetKeyboard()->KeyPressed(KEYBOARD_PRIOR)) {
			w->ShowConsole(true);
		}
		if (Window::GetKeyboard()->KeyPressed(KEYBOARD_NEXT)) {
			w->ShowConsole(false);
		}

		if (Window::GetKeyboard()->KeyPressed(KEYBOARD_HOME)) {
			w->SetFullScreen(true);
		}
		if (Window::GetKeyboard()->KeyPressed(KEYBOARD_END)) {
			w->SetFullScreen(false);
		}

		if (Window::GetKeyboard()->KeyPressed(KEYBOARD_PRIOR)) {
			w->ShowConsole(true);
		}
		if (Window::GetKeyboard()->KeyPressed(KEYBOARD_NEXT)) {
			w->ShowConsole(false);
		}

		if (Window::GetKeyboard()->KeyPressed(KEYBOARD_HOME)) {
			w->SetFullScreen(true);
		}
		if (Window::GetKeyboard()->KeyPressed(KEYBOARD_END)) {
			w->SetFullScreen(false);
		}

		if (Window::GetKeyboard()->KeyDown(KEYBOARD_A))
		{
			viewPosition.x += mvmt;
		}
		if (Window::GetKeyboard()->KeyDown(KEYBOARD_D))
		{
			viewPosition.x -= mvmt;
		}

		if (Window::GetKeyboard()->KeyDown(KEYBOARD_SHIFT))
		{
			viewPosition.y -= mvmt;
		}
		if (Window::GetKeyboard()->KeyDown(KEYBOARD_SPACE))
		{
			viewPosition.y += mvmt;
		}

		if (Window::GetKeyboard()->KeyDown(KEYBOARD_W))
		{
			viewPosition.z += mvmt;
		}
		if (Window::GetKeyboard()->KeyDown(KEYBOARD_S))
		{
			viewPosition.z -= mvmt;
		}

		/***** RESET *****/

		if (Window::GetKeyboard()->KeyPressed(KEYBOARD_R))
		{
			renderer->ResetGameClock();
			cube->SetShader(new OGLShader("CW2_VertexShader.glsl", "CW2_FragmentShader.glsl", "", "CW2_TCS.glsl", "CW2_TES.glsl"));

			vecLaserQuads[0]->SetShader(new OGLShader("CW2_VertexShader_LaserQuads_Hide.glsl", "CW2_FragmentShader_LaserBase_Hide.glsl"));
			vecLaserQuads[1]->SetShader(new OGLShader("CW2_VertexShader_LaserQuads_Hide.glsl", "CW2_FragmentShader_LaserBase_Hide.glsl"));
		}

		/***** BASIC DESTRUCTION *****/

		if (Window::GetKeyboard()->KeyPressed(KEYBOARD_F1))
		{
			// shrink!
			cube->SetBaseTexture(OGLTexture::RGBATextureFromFilename(texCrater));
			cube->SetSecondTexture(OGLTexture::RGBATextureFromFilename(texClean));

			renderer->ResetGameClock();
			cube->SetShader(new OGLShader("CW2_VertexShader.glsl", "CW2_FragmentShader.glsl", "", "CW2_TCS.glsl", "CW2_TES_ShrinkOverTime.glsl"));
			
			vecLaserQuads[0]->SetShader(new OGLShader("CW2_VertexShader_LaserQuads_Hide.glsl", "CW2_FragmentShader_LaserBase_Hide.glsl"));
			vecLaserQuads[1]->SetShader(new OGLShader("CW2_VertexShader_LaserQuads_Hide.glsl", "CW2_FragmentShader_LaserBase_Hide.glsl"));
		}
		if (Window::GetKeyboard()->KeyPressed(KEYBOARD_F2))
		{
			// blend textures!
			cube->SetBaseTexture(OGLTexture::RGBATextureFromFilename(texClean));
			cube->SetSecondTexture(OGLTexture::RGBATextureFromFilename(texBlood));

			renderer->ResetGameClock();
			cube->SetShader(new OGLShader("CW2_VertexShader.glsl", "CW2_FragmentShader_TextureBlend.glsl", "", "CW2_TCS.glsl", "CW2_TES.glsl"));
			
			vecLaserQuads[0]->SetShader(new OGLShader("CW2_VertexShader_LaserQuads_Hide.glsl", "CW2_FragmentShader_LaserBase_Hide.glsl"));
			vecLaserQuads[1]->SetShader(new OGLShader("CW2_VertexShader_LaserQuads_Hide.glsl", "CW2_FragmentShader_LaserBase_Hide.glsl"));
		}
		if (Window::GetKeyboard()->KeyPressed(KEYBOARD_F3))
		{
			// make transparent!
			cube->SetBaseTexture(OGLTexture::RGBATextureFromFilename(texCrater));
			cube->SetSecondTexture(OGLTexture::RGBATextureFromFilename(texClean));

			renderer->ResetGameClock();
			cube->SetShader(new OGLShader("CW2_VertexShader.glsl", "CW2_FragmentShader_Transparency.glsl", "", "CW2_TCS.glsl", "CW2_TES.glsl"));
			
			vecLaserQuads[0]->SetShader(new OGLShader("CW2_VertexShader_LaserQuads_Hide.glsl", "CW2_FragmentShader_LaserBase_Hide.glsl"));
			vecLaserQuads[1]->SetShader(new OGLShader("CW2_VertexShader_LaserQuads_Hide.glsl", "CW2_FragmentShader_LaserBase_Hide.glsl"));
		}

		/***** SLIGHTLY MORE ADVANCED DESTRUCTION *****/

		if (Window::GetKeyboard()->KeyPressed(KEYBOARD_F4))
		{
			// jack out!
			cube->SetBaseTexture(OGLTexture::RGBATextureFromFilename(texGlowDots));
			cube->SetSecondTexture(OGLTexture::RGBATextureFromFilename(texClean));
			cube->SetThirdTexture(OGLTexture::RGBATextureFromFilename(transparentGrid));

			renderer->ResetGameClock();
			cube->SetShader(new OGLShader("CW2_VertexShader_Unplug.glsl", "CW2_FragmentShader_Unplug.glsl", "", "CW2_TCS.glsl", "CW2_TES.glsl"));
		
			vecLaserQuads[0]->SetShader(new OGLShader("CW2_VertexShader_LaserQuads_Hide.glsl", "CW2_FragmentShader_LaserBase_Hide.glsl"));
			vecLaserQuads[1]->SetShader(new OGLShader("CW2_VertexShader_LaserQuads_Hide.glsl", "CW2_FragmentShader_LaserBase_Hide.glsl"));
		}
		if (Window::GetKeyboard()->KeyPressed(KEYBOARD_F5))
		{
			// shatter!
			cube->SetBaseTexture(OGLTexture::RGBATextureFromFilename(texCrater));
			cube->SetSecondTexture(OGLTexture::RGBATextureFromFilename(texClean));

			renderer->ResetGameClock();
			cube->SetShader(new OGLShader("CW2_VertexShader_Explode.glsl", "CW2_FragmentShader.glsl", "CW2_GeometryShader_Explode.glsl", "CW2_TCS_Explode.glsl", "CW2_TES_Explode.glsl"));
		
			vecLaserQuads[0]->SetShader(new OGLShader("CW2_VertexShader_LaserQuads_Hide.glsl", "CW2_FragmentShader_LaserBase_Hide.glsl"));
			vecLaserQuads[1]->SetShader(new OGLShader("CW2_VertexShader_LaserQuads_Hide.glsl", "CW2_FragmentShader_LaserBase_Hide.glsl"));
		}
		if (Window::GetKeyboard()->KeyPressed(KEYBOARD_F6))
		{
			// laser!
			cube->SetBaseTexture(OGLTexture::RGBATextureFromFilename(texCrater));
			cube->SetSecondTexture(OGLTexture::RGBATextureFromFilename(texClean));

			renderer->ResetGameClock();

			cube->SetShader(new OGLShader("CW2_VertexShader_Laser.glsl", "CW2_FragmentShader_Laser.glsl", "", "CW2_TCS_Laser.glsl", "CW2_TES_Laser.glsl"));
		
			vecLaserQuads[0]->SetShader(new OGLShader("CW2_VertexShader_LaserQuads.glsl", "CW2_FragmentShader_LaserBase.glsl"));
			vecLaserQuads[1]->SetShader(new OGLShader("CW2_VertexShader_LaserQuads.glsl", "CW2_FragmentShader_LaserBase.glsl"));
		}

		// mouse movement handling
		vecMousePosition += Window::GetMouse()->GetRelativePosition();
		Matrix4 rot = Matrix4::Rotation
		(
			vecMousePosition.x, Vector3(0.0, 1.0, 0.0)
		)
		* Matrix4::Rotation
		(
			vecMousePosition.y, Vector3(1.0, 0.0, 0.0)
		);
		
		renderer->SetViewMatrix(Matrix4::Translation(viewPosition) * rot);

		w->SetTitle(std::to_string(time));
	}

	delete renderer;

	Window::DestroyGameWindow();
}