#include "ImGUIWrapper.h"
#include "gl/GLApi.h"
#include "imgui.h"

namespace tinyngine
{
	
class ImGUIWrapperImpl : public ImGUIWrapper {
public:
	ImGUIWrapperImpl() {
		CreateDeviceObjects();
	}

	virtual ~ImGUIWrapperImpl() {
		DestroyDeviceObjects();
	}

	void ImGUIWrapperImpl::BeginFrame(MouseState& mouseState, int32_t windowWidth, int32_t windowHeight) override {
		ImGuiIO& io = ImGui::GetIO();
		io.DisplaySize = ImVec2((float)windowWidth, (float)windowHeight);
		io.DisplayFramebufferScale = ImVec2(1.0f, 1.0f);

		io.DeltaTime = 1.0f / 60.0f;

		io.MousePos = ImVec2(static_cast<float>(mouseState.mPosX), static_cast<float>(mouseState.mPosY));
		for (int n = 0; n < 3; n++) {
			io.MouseDown[n] = mouseState.mButtons[n];
		}

		ImGui::NewFrame();
	}

	void ImGUIWrapperImpl::EndFrame() override {
		ImGui::Render();

		ImDrawData* drawData = ImGui::GetDrawData();

		ImGuiIO& io = ImGui::GetIO();
		int fb_width = (int)(io.DisplaySize.x * io.DisplayFramebufferScale.x);
		int fb_height = (int)(io.DisplaySize.y * io.DisplayFramebufferScale.y);
		if (fb_width == 0 || fb_height == 0)
			return;
		drawData->ScaleClipRects(io.DisplayFramebufferScale);

		GLenum last_active_texture; glGetIntegerv(GL_ACTIVE_TEXTURE, (GLint*)&last_active_texture);
		glActiveTexture(GL_TEXTURE0);
		GLint last_program; glGetIntegerv(GL_CURRENT_PROGRAM, &last_program);
		GLint last_texture; glGetIntegerv(GL_TEXTURE_BINDING_2D, &last_texture);
		GLint last_array_buffer; glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &last_array_buffer);
		GLint last_element_array_buffer; glGetIntegerv(GL_ELEMENT_ARRAY_BUFFER_BINDING, &last_element_array_buffer);
		GLint last_viewport[4]; glGetIntegerv(GL_VIEWPORT, last_viewport);
		GLint last_scissor_box[4]; glGetIntegerv(GL_SCISSOR_BOX, last_scissor_box);
		GLenum last_blend_src_rgb; glGetIntegerv(GL_BLEND_SRC_RGB, (GLint*)&last_blend_src_rgb);
		GLenum last_blend_dst_rgb; glGetIntegerv(GL_BLEND_DST_RGB, (GLint*)&last_blend_dst_rgb);
		GLenum last_blend_src_alpha; glGetIntegerv(GL_BLEND_SRC_ALPHA, (GLint*)&last_blend_src_alpha);
		GLenum last_blend_dst_alpha; glGetIntegerv(GL_BLEND_DST_ALPHA, (GLint*)&last_blend_dst_alpha);
		GLenum last_blend_equation_rgb; glGetIntegerv(GL_BLEND_EQUATION_RGB, (GLint*)&last_blend_equation_rgb);
		GLenum last_blend_equation_alpha; glGetIntegerv(GL_BLEND_EQUATION_ALPHA, (GLint*)&last_blend_equation_alpha);
		GLboolean last_enable_blend = glIsEnabled(GL_BLEND);
		GLboolean last_enable_cull_face = glIsEnabled(GL_CULL_FACE);
		GLboolean last_enable_depth_test = glIsEnabled(GL_DEPTH_TEST);
		GLboolean last_enable_scissor_test = glIsEnabled(GL_SCISSOR_TEST);

		glEnable(GL_BLEND);
		glBlendEquation(GL_FUNC_ADD);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glDisable(GL_CULL_FACE);
		glDisable(GL_DEPTH_TEST);
		glEnable(GL_SCISSOR_TEST);

		glViewport(0, 0, (GLsizei)fb_width, (GLsizei)fb_height);
		const float ortho_projection[4][4] = {
			{ 2.0f / io.DisplaySize.x, 0.0f,                   0.0f, 0.0f },
			{ 0.0f,                  2.0f / -io.DisplaySize.y, 0.0f, 0.0f },
			{ 0.0f,                  0.0f,                  -1.0f, 0.0f },
			{ -1.0f,                  1.0f,                   0.0f, 1.0f },
		};
		glUseProgram(mProgramHandle);
		glUniform1i(mAttribLocationTex, 0);
		glUniformMatrix4fv(mAttribLocationModelViewPrj, 1, GL_FALSE, &ortho_projection[0][0]);

