#include "../pch.h"
#include "../../src/vector.h"
#include "../../src/memory.h"
#include "../../src/offsets.h"
#include "../hex/customFont.h"
#include "../../src/hacks/hacks.h"
#include "../../src/globals.h"
#include "../hex/icons/font_awesome.h"
#include "../hex/icons/font_awesome2.h"

ImFont* h1Font;
ImFont* h2Font;
ImFont* icons_font;
bool pressedPanic = false;
bool memAttached = false;
bool startedThreads;
Memory mem{ "cs2.exe" };

template <typename T>
inline VOID c_overlay::safe_release(T*& p) {
	if (NULL != p) {
		p->Release();
		p = NULL;
	}
}

BOOL c_overlay::init_device() {
	allocConsole();
	DXGI_SWAP_CHAIN_DESC vSwapChainDesc;
	ZeroMemory(&vSwapChainDesc, sizeof(vSwapChainDesc));
	vSwapChainDesc.BufferCount = 2;
	vSwapChainDesc.BufferDesc.Width = 0;
	vSwapChainDesc.BufferDesc.Height = 0;
	vSwapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	vSwapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
	vSwapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	vSwapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
	vSwapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	vSwapChainDesc.OutputWindow = window_handle;
	vSwapChainDesc.SampleDesc.Count = 1;
	vSwapChainDesc.SampleDesc.Quality = 0;
	vSwapChainDesc.Windowed = TRUE;
	vSwapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	D3D_FEATURE_LEVEL vFeatureLevel;
	D3D_FEATURE_LEVEL vFeatureLevelArray[2] = { D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_0, };
	
	if (D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, 0, vFeatureLevelArray, 2, D3D11_SDK_VERSION, &vSwapChainDesc, &swap_chain, &d3d_device, &vFeatureLevel, &device_context) != S_OK)
		return false;

	init_render_target();
	
	ShowWindow(window_handle, SW_SHOWNORMAL);
	UpdateWindow(window_handle);
	return true;
}

VOID c_overlay::dest_device() {
	dest_render_target();
	safe_release(swap_chain);
	safe_release(device_context);
	safe_release(d3d_device);
}

VOID c_overlay::init_imgui() {
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();

	ImGui::StyleColorsClassic();
	ImGuiStyle& style = ImGui::GetStyle();

	style.WindowTitleAlign.x = 0.50f;
	style.WindowPadding = ImVec2(15, 15);
	style.WindowRounding = 4.0f;
	style.FramePadding = ImVec2(2, 2);
	style.FrameRounding = 2.0f;
	style.ItemSpacing = ImVec2(12, 8);
	style.ItemInnerSpacing = ImVec2(8, 6);
	style.IndentSpacing = 25.0f;
	style.ScrollbarSize = 15.0f;
	style.ScrollbarRounding = 9.0f;
	style.GrabMinSize = 5.0f;
	style.GrabRounding = 10.0f;

	style.Colors[ImGuiCol_Text] = ImVec4(0.80f, 0.80f, 0.83f, 1.00f);
	style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
	style.Colors[ImGuiCol_WindowBg] = ImVec4(0.06f, 0.06f, 0.06f, 1.00f);
	style.Colors[ImGuiCol_ChildBg] = ImVec4(0.06f, 0.06f, 0.06f, 1.00f);
	style.Colors[ImGuiCol_PopupBg] = ImVec4(0.043f, 0.043f, 0.043f, 1.00f);
	style.Colors[ImGuiCol_Border] = ImVec4(0.76f, 0.28f, 0.28f, 1.00f);
	style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.043f, 0.043f, 0.043f, 0.00f);
	style.Colors[ImGuiCol_FrameBg] = ImVec4(0.143f, 0.143f, 0.143f, 1.00f);
	style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.24f, 0.24f, 0.24f, 1.00f);
	style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
	style.Colors[ImGuiCol_TitleBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
	style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(1.00f, 0.98f, 0.95f, 0.75f);
	style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.07f, 0.07f, 0.09f, 1.00f);
	style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
	style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
	style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.80f, 0.80f, 0.83f, 0.31f);
	style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
	style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
	style.Colors[ImGuiCol_CheckMark] = ImVec4(0.76f, 0.28f, 0.28f, 1.00f);
	style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.76f, 0.28f, 0.28f, 1.00f);
	style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.76f, 0.28f, 0.28f, 1.00f);
	style.Colors[ImGuiCol_Button] = ImVec4(0.143f, 0.143f, 0.143f, 1.00f);
	style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.24f, 0.24f, 0.24f, 1.00f);
	style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
	style.Colors[ImGuiCol_Header] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
	style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
	style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);

	style.Colors[ImGuiCol_ResizeGrip] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
	style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
	style.Colors[ImGuiCol_PlotLines] = ImVec4(0.40f, 0.39f, 0.38f, 0.63f);
	style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.25f, 1.00f, 0.00f, 1.00f);
	style.Colors[ImGuiCol_PlotHistogram] = ImVec4(0.40f, 0.39f, 0.38f, 0.63f);
	style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.25f, 1.00f, 0.00f, 1.00f);
	style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.25f, 1.00f, 0.00f, 0.43f);
	style.Colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.043f, 0.043f, 0.043f, 0.73f);

	ImGuiIO& io = ImGui::GetIO();
	//font->im_font = io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\Tahoma.ttf", 18.0f);
	//font->font_size = 24.f;
	font->im_font = io.Fonts->AddFontFromMemoryTTF(customFont, sizeof(customFont), 18.f);
	h1Font = io.Fonts->AddFontFromMemoryTTF(customFont, sizeof(customFont), 28.f);
	h2Font = io.Fonts->AddFontFromMemoryTTF(customFont, sizeof(customFont), 21.f);
	io.IniFilename = NULL;
	io.LogFilename = NULL;

	static const ImWchar icon_ranges[]{ 0xf000, 0xf3ff, 0 };
	ImFontConfig icons_config;
	icons_config.MergeMode = true;
	icons_config.PixelSnapH = true;
	icons_config.OversampleH = true;
	icons_config.OversampleV = true;

	icons_font = io.Fonts->AddFontFromMemoryCompressedTTF(font_awesome_data, font_awesome_size, 19.5f, &icons_config, icon_ranges);

	ImGui_ImplWin32_Init(window_handle);
	ImGui_ImplDX11_Init(d3d_device, device_context);
}

