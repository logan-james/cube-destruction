#pragma once
#include "../../Plugins/OpenGLRendering/OGLRenderer.h"
#include "../../Plugins/OpenGLRendering/OGLShader.h"
#include "../../Plugins/OpenGLRendering/OGLTexture.h"
#include "../../Plugins/OpenGLRendering/OGLMesh.h"

#include "RenderObject.h"
#include "../../Common/Vector2.h"
#include "../../Common/Camera.h"

namespace NCL
{
	namespace CSC3223
	{
		struct Light
		{
			Vector3 position;
			float radius;
			Vector3 colour;
		};

		class Renderer : public OGLRenderer
		{
		public:
			Renderer(Window& w);
			~Renderer();

			void AddRenderObject(RenderObject* ro) {
				renderObjects.emplace_back(ro);
			}

			void DeleteAllRenderObjects() {
				for (const RenderObject* object : renderObjects) {
					delete object;
				}
				renderObjects.clear();
			}
			
			void DeleteRenderObject(RenderObject* ro)
			{
				renderObjects.erase(std::remove(renderObjects.begin(), renderObjects.end(), ro), renderObjects.end());
				delete ro;
			}
			
			void SetProjectionMatrix(const Matrix4& m) {
				projMatrix = m;
			}

			void SetViewMatrix(const Matrix4& m) {
				viewMatrix = m;
			}

			const bool& GetWireframe() const { return _isWireframe; }
			void SetWireframe(const bool& wire) { _isWireframe = wire; }

			void EnableDepthBuffer(bool state)
			{
				if (state)
				{
					glEnable(GL_DEPTH_TEST);
				}
				else
				{
					glDisable(GL_DEPTH_TEST);
				}
			}

			void EnableAlphaBlending(bool state)
			{
				if (state)
				{
					glEnable(GL_BLEND);
				}
				else
				{
					glDisable(GL_BLEND);
				}
			}

			void WriteDepthBuffer(const string& filepath) const;
			void ResetGameClock();

			void SetBlendToLinear();
			void SetBlendToAdditive();
			void SetBlendToInvert();

			void SetDepthFunctionToNever();
			void SetDepthFunctionToNotEqual();
			void SetDepthFunctionToLequal();

			void ToggleDepth();
			void ToggleAlphaBlend();

			void  EnableBilinearFiltering(OGLTexture& t);
			void  EnableMipMapFiltering(OGLTexture& t);
			void  EnableTextureRepeating(OGLTexture& t, bool  uRepeat, bool  vRepeat);

			void SetLightProperties(Vector3 pos, Vector3 colour, float radius);
		private:
			bool _isWireframe;
		protected:
			void RenderNode(RenderObject* root);

			GameTimer frameTimer;

			void OnWindowResize(int w, int h)	override;

			void RenderFrame()	override;
			OGLShader* defaultShader;


			Matrix4 projMatrix;
			Matrix4	viewMatrix;

			bool modifyObject;
			bool usingDepth;
			bool usingAlpha;
			int blendMode;
			float timeOffset;
			vector<RenderObject*> renderObjects;

			void ApplyLightToShader(const Light& l, const OGLShader* s);
			Light activeLight;
		};
	}
}