		glBindBuffer(GL_ARRAY_BUFFER, mVertexBufferHandle);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIndexBufferHandle);
		glEnableVertexAttribArray(mAttribLocationPosition);
		glEnableVertexAttribArray(mAttribLocationTexcoord);
		glEnableVertexAttribArray(mAttribLocationColor);
#define OFFSETOF(TYPE, ELEMENT) ((size_t)&(((TYPE *)0)->ELEMENT))
		glVertexAttribPointer(mAttribLocationPosition, 2, GL_FLOAT, GL_FALSE, sizeof(ImDrawVert), (GLvoid*)OFFSETOF(ImDrawVert, pos));
		glVertexAttribPointer(mAttribLocationTexcoord, 2, GL_FLOAT, GL_FALSE, sizeof(ImDrawVert), (GLvoid*)OFFSETOF(ImDrawVert, uv));
		glVertexAttribPointer(mAttribLocationColor, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(ImDrawVert), (GLvoid*)OFFSETOF(ImDrawVert, col));
#undef OFFSETOF

		for (int n = 0; n < drawData->CmdListsCount; n++)
		{
			const ImDrawList* cmd_list = drawData->CmdLists[n];
			const ImDrawIdx* idx_buffer_offset = 0;

			glBufferData(GL_ARRAY_BUFFER, (GLsizeiptr)cmd_list->VtxBuffer.Size * sizeof(ImDrawVert), (const GLvoid*)cmd_list->VtxBuffer.Data, GL_STREAM_DRAW);

			glBufferData(GL_ELEMENT_ARRAY_BUFFER, (GLsizeiptr)cmd_list->IdxBuffer.Size * sizeof(ImDrawIdx), (const GLvoid*)cmd_list->IdxBuffer.Data, GL_STREAM_DRAW);

			for (int cmd_i = 0; cmd_i < cmd_list->CmdBuffer.Size; cmd_i++)
			{
				const ImDrawCmd* pcmd = &cmd_list->CmdBuffer[cmd_i];
				if (pcmd->UserCallback)
				{
					pcmd->UserCallback(cmd_list, pcmd);
				}
				else
				{
					glBindTexture(GL_TEXTURE_2D, (GLuint)(intptr_t)pcmd->TextureId);
					glScissor((int)pcmd->ClipRect.x, (int)(fb_height - pcmd->ClipRect.w), (int)(pcmd->ClipRect.z - pcmd->ClipRect.x), (int)(pcmd->ClipRect.w - pcmd->ClipRect.y));
					glDrawElements(GL_TRIANGLES, (GLsizei)pcmd->ElemCount, sizeof(ImDrawIdx) == 2 ? GL_UNSIGNED_SHORT : GL_UNSIGNED_INT, idx_buffer_offset);
				}
				idx_buffer_offset += pcmd->ElemCount;
			}
		}

		glUseProgram(last_program);
		glBindTexture(GL_TEXTURE_2D, last_texture);
		glActiveTexture(last_active_texture);
		glBindBuffer(GL_ARRAY_BUFFER, last_array_buffer);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, last_element_array_buffer);
		glBlendEquationSeparate(last_blend_equation_rgb, last_blend_equation_alpha);
		glBlendFuncSeparate(last_blend_src_rgb, last_blend_dst_rgb, last_blend_src_alpha, last_blend_dst_alpha);
		if (last_enable_blend) glEnable(GL_BLEND); else glDisable(GL_BLEND);
		if (last_enable_cull_face) glEnable(GL_CULL_FACE); else glDisable(GL_CULL_FACE);
		if (last_enable_depth_test) glEnable(GL_DEPTH_TEST); else glDisable(GL_DEPTH_TEST);
		if (last_enable_scissor_test) glEnable(GL_SCISSOR_TEST); else glDisable(GL_SCISSOR_TEST);
		glViewport(last_viewport[0], last_viewport[1], (GLsizei)last_viewport[2], (GLsizei)last_viewport[3]);
		glScissor(last_scissor_box[0], last_scissor_box[1], (GLsizei)last_scissor_box[2], (GLsizei)last_scissor_box[3]);
	}

	void AddInputCharacter(uint32_t ch) override {
		ImGuiIO& io = ImGui::GetIO();
		if (ch > 0 && ch < 0x10000) {
			io.AddInputCharacter(static_cast<ImWchar>(ch));
		}
	}