VOID c_overlay::dest_imgui() {
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}

VOID c_overlay::init_render_target() {
	ID3D11Texture2D* back_buffer = nullptr;
	swap_chain->GetBuffer(0, IID_PPV_ARGS(&back_buffer));
	if (back_buffer)
		d3d_device->CreateRenderTargetView(back_buffer, NULL, &render_target_view);
	back_buffer->Release();
}

VOID c_overlay::dest_render_target() {
	if (!render_target_view)
		return;

	render_target_view->Release();
	render_target_view = NULL;
}

c_overlay::c_overlay() :
	ready(false),
	exit(false),
	window_handle(nullptr),
	window_class({}),
	window_width((FLOAT)GetSystemMetrics(SM_CXSCREEN)),
	window_height((FLOAT)GetSystemMetrics(SM_CYSCREEN)),
	font(nullptr),
	d3d_device(nullptr),
	device_context(nullptr),
	swap_chain(nullptr),
	render_target_view(nullptr),
	draw_list(nullptr),
	breath(NULL),
	menu_ticks(NULL)
{
	window_handle = FindWindowA("CEF-OSC-WIDGET", "NVIDIA GeForce Overlay");
	HWND cs2hwnd = FindWindowA(NULL, "Counter-Strike 2");
	auto getInfo = GetWindowLongA(window_handle, -20);
	auto changeAttributes = SetWindowLongA(window_handle, -20, (LONG_PTR)(getInfo | 0x20));
	LONG_PTR windowStyle = GetWindowLongPtr(window_handle, GWL_EXSTYLE);
	SetWindowLongPtr(window_handle, GWL_EXSTYLE, windowStyle & ~WS_EX_TRANSPARENT);

	SetFocus(cs2hwnd);
	SetFocus(window_handle);

	MARGINS margins = { -1 };
	margins.cyBottomHeight = margins.cyTopHeight = margins.cxLeftWidth = margins.cxRightWidth = -1;
	DwmExtendFrameIntoClientArea(window_handle, &margins);
	SetLayeredWindowAttributes(window_handle, 0x000000, 0xFF, 0x02);
	SetWindowPos(window_handle, HWND_TOPMOST, 0, 0, 0, 0, 0x0002 | 0x0001);

	if (!window_handle)
		return;

	font = new s_font();

	if (!init_device())
		return;
	
	init_imgui();

	std::thread(&c_overlay::input_handler, this).detach();
	std::thread(&c_overlay::anim_handler, this).detach();
}

c_overlay::~c_overlay() {
	dest_imgui();
	dest_device();

	delete font;
}

