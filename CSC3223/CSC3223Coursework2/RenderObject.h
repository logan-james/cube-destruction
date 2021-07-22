#pragma once
#include "../../Common/Matrix4.h"
#include "../../Common/TextureBase.h"
#include "../../Common/ShaderBase.h"

#include <vector>
#include <glad\glad.h>
namespace NCL {
	using namespace NCL::Rendering;
	class MeshGeometry;
	namespace CSC3223 {
		using namespace Maths;

		class RenderObject
		{
		public:
			RenderObject(MeshGeometry* mesh, Matrix4 m = Matrix4());
			~RenderObject();

			MeshGeometry* GetMesh() const {
				return mesh;
			}

			void  SetTransform(Matrix4 mat) {
				transform = mat;
			}

			Matrix4 GetTransform() const {
				return transform;
			}

			void SetBaseTexture(TextureBase* t) {
				texture = t;
			}

			void  SetSecondTexture(TextureBase* t)
			{
				secondTexture = t;
			}

			void  SetThirdTexture(TextureBase* t)
			{
				thirdTexture = t;
			}

			void SetCubeMapTexture(GLuint te)
			{
				cubeTexture = te;
			}

			void SetSecondCubeMapTexture(GLuint te)
			{
				secondCubeTexture = te;
			}

			TextureBase* GetBaseTexture() const {
				return texture;
			}

			TextureBase* GetSecondTexture()  const { return  secondTexture; }

			TextureBase* GetThirdTexture()  const { return  thirdTexture; }

			GLuint GetCubeMapTexture()  const { return  cubeTexture; }

			GLuint GetSecondCubeMapTexture()  const { return  secondCubeTexture; }

			void SetShader(ShaderBase* s) {
				shader = s;
			}

			ShaderBase* GetShader() const {
				return shader;
			}
		protected:
			MeshGeometry* mesh;

			TextureBase* texture;
			TextureBase* secondTexture;
			TextureBase* thirdTexture;

			ShaderBase* shader;
			Matrix4		transform;

			GLuint cubeTexture;
			GLuint secondCubeTexture;
		};
	}
}

