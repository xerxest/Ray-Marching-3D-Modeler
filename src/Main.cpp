/*
 * Copyright 2011-2023 Branimir Karadzic. All rights reserved.
 * License: https://github.com/bkaradzic/bgfx/blob/master/LICENSE
 */

#include "common.h"
#include "bgfx_utils.h"
#include "imgui/imgui.h"
#include "entry/cmd.h"
#include "ViewPortCamera.h"
#include "ShaderBuilder.h"
#include "UI/Panel.h"
#include "UI/ToolBar.h"
#include "UI/TreeView.h"
#include "UI/NodeProperties.h"
#include "UI/Help.h"
#include "SDFTree.h"

namespace
{
	struct PosColorTexCoord0Vertex
	{
		float m_x;
		float m_y;
		float m_z;
		uint32_t m_abgr;
		float m_u;
		float m_v;

		static void init()
		{
			ms_layout
				.begin()
				.add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
				.add(bgfx::Attrib::Color0, 4, bgfx::AttribType::Uint8, true)
				.add(bgfx::Attrib::TexCoord0, 2, bgfx::AttribType::Float)
				.end();
		}

		static bgfx::VertexLayout ms_layout;
	};

	bgfx::VertexLayout PosColorTexCoord0Vertex::ms_layout;

	int getNumOfVec4(int length)
	{
		if (length % 4 == 0)
		{
			return length / 4;
		}

		return (int(length / 4)) + 4;
	}

	void renderScreenSpaceQuad(uint8_t _view, bgfx::ProgramHandle _program, float _x, float _y, float _width, float _height)
	{
		bgfx::TransientVertexBuffer tvb;
		bgfx::TransientIndexBuffer tib;

		if (bgfx::allocTransientBuffers(&tvb, PosColorTexCoord0Vertex::ms_layout, 4, &tib, 6))
		{
			PosColorTexCoord0Vertex *vertex = (PosColorTexCoord0Vertex *)tvb.data;

			float zz = 0.0f;

			const float minx = _x;
			const float maxx = _x + _width;
			const float miny = _y;
			const float maxy = _y + _height;

			float minu = -1.0f;
			float minv = -1.0f;
			float maxu = 1.0f;
			float maxv = 1.0f;

			vertex[0].m_x = minx;
			vertex[0].m_y = miny;
			vertex[0].m_z = zz;
			vertex[0].m_abgr = 0xff0000ff;
			vertex[0].m_u = minu;
			vertex[0].m_v = minv;

			vertex[1].m_x = maxx;
			vertex[1].m_y = miny;
			vertex[1].m_z = zz;
			vertex[1].m_abgr = 0xff00ff00;
			vertex[1].m_u = maxu;
			vertex[1].m_v = minv;

			vertex[2].m_x = maxx;
			vertex[2].m_y = maxy;
			vertex[2].m_z = zz;
			vertex[2].m_abgr = 0xffff0000;
			vertex[2].m_u = maxu;
			vertex[2].m_v = maxv;

			vertex[3].m_x = minx;
			vertex[3].m_y = maxy;
			vertex[3].m_z = zz;
			vertex[3].m_abgr = 0xffffffff;
			vertex[3].m_u = minu;
			vertex[3].m_v = maxv;

			uint16_t *indices = (uint16_t *)tib.data;

			indices[0] = 0;
			indices[1] = 2;
			indices[2] = 1;
			indices[3] = 0;
			indices[4] = 3;
			indices[5] = 2;

			bgfx::setState(BGFX_STATE_DEFAULT);
			bgfx::setIndexBuffer(&tib);
			bgfx::setVertexBuffer(0, &tvb);
			bgfx::submit(_view, _program);
		}
	}

	class RayModel : public entry::AppI
	{
	public:
		RayModel(const char *_name, const char *_description, const char *_url)
			: entry::AppI(_name, _description, _url)
		{
		}

		void init(int32_t _argc, const char *const *_argv, uint32_t _width, uint32_t _height) override
		{
			Args args(_argc, _argv);

			m_width = _width;
			m_height = _height;
			m_debug = BGFX_DEBUG_NONE;
			m_reset = BGFX_RESET_VSYNC;

			bgfx::Init init;
			init.type = args.m_type;
			init.vendorId = args.m_pciId;
			init.platformData.nwh = entry::getNativeWindowHandle(entry::kDefaultWindowHandle);
			init.platformData.ndt = entry::getNativeDisplayHandle();
			init.platformData.type = entry::getNativeWindowHandleType();
			init.resolution.width = m_width;
			init.resolution.height = m_height;
			init.resolution.reset = m_reset;
			bgfx::init(init);


			Panel::m_screenWidth = m_width;
			Panel::m_screenHeight = m_height;

			shaderBuilder->createDefaultScene();
			nodeProperties.setShader(shaderBuilder);
			toolBar.setShader(shaderBuilder);
			// Enable debug text.
			bgfx::setDebug(m_debug);

			m_mouseState.m_mx = 643;
			m_mouseState.m_my = 361;
			m_mouseState.m_mz = 0;

			// Set view 0 clear state.
			bgfx::setViewClear(0, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0x303030ff, 1.0f, 0);

			// Create vertex stream declaration.
			PosColorTexCoord0Vertex::init();

			u_mtx = bgfx::createUniform("u_mtx", bgfx::UniformType::Mat4);
			u_lightDirTime = bgfx::createUniform("u_lightDirTime", bgfx::UniformType::Vec4);

			bgfx::UniformInfo test_info;

			bgfx::getUniformInfo(u_params_test, test_info);

			m_program = loadProgram("vs_raymarching", "fs_raymarching");

			m_timeOffset = bx::getHPCounter();

			ViewPortCamera::cameraCreate();
			ViewPortCamera::cameraSetPosition({-9.2916f, -6.87753f, -9.2916f});

			imguiCreate();
			Panel::setStyle();
		}