const void c_overlay::render(FLOAT width, FLOAT height) {
	if (exit) {
		ready = true;
		return;
	}

	if (pressedPanic) {
		return;
	}

	ImGuiStyle& style = ImGui::GetStyle();

	globals::base_module = mem.GetModuleAddress("client.dll");

	if (startedThreads == false) {
		startedThreads = true;
	}

	init_draw_list();

	double menu_anim = sin((M_PI / 2.0) * (((menu_ticks == 1 ? 0 : 1) * pow(menu_ticks, 2)) / ((double)pow(menu_anim_time - 1, 2))));
	circle({ window_width / 2, window_height / 2 }, menu_anim * (window_width * 0.573674 /*16:9 window calculation constant*/), ImColor(11, 11, 11, 80));
	style.Alpha = menu_anim;
	style.Colors[ImGuiCol_Border] = ImVec4(0.76f, 0.28f, 0.28f, ((breath / (float)breath_anim_time) * .7f) + .3f);

	//if (config::esp::bBoxes || config::esp::bHeadCircles || config::esp::bHealthBar || config::esp::bHealthInt || config::esp::bSkeleton || config::esp::bSnaplines || config::esp::bTeam) {
	if (gui::isRunning) {
		const uintptr_t localPlayer = mem.Read<uintptr_t>(globals::base_module + offsets::dwLocalPlayer);

		if (!localPlayer)
			return; // try not using return if youre gonna use this src idk

		if (config::aim::bFovCircle) {
			ImGui::GetBackgroundDrawList()->AddCircle({ static_cast<float>(globals::windowWidth) / 2.0f, static_cast<float>(globals::windowHeight) / 2.0f }, config::aim::fovSlider, ImColor(255, 255, 255));
		}

		const int8_t localTeam = mem.Read<int>(localPlayer + offsets::m_iTeamNum);
		const view_matrix_t view_matrix = mem.Read<view_matrix_t>(globals::base_module + offsets::dwViewMatrix);
		const uintptr_t entity_list = mem.Read<uintptr_t>(globals::base_module + offsets::dwEntityList);
		const std::uint32_t localPlayerPawn = mem.Read<std::uint32_t>(localPlayer + offsets::m_hPlayerPawn);

		if (!localPlayerPawn) // a 0 is here, prolly offsets, commented the return out for the time being, filtering teams doesnt work now XD
			;
			//return;

		const uintptr_t localList_entry2 = mem.Read<uintptr_t>(entity_list + 0x8 * ((localPlayerPawn & 0x7FFF) >> 9) + 16);
		const uintptr_t localpCSPlayerPawn = mem.Read<uintptr_t>(localList_entry2 + 120 * (localPlayerPawn & 0x1FF));

		if (!localpCSPlayerPawn)
			return;

		Vector3 localOrigin = mem.Read<Vector3>(localpCSPlayerPawn + offsets::m_vecOrigin);
		int playerIndex = 0;
		uintptr_t list_entry;

		for (playerIndex < 32; ++playerIndex;) {
			list_entry = mem.Read<uintptr_t>(entity_list + (8 * (playerIndex & 0x7FFF) >> 9) + 16);
			if (!list_entry)
				break;
			const uintptr_t player = mem.Read<uintptr_t>(list_entry + 120 * (playerIndex & 0x1FF));
			if (!player)
				continue;
			const int playerTeam = mem.Read<int>(player + offsets::m_iTeamNum);
			if (!config::esp::bShowTeam) {
				if (playerTeam == localTeam) {
					continue;
				}
			}

			const std::uint32_t playerPawn = mem.Read<std::uint32_t>(player + offsets::m_hPlayerPawn);
			const uintptr_t list_entry2 = mem.Read<uintptr_t>(entity_list + 0x8 * ((playerPawn & 0x7FFF) >> 9) + 16);

			if (!list_entry2)
				continue;

			const uintptr_t pCSPlayerPawn = mem.Read<uintptr_t>(list_entry2 + 120 * (playerPawn & 0x1FF));

			if (config::esp::bC4)
				vars::isBeingDefused = mem.Read<bool>(pCSPlayerPawn + offsets::m_bIsDefusing);

			if (!pCSPlayerPawn)
				continue;

			const int playerHealth = mem.Read<int>(pCSPlayerPawn + offsets::m_iHealth);

			if (playerHealth <= 0 || playerHealth > 100)
				continue;

			if (pCSPlayerPawn == localPlayer)
				continue;

			uint64_t gamescene = mem.Read<uint64_t>(pCSPlayerPawn + offsets::m_pGameSceneNode);
			uint64_t bonearray = mem.Read<uint64_t>(gamescene + offsets::modelState + 0x80);

			Vector3 origin = mem.Read<Vector3>(pCSPlayerPawn + offsets::m_vecOrigin);
			Vector3 headPos = mem.Read<Vector3>(bonearray + 6 * 32);

			if ((localOrigin - origin).length2d() < 10)
				continue;

			Vector3 feetPos = origin.w2s(view_matrix);
			Vector3 screenHead = (headPos + Vector3{ 0.f, 0.f, 11.f }).w2s(view_matrix);
			Vector3 trueScreenHead = headPos.w2s(view_matrix);
			Vector3 screenCenter = { static_cast<float>(globals::windowWidth) / 2.0f, static_cast<float>(globals::windowHeight) / 2.0f, 0.0f };
			float distance = localOrigin.calculate_distance(origin);
			float roundedDist = std::round(distance) / 10.f;
			int distInMeters = static_cast<int>(roundedDist);

			float distFromCross;
			float maxDist = FLT_MAX;

			if (GetAsyncKeyState(VK_XBUTTON2)) {
				distFromCross = std::sqrt((trueScreenHead.x - globals::windowWidth / 2) * (trueScreenHead.x - globals::windowWidth / 2) + (trueScreenHead.y - globals::windowHeight / 2) * (trueScreenHead.y - globals::windowHeight / 2));
				//std::cout << distFromCross << "\n";
				if (distFromCross < maxDist && distFromCross <= config::aim::fovSlider) {
					maxDist = distFromCross;
					if (playerTeam == localTeam)
						continue;
					mouse_event(MOUSEEVENTF_MOVE, trueScreenHead.x - globals::windowWidth / 2, trueScreenHead.y - globals::windowHeight / 2, 0, 0);
				}
			}

			float healthPercentage = static_cast<float>(playerHealth) / static_cast<float>(100);

			if (feetPos.z >= 0.001f) {
				float height = feetPos.y - screenHead.y;
				float width = height / 2.4f;
				if (config::esp::bBoxes) {
					ImGui::GetBackgroundDrawList()->AddRect({ screenHead.x - width, screenHead.y }, { screenHead.x + width, feetPos.y }, ImColor(255, 255, 255), 1.f);
				}
				if (config::esp::bSnaplines) {
					ImGui::GetBackgroundDrawList()->AddLine({ feetPos.x, feetPos.y }, { 1920 / 2, 1080 }, ImColor(255, 255, 255));
				}
				if (config::esp::bHealthBar) {
					// dont judge me
					ImGui::GetBackgroundDrawList()->AddRectFilled({ (screenHead.x - width - 8.f), screenHead.y + (feetPos.y - screenHead.y) * (1.f - healthPercentage)}, { (screenHead.x - width - 6.2f) , feetPos.y }, ImColor((255 - playerHealth), (55 + playerHealth * 2), 75), 5.f);
				}
				if (config::esp::bSkeleton) {
					Vector3 head = mem.Read<Vector3>(bonearray + 6 * 32);
					Vector3 cou = mem.Read<Vector3>(bonearray + 5 * 32);
					Vector3 shoulderR = mem.Read<Vector3>(bonearray + 8 * 32);
					Vector3 shoulderL = mem.Read<Vector3>(bonearray + 13 * 32);
					Vector3 brasR = mem.Read<Vector3>(bonearray + 9 * 32);
					Vector3 brasL = mem.Read<Vector3>(bonearray + 14 * 32);
					Vector3 handR = mem.Read<Vector3>(bonearray + 11 * 32);
					Vector3 handL = mem.Read<Vector3>(bonearray + 16 * 32);
					Vector3 cock = mem.Read<Vector3>(bonearray + 0 * 32);
					Vector3 kneesR = mem.Read<Vector3>(bonearray + 23 * 32);
					Vector3 kneesL = mem.Read<Vector3>(bonearray + 26 * 32);
					Vector3 feetR = mem.Read<Vector3>(bonearray + 24 * 32);
					Vector3 feetL = mem.Read<Vector3>(bonearray + 27 * 32);
					ImGui::GetBackgroundDrawList()->AddLine({ (head.w2s(view_matrix)).x, (head.w2s(view_matrix)).y }, { (cou.w2s(view_matrix)).x, (cou.w2s(view_matrix)).y }, ImColor(255, 255, 255), 1.f);
					ImGui::GetBackgroundDrawList()->AddLine({ (cou.w2s(view_matrix)).x, (cou.w2s(view_matrix)).y }, { (shoulderR.w2s(view_matrix)).x, (shoulderR.w2s(view_matrix)).y }, ImColor(255, 255, 255), 1.f);
					ImGui::GetBackgroundDrawList()->AddLine({ (shoulderR.w2s(view_matrix)).x, (shoulderR.w2s(view_matrix)).y }, { (brasR.w2s(view_matrix)).x, (brasR.w2s(view_matrix)).y }, ImColor(255, 255, 255), 1.f);
					ImGui::GetBackgroundDrawList()->AddLine({ (brasR.w2s(view_matrix)).x, (brasR.w2s(view_matrix)).y }, { (handR.w2s(view_matrix)).x, (handR.w2s(view_matrix)).y }, ImColor(255, 255, 255), 1.f);
					ImGui::GetBackgroundDrawList()->AddLine({ (cou.w2s(view_matrix)).x, (cou.w2s(view_matrix)).y }, { (shoulderL.w2s(view_matrix)).x, (shoulderL.w2s(view_matrix)).y }, ImColor(255, 255, 255), 1.f);
					ImGui::GetBackgroundDrawList()->AddLine({ (shoulderL.w2s(view_matrix)).x, (shoulderL.w2s(view_matrix)).y }, { (brasL.w2s(view_matrix)).x, (brasL.w2s(view_matrix)).y }, ImColor(255, 255, 255), 1.f);
					ImGui::GetBackgroundDrawList()->AddLine({ (brasL.w2s(view_matrix)).x, (brasL.w2s(view_matrix)).y }, { (handL.w2s(view_matrix)).x, (handL.w2s(view_matrix)).y }, ImColor(255, 255, 255), 1.f);
					ImGui::GetBackgroundDrawList()->AddLine({ (cou.w2s(view_matrix)).x, (cou.w2s(view_matrix)).y }, { (cock.w2s(view_matrix)).x, (cock.w2s(view_matrix)).y }, ImColor(255, 255, 255), 1.f);
					ImGui::GetBackgroundDrawList()->AddLine({ (cock.w2s(view_matrix)).x, (cock.w2s(view_matrix)).y }, { (kneesR.w2s(view_matrix)).x, (kneesR.w2s(view_matrix)).y }, ImColor(255, 255, 255), 1.f);
					ImGui::GetBackgroundDrawList()->AddLine({ (kneesR.w2s(view_matrix)).x, (kneesR.w2s(view_matrix)).y }, { (feetR.w2s(view_matrix)).x, (feetR.w2s(view_matrix)).y }, ImColor(255, 255, 255), 1.f);
					ImGui::GetBackgroundDrawList()->AddLine({ (cock.w2s(view_matrix)).x, (cock.w2s(view_matrix)).y }, { (kneesL.w2s(view_matrix)).x, (kneesL.w2s(view_matrix)).y }, ImColor(255, 255, 255), 1.f);
					ImGui::GetBackgroundDrawList()->AddLine({ (kneesL.w2s(view_matrix)).x, (kneesL.w2s(view_matrix)).y }, { (feetL.w2s(view_matrix)).x, (feetL.w2s(view_matrix)).y }, ImColor(255, 255, 255), 1.f);
				}
				if (config::esp::bHeadCircles) {
					ImGui::GetBackgroundDrawList()->AddCircle({ trueScreenHead.x, trueScreenHead.y }, 15.f, ImColor(255, 255, 255));
				}
				if (config::esp::bHealthInt) {
					ImGui::GetBackgroundDrawList()->AddText({ screenHead.x + width + 5.f, screenHead.y + 0.f }, ImColor(255, 255, 255), (std::to_string(playerHealth) + "hp").c_str());
				}
				if (config::esp::bName) {
					std::string playerName = "Invalid name";
					const DWORD64 playerNameAddress = mem.Read<DWORD64>(player + offsets::m_SanitizedName);
					if (playerNameAddress) {
						char buf[256];
						mem.ReadRaw(playerNameAddress, buf, sizeof(buf));
						playerName = std::string(buf);
					}
					ImGui::GetBackgroundDrawList()->AddText(
					{ screenHead.x - (ImGui::CalcTextSize(playerName.c_str()).x / 2), screenHead.y - 17.f },
					ImColor(255, 255, 255), 
					playerName.c_str()
					);
				}
				if (config::esp::bTeam) {
					ImGui::GetBackgroundDrawList()->AddText(
					{feetPos.x - (ImGui::CalcTextSize("Enemy").x /2), feetPos.y + 4.f}, 
					ImColor(255, 255, 255), 
					playerTeam == localTeam ? "Friendly" : "Enemy"
					);
				}
				if (config::esp::bDistance) {
					ImGui::GetBackgroundDrawList()->AddText(
					{ screenHead.x + width + 5.f, screenHead.y + 15.f },
					ImColor(255, 255, 255),
					(std::to_string(distInMeters) + "m").c_str()
					);
				}
			}
		}

		if (config::misc::watermark) {
			std::string watahmark = "big chit | ";
			std::string fps = std::to_string(static_cast<int>(ImGui::GetIO().Framerate));
			watahmark += fps.c_str();
			watahmark += "fps";
			ImVec2 textSize = ImGui::CalcTextSize(watahmark.c_str());
			ImVec2 rectPos = { globals::windowWidth - textSize.x - 20, 20 };
			ImVec2 rectSize = { textSize.x + 20, textSize.y + 5 };

			ImGui::GetBackgroundDrawList()->AddRectFilled(
				rectPos,
				ImVec2(rectPos.x + rectSize.x, rectPos.y + rectSize.y),
				ImColor(0, 0, 0, 150)
			);

			ImGui::GetBackgroundDrawList()->AddRect(
				rectPos,
				ImVec2(rectPos.x + rectSize.x, rectPos.y + rectSize.y),
				ImColor(194, 72, 72, 150)
			);

			ImGui::GetBackgroundDrawList()->AddText(
				{ globals::windowWidth - textSize.x - 10, 20 },
				ImColor(255, 255, 255),
				watahmark.c_str()
			);
		}

		if (config::esp::bC4) {
			bool isC4Planted = mem.Read<bool>(globals::base_module + offsets::dwC4Planted - 0x08);

			const uintptr_t planted_c4 = mem.Read<uintptr_t>(mem.Read<uintptr_t>(globals::base_module + offsets::dwC4Planted));
			const uintptr_t c4Node = mem.Read<uintptr_t>(planted_c4 + offsets::m_pGameSceneNode);
			
			const Vector3 c4Origin = mem.Read<Vector3>(c4Node + offsets::m_vecOrigin);
			const Vector3 c4ScreenPos = c4Origin.w2s(view_matrix);
			
			std::string plantedText = "Planted: ";
			plantedText += isC4Planted ? "Yes" : "No";

			// doesnt work
			std::string defusingText = "Defusing: ";
			defusingText += vars::isBeingDefused ? "Yes" : "No";
			
			std::string timerText = "Timer: " + std::to_string(vars::remainingTime.count()) + "s";

			if (isC4Planted && vars::remainingTime.count() <= 0) {
				std::thread(hacks::c4timer).detach();
			}
			else if (!isC4Planted && !vars::killTimer) {
				vars::killTimer = true;
				vars::remainingTime = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::seconds(0));
			}
			ImGui::GetBackgroundDrawList()->AddRectFilled(
				{ static_cast<float>(0 + static_cast<float>(gui::bombOffsetX)), static_cast<float>(0 + static_cast<float>(gui::bombOffsetY)) },
				{ static_cast<float>(150 + static_cast<float>(gui::bombOffsetX)), static_cast<float>(70 + static_cast<float>(gui::bombOffsetY)) },
				ImColor(0, 0, 0, 200)
			);
			ImGui::GetBackgroundDrawList()->AddRect(
				{ static_cast<float>(0 + static_cast<float>(gui::bombOffsetX)), static_cast<float>(0 + static_cast<float>(gui::bombOffsetY)) },
				{ static_cast<float>(150 + static_cast<float>(gui::bombOffsetX)), static_cast<float>(70 + static_cast<float>(gui::bombOffsetY)) },
				ImColor(194, 72, 72),
				0.0f,
				ImDrawCornerFlags_All
			);
			ImGui::GetBackgroundDrawList()->AddText(
				{ 10 + static_cast<float>(gui::bombOffsetX), 10 + static_cast<float>(gui::bombOffsetY) },
				ImColor(255, 255, 255),
				plantedText.c_str()
			);
			ImGui::GetBackgroundDrawList()->AddText(
				{ 10 + static_cast<float>(gui::bombOffsetX), 25 + static_cast<float>(gui::bombOffsetY) },
				ImColor(255, 255, 255),
				"Defusing: No"
			);
			ImGui::GetBackgroundDrawList()->AddText(
				{ 10 + static_cast<float>(gui::bombOffsetX), 40 + static_cast<float>(gui::bombOffsetY) },
				ImColor(255, 255, 255),
				timerText.c_str()
			);
		}
	}

	ImGui::SetNextWindowSize(ImVec2(644, 400));
	ImGui::Begin("Menu", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar);
	ImGui::PushFont(icons_font);

	if (gui::isRunning) {
		ImGui::PopFont();
		ImGui::BeginChild("Sidebar", ImVec2(150, 0), true);
		ImGui::PushFont(h1Font);
		ImGui::Text("big chit");
		ImGui::Separator();
		ImGui::PopFont();
		ImGui::PushFont(icons_font);
		if (ImGui::ButtonNoBg(ICON_FA_EYE " Visual")) {
			gui::showVisuals = true;
			gui::showMisc = false;
			gui::showAim = false;
			gui::showConfig = false;
			gui::showAbout = false;
		}
		if (ImGui::ButtonNoBg(ICON_FA_CROSSHAIRS " Aimbot")) {
			gui::showVisuals = false;
			gui::showMisc = false;
			gui::showAim = true;
			gui::showConfig = false;
			gui::showAbout = false;
		}
		if (ImGui::ButtonNoBg(ICON_FA_COG " Misc")) {
			gui::showVisuals = false;
			gui::showMisc = true;
			gui::showAim = false;
			gui::showConfig = false;
			gui::showAbout = false;
		}
		if (ImGui::ButtonNoBg(ICON_FA_SAVE " Config")) {
			gui::showVisuals = false;
			gui::showMisc = false;
			gui::showAim = false;
			gui::showConfig = true;
			gui::showAbout = false;
		}
		if (ImGui::ButtonNoBg(ICON_FA_INFO_CIRCLE " About")) {
			gui::showVisuals = false;
			gui::showMisc = false;
			gui::showAim = false;
			gui::showConfig = false;
			gui::showAbout = true;
		}
		ImGui::EndChild();
	}
	ImGui::PopFont();

	if (gui::isRunning) {
		ImGui::SameLine();
		if (gui::showVisuals) {
			ImGui::BeginChild("esp main", ImVec2(220, 0), true);
			ImGui::PushFont(h2Font);
			ImGui::Text("Main");
			ImGui::PopFont();
			ImGui::Separator();
			ImGui::Checkbox("Boxes", &config::esp::bBoxes);
			ImGui::Checkbox("Snaplines", &config::esp::bSnaplines);
			ImGui::Checkbox("Health Bar", &config::esp::bHealthBar);
			ImGui::Checkbox("Skeletons", &config::esp::bSkeleton);
			ImGui::Checkbox("Head Circles", &config::esp::bHeadCircles);
			ImGui::Checkbox("Show Team", &config::esp::bShowTeam);
			ImGui::EndChild();

			ImGui::SameLine();

			ImGui::BeginChild("esp info", ImVec2(220, 0), true);
			ImGui::PushFont(h2Font);
			ImGui::Text("Info");
			ImGui::PopFont();
			ImGui::Separator();
			ImGui::Checkbox("Name", &config::esp::bName);
			ImGui::Checkbox("Health", &config::esp::bHealthInt);
			ImGui::Checkbox("Distance", &config::esp::bDistance); // fucky idk
			ImGui::Checkbox("Team", &config::esp::bTeam);
			ImGui::Checkbox("C4", &config::esp::bC4);
			ImGui::SetNextItemWidth(120.f);
			ImGui::SliderInt("X", &gui::bombOffsetX, 0, 1920);
			ImGui::SameLine();
			ImGui::Text(std::to_string(gui::bombOffsetX).c_str());
			ImGui::SetNextItemWidth(120.f);
			ImGui::SliderInt("Y", &gui::bombOffsetY, 0, 1080);
			ImGui::SameLine();
			ImGui::Text(std::to_string(gui::bombOffsetY).c_str());
			ImGui::EndChild();
		}

		if (gui::showAim) {
			ImGui::BeginChild("aim", ImVec2(220, 0), true);
			ImGui::PushFont(h2Font);
			ImGui::Text("Aimbot");
			ImGui::PopFont();
			ImGui::Separator();
			ImGui::Checkbox("Enabled", &config::aim::bAimbot);
			ImGui::Checkbox("FOV Circle", &config::aim::bFovCircle);
			ImGui::SetNextItemWidth(120.f);
			ImGui::SliderFloat("FOV", &config::aim::fovSlider, 1.f, 100.f);
			ImGui::SameLine();
			ImGui::Text(std::to_string(config::aim::fovSlider).c_str());
			ImGui::EndChild();

			ImGui::SameLine();

			ImGui::BeginChild("triggerbot", ImVec2(220, 0), true);
			ImGui::PushFont(h2Font);
			ImGui::Text("Triggerbot");
			ImGui::PopFont();
			ImGui::Separator();
			ImGui::SetNextItemWidth(120.f);
			ImGui::Checkbox("Triggerbot", &config::aim::bTriggerbot);
			ImGui::SliderInt("Delay", &config::aim::triggerDelay, 1, 100);
			ImGui::EndChild();
		}

		if (gui::showMisc) {
			ImGui::BeginChild("misc", ImVec2(0, 0), true);
			ImGui::PushFont(h2Font);
			ImGui::Text("Miscellaneous");
			ImGui::PopFont();
			ImGui::Separator();
			ImGui::Checkbox("Watermark", &config::misc::watermark);
			//ImGui::Checkbox("Bunny Hop", &config::misc::bBhop);
			ImGui::EndChild();
		}

		if (gui::showConfig) {
			ImGui::BeginChild("configs", ImVec2(0, 0), true);
			ImGui::PushFont(h2Font);
			ImGui::Text("Configs");
			ImGui::PopFont();
			ImGui::Separator();
			ImGui::Button("Import");
			ImGui::Button("Export");
			ImGui::Text("I didnt make this stuff yet dont bother");
			ImGui::EndChild();
		}

		if (gui::showAbout) {
			ImGui::BeginChild("about", ImVec2(0, 0), true);
			ImGui::PushFont(h2Font);
			ImGui::Text("About");
			ImGui::PopFont();
			ImGui::Separator();
			ImGui::Text("Made by hellokittyfan47\n");
			if (ImGui::ButtonNoBg("site button click thing")) {
				ShellExecute(NULL, "open", "https://www.google.com/", 0, 0, SW_SHOWNORMAL);
			}
			ImGui::EndChild();
		}

		if (!gui::showVisuals && !gui::showAim && !gui::showMisc && !gui::showConfig && !gui::showAbout) {
			ImGui::BeginChild("helloguys", ImVec2(0, 0), true);
			ImGui::PushFont(h2Font);
			ImGui::Text("First window thingy hello :D");
			ImGui::PopFont();
			ImGui::Separator();
			ImGui::Text("https://github.com/hellokittyfan48");
			ImGui::EndChild();
		}

		ImGui::End();
	}

	if (menu_ticks == 1)
		return;
}

