#include "Renderer.h"
#include "../../Common/TextureWriter.h"
#include "../../Common/Maths.h"
#include "../../Common/Matrix3.h"
using namespace NCL;
using namespace Rendering;
using namespace CSC3223;

Renderer::Renderer(Window& w) : OGLRenderer(w)
{
	defaultShader = new OGLShader("rasterisationVert.glsl", "rasterisationFrag.glsl");
	projMatrix = Matrix4::Orthographic(-1.0f, 1.0f, (float)currentWidth, 0.0f, 0.0f, (float)currentHeight);

	usingDepth = false;
	usingAlpha = false;
	blendMode = 0;
	modifyObject = true;
	timeOffset = 0;

	frameTimer = GameTimer();
}

Renderer::~Renderer()
{
	delete defaultShader;
}

void Renderer::ResetGameClock()
{
	frameTimer.resetTime();
	timeOffset = frameTimer.GetTotalTimeMSec();
}

void Renderer::RenderFrame() {
	OGLShader* activeShader = nullptr;

	int modelLocation = 0;

	for (const RenderObject* object : renderObjects) {
		OGLShader* objectShader = (OGLShader*)object->GetShader();
		if (!object->GetMesh()) {
			continue;
		}
		if (objectShader == nullptr) {
			objectShader = defaultShader;
		}
		if (objectShader != activeShader) {
			activeShader = objectShader;
			BindShader(activeShader);

			int projLocation = glGetUniformLocation(activeShader->GetProgramID(), "projMatrix");
			int viewLocation = glGetUniformLocation(activeShader->GetProgramID(), "viewMatrix");
			modelLocation = glGetUniformLocation(activeShader->GetProgramID(), "modelMatrix");
			int camLocation = glGetUniformLocation(activeShader->GetProgramID(), "cameraPos");

			glUniformMatrix4fv(projLocation, 1, false, (float*)& projMatrix);
			glUniformMatrix4fv(viewLocation, 1, false, (float*)& viewMatrix);
			ApplyLightToShader(activeLight, activeShader);

			Matrix3 rotation = Matrix3(viewMatrix);
			Vector3 invCamPos = viewMatrix.GetPositionVector();
			Vector3 camPos = rotation * -invCamPos; // view mat is an 'inverse'
			glUniform3fv(camLocation, 1, (float*)& camPos);

			int timeLocation = glGetUniformLocation(activeShader->GetProgramID(), "time");
			if (timeLocation >= 0)
			{
				// float totalTime =  fmod(frameTimer.GetTotalTimeMSec(), 100);
				float totalTime = frameTimer.GetTotalTimeMSec() - timeOffset;
				glUniform1f(timeLocation, totalTime);
			}
		}

		Matrix4 mat = object->GetTransform();
		glUniformMatrix4fv(modelLocation, 1, false, (float*)& mat);

		BindTextureToShader(object->GetBaseTexture(), "mainTex", 0);
		BindTextureToShader(object->GetSecondTexture(), "secondTex", 1);
		BindTextureToShader(object->GetThirdTexture(), "thirdTex", 2);
		BindTextureToShader(object->GetCubeMapTexture(), "mainCubeTex", 3);
		BindTextureToShader(object->GetSecondCubeMapTexture(), "secondCubeTex", 4);
		BindMesh(object->GetMesh());
		DrawBoundMesh();
	}

	glPolygonMode(GL_FRONT_AND_BACK, GetWireframe() ? GL_LINE : GL_FILL);
}

void Renderer::ApplyLightToShader(const Light& light, const OGLShader* shader)
{
	glUniform3fv
	(
		glGetUniformLocation(shader->GetProgramID(), "lightColour")
		, 1
		, (float*)& light.colour
	);

	glUniform3fv
	(
		glGetUniformLocation(shader->GetProgramID(), "lightPos")
		, 1
		, (float*)& light.position
	);

	glUniform1f
	(
		glGetUniformLocation(shader->GetProgramID(), "lightRadius")
		, light.radius
	);
}