		int shutdown() override
		{
			imguiDestroy();
			ViewPortCamera::cameraDestroy();

			// Cleanup.
			bgfx::destroy(m_program);

			bgfx::destroy(u_mtx);
			bgfx::destroy(u_lightDirTime);
			bgfx::destroy(u_params_test);

			// Shutdown bgfx.
			bgfx::shutdown();

			delete (treeView);

			return 0;
		}

		bool update() override
		{
			if (!entry::processEvents(m_width, m_height, m_debug, m_reset, &m_mouseState))
			{
				ImGuiIO &io = ImGui::GetIO();
				const auto *data = inputGetChar();
				const auto character = data != nullptr ? 0u [data] : 0u;
				io.AddInputCharacter(character);
				io.AddKeyEvent(ImGuiKey_Backspace,(character == 127));

				int64_t now = bx::getHPCounter();
				static int64_t last = now;
				const int64_t frameTime = now - last;
				last = now;
				const double freq = double(bx::getHPFrequency());
				const float deltaTime = float(frameTime / freq);

				imguiBeginFrame(m_mouseState.m_mx, m_mouseState.m_my, (m_mouseState.m_buttons[entry::MouseButton::Left] ? IMGUI_MBUT_LEFT : 0) | (m_mouseState.m_buttons[entry::MouseButton::Right] ? IMGUI_MBUT_RIGHT : 0) | (m_mouseState.m_buttons[entry::MouseButton::Middle] ? IMGUI_MBUT_MIDDLE : 0), m_mouseState.m_mz, uint16_t(m_width), uint16_t(m_height));

				showExampleDialog(this);
				// ImGui::ShowDemoWindow();

				Panel::m_screenWidth = m_width;
				Panel::m_screenHeight = m_height;

				// UI
				treeView->OnImGuiRender();

				toolBar.setSelectedNode(treeView->getSelectedNode());
				toolBar.OnImGuiRender();

				nodeProperties.setSelected(treeView->getSelectedNode());
				nodeProperties.OnImGuiRender();

				helpMenu.OnImGuiRender();

				imguiEndFrame();
				// Set view 0 default viewport.
				bgfx::setViewRect(0, 0, 0, uint16_t(m_width), uint16_t(m_height));

				// Set view 1 default viewport.
				bgfx::setViewRect(1, 0, 0, uint16_t(m_width), uint16_t(m_height));

				// This dummy draw call is here to make sure that view 0 is cleared
				// if no other draw calls are submitted to viewZ 0.
				bgfx::touch(0);

				float proj[16];
				const bgfx::Caps *caps = bgfx::getCaps();
				bx::mtxProj(proj, 60.0f, float(m_width) / float(m_height), 0.1f, 100.0f, caps->homogeneousDepth);

				float ortho[16];
				bx::mtxOrtho(ortho, 0.0f, 1280.0f, 720.0f, 0.0f, 0.0f, 100.0f, 0.0, caps->homogeneousDepth);

				// Set view and projection matrix for view 0.
				bgfx::setViewTransform(1, NULL, ortho);

				float time = (float)((bx::getHPCounter() - m_timeOffset) / double(bx::getHPFrequency()));

				float vp[16];

				ViewPortCamera::cameraGetViewMtx(m_viewMtx);

				bx::mtxMul(vp, m_viewMtx, proj);

				float lightDirTime[4];
				const bx::Vec3 lightDirModelN = bx::normalize(bx::Vec3{-0.4f, -0.5f, -1.0f});
				bx::store(lightDirTime, lightDirModelN);
				lightDirTime[3] = 1.0f;
				bgfx::setUniform(u_lightDirTime, lightDirTime);
				bgfx::setUniform(u_params_test, test_buffer, 6);

				float invMvp[16];
				bx::mtxInverse(invMvp, vp);
				bgfx::setUniform(u_mtx, invMvp);
				if (shaderBuilder->updateScene())
				{
					m_program = loadProgram("vs_raymarching", "fs_raymarching");
				}

				ViewPortCamera::cameraUpdate(deltaTime, m_mouseState, ImGui::MouseOverArea());

				renderScreenSpaceQuad(1, m_program, 0.0f, 0.0f, 1280.0f, 720.0f);

				// Advance to next frame. Rendering thread will be kicked to
				// process submitted rendering primitives.
				bgfx::frame();
				return true;
			}

			return false;
		}

		float m_camX = 0;
		float m_camY = 0;
		float m_camz = 0;

		entry::MouseState m_mouseState;
		InputBinding keyPress;

		uint32_t m_width;
		uint32_t m_height;
		uint32_t m_debug;
		uint32_t m_reset;

		float m_viewMtx[16];
		bgfx::UniformHandle u_size;

		float posData[4 * 2];

		int64_t m_timeOffset;
		bgfx::UniformHandle u_mtx;
		bgfx::UniformHandle u_lightDirTime;
		bgfx::ProgramHandle m_program;
		bgfx::UniformHandle u_params_test;
		float test_buffer[6 * 4];
		std::shared_ptr<SDFTree> scence = std::make_shared<SDFTree>();
		ShaderBuilder *shaderBuilder = new ShaderBuilder(scence);

		// UI
		ToolBar toolBar;
		TreeView *treeView = new TreeView(scence);
		NodeProperties nodeProperties;
		Help helpMenu;
	};

} // namespace

ENTRY_IMPLEMENT_MAIN(
	RayModel, "RayModel", "", "");

int _main_(int _argc, char **_argv) { return 0; };