BOOL c_overlay::msg_loop() {
	if (window_handle) {
		MSG msg{ 0 };
		ImVec4 clear_clr = { 0,0,0,0 };

		HWND cs2hwnd = FindWindowA(NULL, "Counter-Strike 2");
		HWND cs2hwndActive = GetForegroundWindow();

		if (cs2hwndActive == cs2hwnd) {
			ShowWindow(window_handle, SW_SHOW);
		}
		else
		{
			ShowWindow(window_handle, SW_HIDE);
		}

		if (::PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE)) {
			::TranslateMessage(&msg);
			::DispatchMessage(&msg);
		}

		ImGui_ImplDX11_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();


		render(window_width, window_height);

		ImGui::Render();
		device_context->OMSetRenderTargets(1, &render_target_view, NULL);
		device_context->ClearRenderTargetView(render_target_view, (float*)&clear_clr);
		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
		swap_chain->Present(1, 0);

		return msg.message != WM_QUIT;
	}
	return false;
}

void c_overlay::input_handler() {
	for (; !exit; Sleep(1)) {
		ImGuiIO& io = ImGui::GetIO();

		POINT p{};
		GetCursorPos(&p);
		io.MousePos = ImVec2((float)p.x, (float)p.y);

		io.MouseDown[0] = GetAsyncKeyState(VK_LBUTTON) & 0x8000;
		io.MouseDown[1] = GetAsyncKeyState(VK_RBUTTON) & 0x8000;
	}
}