private:
	void CreateDeviceObjects() {
		ImGuiIO& io = ImGui::GetIO();
		io.RenderDrawListsFn = nullptr;

		GLint last_texture, last_array_buffer;
		glGetIntegerv(GL_TEXTURE_BINDING_2D, &last_texture);
		glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &last_array_buffer);

		const GLchar *vertex_shader = SHADER_SOURCE
		(
			attribute highp vec2 a_position;
			attribute lowp vec4 a_color;
			attribute mediump vec2 a_texcoord;
			uniform highp mat4 u_modelViewProj;
			varying lowp vec4 v_color;
			varying mediump vec2 v_texcoord;
			void main(void)
			{
				gl_Position = u_modelViewProj * vec4(a_position.xy, 0.0, 1.0);
				v_color = a_color;
				v_texcoord = a_texcoord;
			}
		);
		const GLchar* fragment_shader = SHADER_SOURCE
		(
			varying lowp vec4 v_color;
			varying mediump vec2 v_texcoord;
			uniform sampler2D u_texture;
			void main(void)
			{
				gl_FragColor = v_color * texture2D(u_texture, v_texcoord);
			}
		);

		mProgramHandle = glCreateProgram();
		GLuint vertexShaderHandle = glCreateShader(GL_VERTEX_SHADER);
		GLuint fragmentShaderHandle = glCreateShader(GL_FRAGMENT_SHADER);
		GL_CHECK(glShaderSource(vertexShaderHandle, 1, (const char**)&vertex_shader, 0));
		GL_CHECK(glShaderSource(fragmentShaderHandle, 1, (const char**)&fragment_shader, 0));
		GL_CHECK(glCompileShader(vertexShaderHandle));
		GL_CHECK(glCompileShader(fragmentShaderHandle));
		GL_CHECK(glAttachShader(mProgramHandle, vertexShaderHandle));
		GL_CHECK(glAttachShader(mProgramHandle, fragmentShaderHandle));
		GL_CHECK(glLinkProgram(mProgramHandle));
		GL_CHECK(glDeleteShader(vertexShaderHandle));
		GL_CHECK(glDeleteShader(fragmentShaderHandle));

		mAttribLocationTex = glGetUniformLocation(mProgramHandle, "u_texture");
		mAttribLocationModelViewPrj = glGetUniformLocation(mProgramHandle, "u_modelViewProj");
		mAttribLocationPosition = glGetAttribLocation(mProgramHandle, "a_position");
		mAttribLocationTexcoord = glGetAttribLocation(mProgramHandle, "a_texcoord");
		mAttribLocationColor = glGetAttribLocation(mProgramHandle, "a_color");

		glGenBuffers(1, &mVertexBufferHandle);
		glGenBuffers(1, &mIndexBufferHandle);
		glBindBuffer(GL_ARRAY_BUFFER, last_array_buffer);

		unsigned char* pixels;
		int width, height;
		io.Fonts->GetTexDataAsRGBA32(&pixels, &width, &height);
		glGetIntegerv(GL_TEXTURE_BINDING_2D, &last_texture);
		glGenTextures(1, &mFontTexture);
		glBindTexture(GL_TEXTURE_2D, mFontTexture);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
		io.Fonts->TexID = (void *)(intptr_t)mFontTexture;
		io.Fonts->AddFontFromFileTTF("fonts/DroidSans.ttf", 20.0f);
		glBindTexture(GL_TEXTURE_2D, last_texture);
	}

	void DestroyDeviceObjects() {
		if (mFontTexture) {
			glDeleteTextures(1, &mFontTexture);
			ImGui::GetIO().Fonts->TexID = 0;
			mFontTexture = 0;
		}
		if (mProgramHandle) {
			glDeleteProgram(mProgramHandle);
			mProgramHandle = 0;
		}
		if (mVertexBufferHandle) {
			glDeleteBuffers(1, &mVertexBufferHandle);
			mVertexBufferHandle = 0;
		}
		if (mIndexBufferHandle) {
			glDeleteBuffers(1, &mIndexBufferHandle);
			mIndexBufferHandle = 0;
		}
	}

private:
	GLuint mFontTexture = 0;
	GLuint mVertexBufferHandle = 0;
	GLuint mIndexBufferHandle = 0;
	GLuint mProgramHandle = 0;
	GLint mAttribLocationTex = -1;
	GLint mAttribLocationModelViewPrj = -1;
	GLint mAttribLocationPosition = -1;
	GLint mAttribLocationTexcoord = -1;
	GLint mAttribLocationColor = -1;
};

ImGUIWrapper* CreateImGUIWrapper() {
	return new ImGUIWrapperImpl();
}

} // namespace tinyngine