void Renderer::WriteDepthBuffer(const string& filepath) const
{
	float* data = new float[currentWidth * currentHeight];
	glReadPixels(0, 0, currentWidth, currentHeight, GL_DEPTH_COMPONENT, GL_FLOAT, data);

	char* pixels = new  char[currentWidth * currentHeight * 3];
	char* pixelPointer = pixels;

	for (int y = 0; y < currentHeight; ++y)
	{
		for (int x = 0; x < currentWidth; ++x)
		{
			float  depthValue = data[(y * currentWidth) + x];
			float  mult = 1.0f / 0.333f;

			float redAmount = Maths::Clamp(depthValue, 0.0f, 0.333f) * mult;
			float greenAmount = (Maths::Clamp(depthValue, 0.333f, 0.666f) - 0.333f) * mult;
			float blueAmount = (Maths::Clamp(depthValue, 0.666f, 1.0f) - 0.666f) * mult;

			unsigned char redByte = (char)(redAmount * 255);
			unsigned char greenByte = (char)(greenAmount * 255);
			unsigned char blueByte = (char)(blueAmount * 255);

			*pixelPointer++ = (char)(redAmount * 255);
			*pixelPointer++ = (char)(greenAmount * 255);
			*pixelPointer++ = (char)(blueAmount * 255);
		}
	}

	TextureWriter::WritePNG(filepath, pixels, currentWidth, currentHeight, 3);

	delete[] pixels;
	delete[] data;
}

void Renderer::OnWindowResize(int w, int h) {
	OGLRenderer::OnWindowResize(w, h);
	projMatrix = Matrix4::Orthographic(-1.0f, 1.0f, (float)currentWidth, 0.0f, 0.0f, (float)currentHeight);
}

void Renderer::SetBlendToLinear()
{
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void Renderer::SetBlendToAdditive()
{
	glBlendFunc(GL_ONE, GL_ONE);
}

void Renderer::SetBlendToInvert()
{
	glBlendFunc(GL_ONE_MINUS_SRC_COLOR, GL_ONE_MINUS_DST_COLOR);
}

void Renderer::SetDepthFunctionToNever()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glDepthFunc(GL_NEVER);
}

void Renderer::SetDepthFunctionToNotEqual()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glDepthFunc(GL_NOTEQUAL);
}

void Renderer::SetDepthFunctionToLequal()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glDepthFunc(GL_LEQUAL);
}

void Renderer::ToggleDepth()
{
	usingDepth = !usingDepth;
	usingDepth ? glEnable(GL_DEPTH_TEST) : glDisable(GL_DEPTH_TEST);
}

void Renderer::ToggleAlphaBlend()
{
	usingAlpha = !usingAlpha;
	usingAlpha ? glEnable(GL_BLEND) : glDisable(GL_BLEND);
}

void Renderer::EnableBilinearFiltering(OGLTexture& t)
{
	GLuint  id = t.GetObjectID();
	glBindTexture(GL_TEXTURE_2D, id);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, 0);
}

void Renderer::EnableMipMapFiltering(OGLTexture& t)
{
	GLuint  id = t.GetObjectID();
	glBindTexture(GL_TEXTURE_2D, id);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, 0);
}

void Renderer::EnableTextureRepeating(OGLTexture& t, bool uRepeat, bool vRepeat)
{
	GLuint  id = t.GetObjectID();
	glBindTexture(GL_TEXTURE_2D, id);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, uRepeat ? GL_REPEAT : GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, vRepeat ? GL_REPEAT : GL_CLAMP);

	glBindTexture(GL_TEXTURE_2D, 0);
}

void Renderer::SetLightProperties(Vector3 position, Vector3 colour, float radius)
{
	activeLight.position = position;
	activeLight.colour = colour;
	activeLight.radius = radius;
}