void c_overlay::anim_handler() {
	breath_timer.start();
	menu_timer.start();

	for (;!exit;) {
		RECT rect{ 0 };
		POINT point{ 0 };
		GetClientRect(window_handle, &rect);
		SetWindowPos(window_handle, HWND_TOPMOST, rect.left, rect.top, rect.right, rect.bottom, SWP_NOREDRAW);

		if (GetAsyncKeyState(VK_END) & 0x8000) {
			if (gui::isOpen) {
				LONG_PTR windowStyle = GetWindowLongPtr(window_handle, GWL_EXSTYLE);
				SetWindowLongPtr(window_handle, GWL_EXSTYLE, windowStyle | WS_EX_TRANSPARENT);
			}
			// idfk
			gui::isOpen = false;
			gui::isRunning = false;
			config::esp::bBoxes = false;
			config::esp::bSnaplines = false;
			config::esp::bHealthBar = false;
			config::esp::bHealthInt = false;
			config::esp::bHeadCircles = false;
			config::esp::bShowTeam = false;
			config::esp::bSkeleton = false;
			config::esp::bWeapon = false;
			config::esp::bName = false;
			config::esp::bTeam = false;
			config::aim::bTriggerbot = false;
			config::aim::bAimbot = false;
			config::aim::bFovCircle = false;
			config::misc::bBhop = false;
			config::misc::watermark = false;
			menu_ticks = menu_anim_time;
			menu_timer.start();
			pressedPanic = true;
		}
		
		int breath_tmr = breath_timer.end();
		if (breath_tmr >= breath_anim_time * 2) {
			breath = breath_anim_time; 
			breath_timer.start();
		}
		else if (breath_tmr > breath_anim_time) 
			breath = breath_anim_time - (breath_tmr - breath_anim_time);
		else if (breath_tmr <= breath_anim_time) 
			breath = breath_tmr;

		int menu_tmr = menu_timer.end();
		if (GetAsyncKeyState(VK_INSERT) & 0x8000 && menu_tmr > menu_anim_time) {
			if (gui::isOpen) {
				LONG_PTR windowStyle = GetWindowLongPtr(window_handle, GWL_EXSTYLE);
				SetWindowLongPtr(window_handle, GWL_EXSTYLE, windowStyle | WS_EX_TRANSPARENT);
			}
			else {
				LONG_PTR windowStyle = GetWindowLongPtr(window_handle, GWL_EXSTYLE);
				SetWindowLongPtr(window_handle, GWL_EXSTYLE, windowStyle & ~WS_EX_TRANSPARENT);
			}
			gui::isOpen = !gui::isOpen;
			menu_ticks = menu_anim_time;
			menu_timer.start();
		}
		else if (menu_tmr < menu_anim_time) {
			menu_ticks = (gui::isOpen ? menu_tmr : menu_anim_time - menu_tmr);
		}
	}
}

VOID c_overlay::init_draw_list() {
	draw_list = ImGui::GetBackgroundDrawList();
}

VOID c_overlay::crosshair(const FLOAT aSize, ImU32 color) {
	draw_list->AddLine({ window_width / 2,window_height / 2 - (aSize + 1) }, { window_width / 2 ,window_height / 2 + (aSize + 1) }, color, 2);
	draw_list->AddLine({ window_width / 2 - (aSize + 1),window_height / 2 }, { window_width / 2 + (aSize + 1), window_height / 2 }, color, 2);
}

VOID c_overlay::box(const ImVec2& pos, const FLOAT width, const FLOAT height, ImU32 color, const FLOAT line_width) {
	std::array<ImVec2, 4> box_lines{ pos,ImVec2{pos.x + width,pos.y},ImVec2{pos.x + width,pos.y + height},ImVec2{pos.x,pos.y + height} };
	draw_list->AddPolyline(box_lines.data(), box_lines.size(), color, true, 2);
}

VOID c_overlay::line(const ImVec2& point1, const ImVec2 point2, ImU32 color, const FLOAT line_width) {
	draw_list->AddLine(point1, point2, color, line_width);
}

VOID c_overlay::circle(const ImVec2& point, const FLOAT radius, ImU32 color) {
	draw_list->AddCircleFilled(point, radius, color, 200); // num segments is how circular the circle actually is
}

VOID c_overlay::text(const ImVec2& pos, const std::string& text, ImU32 color, bool outline) {
	if (outline) {
		draw_list->AddText(font->im_font, 18.f, { pos.x + 1, pos.y }, IM_COL32_BLACK, text.c_str());
		draw_list->AddText(font->im_font, 18.f, { pos.x - 1, pos.y }, IM_COL32_BLACK, text.c_str());
		draw_list->AddText(font->im_font, 18.f, { pos.x, pos.y + 1 }, IM_COL32_BLACK, text.c_str());
		draw_list->AddText(font->im_font, 18.f, { pos.x, pos.y - 1 }, IM_COL32_BLACK, text.c_str());
	}
	draw_list->AddText(font->im_font, 18.f, pos, color, text.c_str());
}

VOID c_overlay::radial_gradient(const ImVec2& center, float radius, ImU32 col_in, ImU32 col_out) {
	if (((col_in | col_out) & IM_COL32_A_MASK) == 0 || radius < 0.5f)
		return;

	draw_list->_PathArcToFastEx(center, radius, 0, IM_DRAWLIST_ARCFAST_SAMPLE_MAX, 0);
	const int count = draw_list->_Path.Size - 1;

	unsigned int vtx_base = draw_list->_VtxCurrentIdx;
	draw_list->PrimReserve(count * 3, count + 1);

	const ImVec2 uv = draw_list->_Data->TexUvWhitePixel;
	draw_list->PrimWriteVtx(center, uv, col_in);
	for (int n = 0; n < count; n++)
		draw_list->PrimWriteVtx(draw_list->_Path[n], uv, col_out);
	
	for (int n = 0; n < count; n++) {
		draw_list->PrimWriteIdx((ImDrawIdx)(vtx_base));
		draw_list->PrimWriteIdx((ImDrawIdx)(vtx_base + 1 + n));
		draw_list->PrimWriteIdx((ImDrawIdx)(vtx_base + 1 + ((n + 1) % count)));
	}
	draw_list->_Path.Size = 0;
}

BOOL c_overlay::in_screen(const ImVec2& pos) {
	return !(pos.x > window_width || pos.x<0 || pos.y>window_height || pos.y < 0);
}