#include "main.hpp"
#include "include.h"
#include "esp.hpp"
#include "aimbot.hpp"
#include "memory.h"
#include "offsets.h"
#include "exploit.h"
#include "xor.hpp"
#include <Windows.h>
#include "auth.hpp"
#include <string>
#include "skStr.h"
#include <urlmon.h>
#include "color.hpp"
#include "user_interface.h"
#include "imgui/imgui_impl_win32.h"
#include "trigger.hpp"
#include "nav_elements.h"
#include <iostream>
#include "json.hpp"
#include "utils.hpp"
#include <cstdlib>
#include <ctime>
#include <cstdio>
#include "file_operations.h"
#include "console_title_animator.h"
#include <thread>
#include "AntiDebug.h"
#include <fstream>
#include <shellapi.h>

#pragma comment(lib, "urlmon.lib")

std::string tm_to_readable_time(tm ctx);
static std::time_t string_to_timet(std::string timestamp);
static std::tm timet_to_tm(time_t timestamp);

using namespace KeyAuth;
std::string FPS;
std::string name = "z7Joel Spoofer"; // application name. right above the blurred text aka the secret on the licenses tab among other tabs
std::string ownerid = "ZkWAEVx0Ee"; // ownerid, found in account settings. click your profile picture on top right of dashboard and then account settings.
std::string secret = "742ff32663c0927e3372d033850555be20022c22c66f15f04e8c2e8ebf0ba10a"; // app secret, the blurred text on licenses tab and other tabs
std::string version = "1.0"; // leave alone unless you've changed version on website
std::string url = "https://keyauth.win/api/1.2/"; // change if you're self-hosting

std::string username;
std::string password;
std::string key;
std::string logg;
using namespace std;
bool visual = true;
bool aimbot = true;
enum heads {
	rage, antiaim, visuals, settings, skins, configs, scripts
};

enum sub_heads {
	general, accuracy, exploits, _general, advanced
};


void panel();


bool LoadTextureFromFile(const char* filename, GLuint* out_texture, int* out_width, int* out_height) {

	// Load from file
	int image_width = 0;
	int image_height = 0;
	unsigned char* image_data = stbi_load(filename, &image_width, &image_height, NULL, 4);
	if (image_data == NULL)
		return false;

	// Create a OpenGL texture identifier
	GLuint image_texture;
	glGenTextures(1, &image_texture);
	glBindTexture(GL_TEXTURE_2D, image_texture);

	// Setup filtering parameters for display
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); // This is required on WebGL for non power-of-two textures
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); // Same

	// Upload pixels into texture
#if defined(GL_UNPACK_ROW_LENGTH) && !defined(__EMSCRIPTEN__)
	glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
#endif
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image_width, image_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_data);
	stbi_image_free(image_data);

	*out_texture = image_texture;
	*out_width = image_width;
	*out_height = image_height;

	return true;
}

void FovAimbot() {
	int screen_x = GetSystemMetrics(SM_CXSCREEN);
	int screen_y = GetSystemMetrics(SM_CYSCREEN);
	auto draw = ImGui::GetOverlayDrawList();
	draw->AddCircle(ImVec2(screen_x / 2, screen_y / 2), aimbot_fov_value, IM_COL32(233, 232, 232, 255), fov_segments, 2.0f);
}

void GPlayerList() {
	uintptr_t PedReplayInterface = read_mem<uintptr_t>(ReplayInterface_ptr + 0x18);
	uintptr_t PedList = read_mem<uintptr_t>(PedReplayInterface + 0x100);
	for (int i = 0; i < 256; i++) {
		if (PedList) {
			uintptr_t Ped = read_mem<uintptr_t>(PedList + (i * 0x10));
			if (Ped) {
				if (localplayer != 0) {
					if (Ped != localplayer) {

						player_contry[i] = Ped;

					}
				}
			}
		}
	}
}

void PlayerList() {
	GPlayerList();
	for (int i = 0; i < 256; i++) {
		if (player_contry[i] != 0) {
			string layer = std::to_string(i);
			if (player_friend[i] == 0) {
				if (ImGui::Button(("Add Friend ID: " + layer).c_str(), { 199, 22 })) {
					player_friend[i] = player_contry[i];
				}
			}
			else {
				if (ImGui::Button(("Remove Friend ID: " + layer).c_str(), { 199, 22 })) {
					player_friend[i] = 0;
				}
			}
		}
	}
}

void OpenWebsite() {
	ShellExecuteA(nullptr, "open", "yourdiscordlink", nullptr, nullptr, SW_SHOWNORMAL);
}

void OpenWebsiteTDCheats() {
	ShellExecuteA(nullptr, "open", "yourdiscordlink", nullptr, nullptr, SW_SHOWNORMAL);
}

void OpenWebsiteTyhluDesigns() {
	ShellExecuteA(nullptr, "open", "yourdiscordlink", nullptr, nullptr, SW_SHOWNORMAL);
}

void logoload() {

	string dwnld_URL = "https://cdn.discordapp.com/attachments/1149157419637817418/1173077183154094090/Untitled-12.png?ex=6562a443&is=65502f43&hm=b24b628fcf7fcd8f3943deacbd5d9ba70e36b4748893ce556e2b6cdf66647dd3&";
	string savepath = "C:\\Untitled-12.png";
	URLDownloadToFile(NULL, dwnld_URL.c_str(), savepath.c_str(), 0, NULL);

}

void loginpanel();

int valueI;
bool the_bool;

bool god;
bool nspeed;
bool nrec;
bool nreload;
bool npread;
bool VehSp;
bool VehBrk;
bool VehGod;
int tabb = 0;

const char* menu_bind[] = { ("F4"), ("F8"), ("Delete"), ("Insert") };
const char* aimbot_bind[] = { ("Right Button") ,("Left Button") , ("Page Backward Button"), ("Page Forward Button"),("Shift"),("Menu"),("E") };
const char* trigger_bind[] = { ("Right Button") ,("Left Button"), ("Page Backward Button"), ("Page Forward Button"),("Shift"),("Menu"),("E") };
static int selected = 0;
static int sub_selected = 0;

static bool bullying = false;
static bool bullythosemfs = false;
static bool stopbullythemyoushitters = false;
static bool stop = false;
static bool niggas = false;
static bool hihihi = false;
static bool bro = false;
static int sliderinc = 0;
//static int combo = 0;
const char* hitbox_items[] = { "Head", "Neck", "Pelvis", "Right Hand", "Left Hand", "Right Foot", "Left Foot" };
static float color[5] = { 1.f, 1.f, 1.f, 1.f };


static heads tab{ rage };
static sub_heads subtab{ general };

const char* tab_name = tab == rage ? "Ragebot" : tab == antiaim ? "Anti-aim" : tab == visuals ? "Visuals" : tab == settings ? "Settings" : tab == skins ? "Skins" : tab == configs ? "Configs" : tab == scripts ? "Scripts" : 0;
const char* tab_icon = tab == rage ? "B" : tab == antiaim ? "C" : tab == visuals ? "D" : tab == settings ? "E" : tab == skins ? "F" : tab == configs ? "G" : tab == scripts ? "H" : 0;


bool loadConfigRequested = false;  // Flag to indicate whether to load config

void LoadSettings(const char* filename) {
	std::ifstream file(filename, std::ios::binary);
	if (!file.is_open()) {
		std::cerr << "Error opening file for reading: " << filename << std::endl;
		return;
	}

	file.read(reinterpret_cast<char*>(&ex_noreaload), sizeof(bool));
	file.read(reinterpret_cast<char*>(&ex_norecoil), sizeof(bool));
	file.read(reinterpret_cast<char*>(&ex_nospread), sizeof(bool));
	file.read(reinterpret_cast<char*>(&ex_vehicleaceleration), sizeof(bool));
	file.read(reinterpret_cast<char*>(&ex_runspeed_on), sizeof(bool));
	file.read(reinterpret_cast<char*>(&ex_runspeed_value), sizeof(bool));
	file.read(reinterpret_cast<char*>(&ex_vehiclebreak), sizeof(bool));
	file.read(reinterpret_cast<char*>(&ex_vehiclebreak_value), sizeof(bool));
	file.read(reinterpret_cast<char*>(&ex_vehiclegodmode), sizeof(bool));
	file.read(reinterpret_cast<char*>(&aimbot_on), sizeof(bool));
	file.read(reinterpret_cast<char*>(&aimbot_bone), sizeof(bool));
	file.read(reinterpret_cast<char*>(&aimbot_fov), sizeof(bool));
	file.read(reinterpret_cast<char*>(&aimbot_fov_value), sizeof(bool));
	file.read(reinterpret_cast<char*>(&TargetNpcs), sizeof(bool));
	file.read(reinterpret_cast<char*>(&esp_box), sizeof(bool));
	file.read(reinterpret_cast<char*>(&esp_corner), sizeof(bool));
	file.read(reinterpret_cast<char*>(&esp_drawnpcs), sizeof(bool));
	file.read(reinterpret_cast<char*>(&esp_head), sizeof(bool));
	file.read(reinterpret_cast<char*>(&esp_hpbar), sizeof(bool));
	file.read(reinterpret_cast<char*>(&esp_lines), sizeof(bool));
	file.read(reinterpret_cast<char*>(&esp_max_distance), sizeof(bool));
	file.read(reinterpret_cast<char*>(&esp_showdistance), sizeof(bool));
	file.read(reinterpret_cast<char*>(&esp_showid), sizeof(bool));
	file.read(reinterpret_cast<char*>(&esp_skeletons), sizeof(bool));
	file.read(reinterpret_cast<char*>(&esp_skeletons2), sizeof(bool));
	file.read(reinterpret_cast<char*>(&trigger_fov), sizeof(bool));
	file.read(reinterpret_cast<char*>(&trigger_fov_value), sizeof(bool));
	file.read(reinterpret_cast<char*>(&trigger_on), sizeof(bool));
	file.read(reinterpret_cast<char*>(&esp_hptext), sizeof(bool));
	file.read(reinterpret_cast<char*>(&filled_box), sizeof(bool));

	file.close();
}

void SaveSettings(const char* filename) {
	std::ofstream file(filename, std::ios::binary);
	if (!file.is_open()) {
		std::cerr << "Error opening file for writing: " << filename << std::endl;
		return;
	}

	file.write(reinterpret_cast<char*>(&ex_noreaload), sizeof(bool));
	file.write(reinterpret_cast<char*>(&ex_norecoil), sizeof(bool));
	file.write(reinterpret_cast<char*>(&ex_nospread), sizeof(bool));
	file.write(reinterpret_cast<char*>(&ex_vehicleaceleration), sizeof(bool));
	file.write(reinterpret_cast<char*>(&ex_runspeed_on), sizeof(bool));
	file.write(reinterpret_cast<char*>(&ex_runspeed_value), sizeof(bool));
	file.write(reinterpret_cast<char*>(&ex_vehiclebreak), sizeof(bool));
	file.write(reinterpret_cast<char*>(&ex_vehiclebreak_value), sizeof(bool));
	file.write(reinterpret_cast<char*>(&ex_vehiclegodmode), sizeof(bool));
	file.write(reinterpret_cast<char*>(&aimbot_on), sizeof(bool));
	file.write(reinterpret_cast<char*>(&aimbot_bone), sizeof(bool));
	file.write(reinterpret_cast<char*>(&aimbot_fov), sizeof(bool));
	file.write(reinterpret_cast<char*>(&aimbot_fov_value), sizeof(bool));
	file.write(reinterpret_cast<char*>(&TargetNpcs), sizeof(bool));
	file.write(reinterpret_cast<char*>(&esp_box), sizeof(bool));
	file.write(reinterpret_cast<char*>(&esp_corner), sizeof(bool));
	file.write(reinterpret_cast<char*>(&esp_drawnpcs), sizeof(bool));
	file.write(reinterpret_cast<char*>(&esp_head), sizeof(bool));
	file.write(reinterpret_cast<char*>(&esp_hpbar), sizeof(bool));
	file.write(reinterpret_cast<char*>(&esp_lines), sizeof(bool));
	file.write(reinterpret_cast<char*>(&esp_max_distance), sizeof(bool));
	file.write(reinterpret_cast<char*>(&esp_showdistance), sizeof(bool));
	file.write(reinterpret_cast<char*>(&esp_showid), sizeof(bool));
	file.write(reinterpret_cast<char*>(&esp_skeletons), sizeof(bool));
	file.write(reinterpret_cast<char*>(&esp_skeletons2), sizeof(bool));
	file.write(reinterpret_cast<char*>(&trigger_fov), sizeof(bool));
	file.write(reinterpret_cast<char*>(&trigger_fov_value), sizeof(bool));
	file.write(reinterpret_cast<char*>(&trigger_on), sizeof(bool));
	file.write(reinterpret_cast<char*>(&esp_hptext), sizeof(bool));
	file.write(reinterpret_cast<char*>(&filled_box), sizeof(bool));


	file.close();
}

void Render() {

	glfwPollEvents();
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();



	if (bind_menu == 0) { bind_menuVK = VK_F4; }
	else if (bind_menu == 1) { bind_menuVK = VK_F8; }
	else if (bind_menu == 2) { bind_menuVK = VK_DELETE; }
	else if (bind_menu == 3) { bind_menuVK = VK_INSERT; }

	__try {
		if (aimbot_fov) { FovAimbot(); }

		if (esp_on) { DrawEsp(); }

		if (aimbot_on) { bindaimbot(); Aimbot(); }

		if (trigger_on) {
			bindtrigger();
			if (trigger_type == 0 || trigger_type == 1) {
				RageTriggerbot();
			}
			else if (trigger_type == 2) {
				LegitTriggerbot();
			}
		}

		if (ex_runspeed_on) { Player::SpeedWalk(true, ex_runspeed_value); nspeed = true; }
		else { if (nspeed) { Player::SpeedWalk(false, 0); }nspeed = false; }

		if (ex_runspeed_on) { Player::SpeedWalk(true, ex_runspeed_value); nspeed = true; }
		else { if (nspeed) { Player::SpeedWalk(false, 0); }nspeed = false; }

		if (ex_norecoil) { Weapons::NoRecoil(true); nrec = true; }
		else { if (nrec) { Weapons::NoRecoil(false); }nrec = false; }


		if (ex_noreaload) { Weapons::NoReaload(true); nreload = true; }
		else { if (nreload) { Weapons::NoReaload(false); }nreload = false; }

		if (ex_nospread) { Weapons::NoSpread(true); npread = true; }
		else { if (npread) { Weapons::NoSpread(false); }npread = false; }

		if (ex_godmode) { Player::Godmode(true); god = true; }
		else { if (god) { Player::Godmode(false); }god = false; }

		if (ex_semigodmode) {
			if (Player::GetHealth() <= 200) {
				Player::SetHealth(Player::GetMaxHealth());
			}
		}

		if (ex_vehicleaceleration) { Vehicle::VehicleAceleration(true, ex_vehicleaceleration_value); VehSp = true; }
		else { if (VehSp) { Vehicle::VehicleAceleration(false, ex_vehicleaceleration_value); }VehSp = false; }

		if (ex_vehiclebreak) { Vehicle::VehicleBrakeForce(true, ex_vehiclebreak_value); VehBrk = true; }
		else { if (VehBrk) { Vehicle::VehicleBrakeForce(false, ex_vehiclebreak_value); }VehBrk = false; }


		if (ex_vehiclegodmode) { Vehicle::GodModeVehicle(true); VehGod = true; }
		else { if (VehGod) { Vehicle::GodModeVehicle(false); }VehGod = false; }

	}
	__except ((GetExceptionCode() == EXCEPTION_ACCESS_VIOLATION) ? EXCEPTION_EXECUTE_HANDLER : EXCEPTION_CONTINUE_SEARCH) {}

	if (menu_visible) {

		ImGui::SetNextWindowSize(ImVec2(700, 495));

		ImGui::Begin("TD External Main", nullptr, ImGuiWindowFlags_NoDecoration);

		auto draw = ImGui::GetWindowDrawList();
		ImVec2 pos = ImGui::GetWindowPos();
		ImVec2 size = ImGui::GetWindowSize();


		draw->AddRectFilled(ImVec2(0, 0), ImVec2(1920, 1080), ImColor(0, 0, 0, 255), 0.2f);


		draw->AddLine(ImVec2(pos.x + 55, pos.y + 35), ImVec2(pos.x + size.x, pos.y + 35), ImColor(0, 0, 0, 15)); // upper line
		draw->AddLine(ImVec2(pos.x + 55, pos.y), ImVec2(pos.x + 55, pos.y + size.y), ImColor(255, 255, 255, 15)); // left line

		ImGui::SetCursorPos(ImVec2(13, 11));
		ImGui::BeginGroup();
		if (elements::tab("B", tab == rage)) {
			tab = rage; sub_selected = 0;
		}
		if (elements::tab("D", tab == antiaim)) {
			tab = antiaim; sub_selected = 2;
		}
		if (elements::tab("C", tab == visuals)) {
			tab = visuals; sub_selected = 184;
		}
		if (elements::tab("E", tab == settings)) {
			tab = settings; sub_selected = 456;
		}
		ImGui::EndGroup();

		ImGui::SetNextWindowPos(ImVec2(5, 1060));

		if (ImGui::Begin(("TD FiveM"), NULL, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoMove))
		{
			ImGui::Text(("TD Cheats | Build Date: %s  | %.1f FPS"), __DATE__, ImGui::GetIO().Framerate);
		}
		ImGui::End();

		if (tab == rage) {
			ImGui::BeginGroup();
			ImGui::SetCursorPos(ImVec2(65, 14));
			if (custom_interface::subtab("Aimbot", 0 == sub_selected)) {
				sub_selected = 0;
			}
			ImGui::SameLine();
			if (custom_interface::subtab("TriggerBot", 16 == sub_selected)) {
				sub_selected = 16;
			}
			ImGui::SameLine();
			if (custom_interface::subtab("Extra", 87 == sub_selected)) {
				sub_selected = 87;
			}
			ImGui::EndGroup();

			if (sub_selected == 0) {
				ImGui::SetCursorPos(ImVec2(65, 44));
				ImGui::BeginChild("Aim Assistance", ImVec2(219, 382));
				{
					ImGui::Checkbox("Aimbot", &aimbot_on);
					ImGui::Checkbox("Show FOV", &aimbot_fov);
					ImGui::Checkbox("Target Peds/NPCs", &TargetNpcs);
				}
				ImGui::EndChild();

				ImGui::SetCursorPos(ImVec2(291, 44));
				ImGui::BeginChild("Config", ImVec2(219, 382));
				{
					ImGui::Combo("Aimbot Bone", &aimbot_bone, hitbox_items, IM_ARRAYSIZE(hitbox_items));
					ImGui::SliderFloat("Aimbot Smoothing", &aimbot_smooth, 0.f, 50.f, "% .2f");
					ImGui::SliderFloat("Aimbot Distance", &aimbot_max_distance, 20.f, 500.f, "% .2f");
					ImGui::SliderFloat("Aimbot FOV", &aimbot_fov_value, 20.f, 400.f, "% .2f");
				}
				ImGui::EndChild();

			}

			if (sub_selected == 16) {
				ImGui::SetCursorPos(ImVec2(65, 44));
				ImGui::BeginChild("TriggerBot", ImVec2(219, 382));
				{
					ImGui::Checkbox("TriggerBot", &trigger_on);
					ImGui::Checkbox("TriggerBot FOV", &trigger_fov);

				}
				ImGui::EndChild();

				ImGui::SetCursorPos(ImVec2(291, 44));
				ImGui::BeginChild("Config", ImVec2(219, 382));
				{
					//ImGui::Combo("Aimbot Bone", &combo, combo_items, 4);
					ImGui::SliderFloat("TriggerBot FOV Value", &trigger_fov_value, 0.f, 50.f, "% .2f");
					ImGui::SliderFloat("TriggerBot Distance", &aimbot_max_distance, 20.f, 500.f, "% .2f");
				}
				ImGui::EndChild();
			}

			if (sub_selected == 87) {
				ImGui::SetCursorPos(ImVec2(65, 44));
				ImGui::BeginChild("Friend List", ImVec2(219, 382));
				{
					PlayerList();
				}
				ImGui::EndChild();
			}
		}

		if (tab == antiaim) {
			ImGui::BeginGroup();
			ImGui::SetCursorPos(ImVec2(65, 14));
			if (custom_interface::subtab("Visuals", 2 == sub_selected)) {
				sub_selected = 2;
			}
			ImGui::EndGroup();

			if (sub_selected == 2) {
				ImGui::SetCursorPos(ImVec2(65, 44));
				ImGui::BeginChild("Visuals", ImVec2(219, 382));
				{

					ImGui::Checkbox(XorStr("Box").c_str(), &esp_box);
					ImGui::SameLine();
					ImGui::SetCursorPosX(200);
					ImGui::ColorEdit4(XorStr("Box Color").c_str(), Temp::Box, ImGuiColorEditFlags_NoLabel | ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar);
					Colors::ESPBox.Value.x = Temp::Box[0]; Colors::ESPBox.Value.y = Temp::Box[1]; Colors::ESPBox.Value.z = Temp::Box[2]; Colors::ESPBox.Value.w = Temp::Box[3];

					ImGui::Checkbox(XorStr("Box Corner").c_str(), &esp_corner);
					ImGui::SameLine();
					ImGui::SetCursorPosX(200);
					ImGui::ColorEdit4(XorStr("Box Corner Color").c_str(), Temp::Corner, ImGuiColorEditFlags_NoLabel | ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar);
					Colors::ESPCorner.Value.x = Temp::Corner[0]; Colors::ESPCorner.Value.y = Temp::Corner[1]; Colors::ESPCorner.Value.z = Temp::Corner[2]; Colors::ESPCorner.Value.w = Temp::Corner[3];

					ImGui::Checkbox(XorStr("Skeleton").c_str(), &esp_skeletons);
					ImGui::SameLine();
					ImGui::SetCursorPosX(200);
					ImGui::ColorEdit4(XorStr("Skeleton Color").c_str(), Temp::Skeleton, ImGuiColorEditFlags_NoLabel | ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar);
					Colors::ESPSkeleton.Value.x = Temp::Skeleton[0]; Colors::ESPSkeleton.Value.y = Temp::Skeleton[1]; Colors::ESPSkeleton.Value.z = Temp::Skeleton[2]; Colors::ESPSkeleton.Value.w = Temp::Skeleton[3];

					ImGui::Checkbox(XorStr("Show Distance").c_str(), &esp_showdistance);
					ImGui::SameLine();
					ImGui::SetCursorPosX(200);
					ImGui::ColorEdit4(XorStr("Text Distance Color").c_str(), Temp::Distance, ImGuiColorEditFlags_NoLabel | ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar);
					Colors::ESPDistance.Value.x = Temp::Distance[0]; Colors::ESPDistance.Value.y = Temp::Distance[1]; Colors::ESPDistance.Value.z = Temp::Distance[2]; Colors::ESPDistance.Value.w = Temp::Distance[3];

					ImGui::Checkbox(XorStr("Health Text").c_str(), &esp_hptext);
					ImGui::SameLine();
					ImGui::SetCursorPosX(200);
					ImGui::ColorEdit4(XorStr("Text Health Color").c_str(), Temp::HpText, ImGuiColorEditFlags_NoLabel | ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar);
					Colors::ESPHpText.Value.x = Temp::HpText[0]; Colors::ESPHpText.Value.y = Temp::HpText[1]; Colors::ESPHpText.Value.z = Temp::HpText[2]; Colors::ESPHpText.Value.w = Temp::HpText[3];

					ImGui::Checkbox(XorStr("Lines").c_str(), &esp_lines);
					ImGui::SameLine();
					ImGui::SetCursorPosX(200);
					ImGui::ColorEdit4(XorStr("Lines Color").c_str(), Temp::Lines, ImGuiColorEditFlags_NoLabel | ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar);
					Colors::ESPLines.Value.x = Temp::Lines[0]; Colors::ESPLines.Value.y = Temp::Lines[1]; Colors::ESPLines.Value.z = Temp::Lines[2]; Colors::ESPLines.Value.w = Temp::Lines[3];


					ImGui::Checkbox(XorStr("Draw NPCS").c_str(), &esp_drawnpcs);

					ImGui::Checkbox(XorStr("Filled Box").c_str(), &filled_box);

					ImGui::Checkbox(XorStr("Health Bar").c_str(), &esp_hpbar);

					ImGui::Checkbox(XorStr("Head").c_str(), &esp_head);

					ImGui::Checkbox(XorStr("Show Menu IDs").c_str(), &esp_showid);

					ImGui::SliderFloat(XorStr("ESP Distance").c_str(), &esp_max_distance, 20, 500, "% .2f");
				}
				ImGui::EndChild();


			}
		}

		if (tab == visuals) {
			ImGui::BeginGroup();
			ImGui::SetCursorPos(ImVec2(65, 14));
			if (custom_interface::subtab("Self", 184 == sub_selected)) {
				sub_selected = 184;
			}
			ImGui::SameLine();
			if (custom_interface::subtab("Misc", 145 == sub_selected)) {
				sub_selected = 145;
			}
			ImGui::EndGroup();

			//armas
			if (sub_selected == 184) {
				ImGui::SetCursorPos(ImVec2(65, 44));
				ImGui::BeginChild("Player", ImVec2(219, 382));
				{
					ImGui::Checkbox("GodMode (Not Fully Stable)", &ex_godmode);
					ImGui::Checkbox("Change Run Speed", &ex_runspeed_on);
					ImGui::SliderFloat(XorStr("Run Speed").c_str(), &ex_runspeed_value, 0.f, 1000000.f, "% .1f");
				}
				ImGui::EndChild();
			}
			else if (sub_selected == 145) {

				ImGui::SetCursorPos(ImVec2(65, 44));
				ImGui::BeginChild("Vehicle", ImVec2(219, 382));
				{
					ImGui::Checkbox("Vehicle Acceleration", &ex_vehicleaceleration);
					ImGui::Checkbox("Vehicle GodMode", &ex_vehiclegodmode);
					ImGui::Checkbox("Fix Engine", &the_bool);
					ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 1.0f, 1.0f));  // Set text color to white
					if (ImGui::Button("Fix Engine", ImVec2(192, 22))) {
						Vehicle::FixEngine(1000.f);
					}
					ImGui::PopStyleColor();  // Revert text color back to default
					ImGui::SliderFloat(XorStr("Vehicle Acceleration").c_str(), &ex_vehicleaceleration_value, 0.f, 20000.f, "% .2f");
				}
				ImGui::EndChild();

				ImGui::SetCursorPos(ImVec2(291, 44));
				ImGui::BeginChild("Weapons", ImVec2(219, 382));
				{
					ImGui::Checkbox("No Recoil", &ex_norecoil);
					ImGui::Checkbox("No Reload", &ex_noreaload);
					ImGui::Checkbox("No Spread", &ex_nospread);
				}
				ImGui::EndChild();

			}



		}

		if (tab == settings) {

			ImGui::BeginGroup();
			ImGui::SetCursorPos(ImVec2(65, 14));
			if (custom_interface::subtab("Extra", 456 == sub_selected)) {
				sub_selected = 456; //esta la pones aqui tmb
			}
			ImGui::EndGroup();

			if (sub_selected == 456) {
				ImGui::SetCursorPos(ImVec2(65, 44));
				ImGui::BeginChild("Binds", ImVec2(219, 382));
				{
					ImGui::Combo("Menu Key", &bind_menu, menu_bind, IM_ARRAYSIZE(menu_bind));
					ImGui::Combo("Aimbot Key", &bind_aimbot, aimbot_bind, IM_ARRAYSIZE(aimbot_bind));
					ImGui::Combo("TriggerBot Key", &bind_trigger, trigger_bind, IM_ARRAYSIZE(trigger_bind));

				}
				ImGui::EndChild();

				ImGui::SetCursorPos(ImVec2(291, 44));
				ImGui::BeginChild("Information", ImVec2(219, 382));
				{
					ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 1.0f, 1.0f)); // Set text color to white
					ImGui::Spacing();
					ImGui::Spacing();
					ImGui::PushFont(bold);
					ImGui::Text("TD Cheats | Configs");
					ImGui::PopFont();
					ImGui::Separator();
					if (ImGui::Button("Load Config")) {
						loadConfigRequested = true;
					}

					// Save Config button
					ImGui::SameLine();
					if (ImGui::Button("Save Config")) {
						SaveSettings("TD config.dat");
					}

					ImGui::Spacing();
					ImGui::Spacing();
					ImGui::Spacing();
					ImGui::Spacing();
					ImGui::Spacing();
					ImGui::Spacing();
					ImGui::Spacing();
					ImGui::Spacing();
					ImGui::Spacing();
					ImGui::Spacing();
					ImGui::Spacing();
					ImGui::Spacing();
					ImGui::Spacing();
					ImGui::Spacing();
					ImGui::Spacing();
					ImGui::Spacing();
					ImGui::PushFont(bold);
					ImGui::Text("TD cheats");
					ImGui::PopFont();
					ImGui::Separator();
					ImGui::Spacing();
					if (ImGui::Button("TD Cheats Discord")) {
						OpenWebsiteTyhluDesigns();
					}
					if (ImGui::Button("TD Cheats Discord")) {
						OpenWebsiteTDCheats();
					}
					if (ImGui::Button("TD Cheats Discord")) {
						OpenWebsite();
					}
					ImGui::PopStyleColor();
				}
				ImGui::EndChild();
			}
		}
		ImGui::End();
	}

	if (loadConfigRequested) {
		LoadSettings("TD config.dat");
		loadConfigRequested = false;
	}

	ImGui::Render();
	int display_w, display_h;
	glfwGetFramebufferSize(menu_window, &display_w, &display_h);
	glViewport(0, 0, display_w, display_h);
	glClear(GL_COLOR_BUFFER_BIT);
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	glfwSwapBuffers(menu_window);
}

void ReallockLocalPlayer() {
	while (true) {

		localplayer = read_mem<uintptr_t>(world_ptr + (0x8));

	}
}

string procname = "";
int initialize() {
	attach_window = FindWindow("grcWindow", 0);
	DWORD off_world, off_replay, off_view;
	if (isProcRunning("FiveM_GTAProcess.exe")) {
		off_world = offset::b1604::world_ptr;
		off_replay = offset::b1604::ReplayInterface_ptr;
		off_view = offset::b1604::viewport_ptr;
		procname = "FiveM_GTAProcess.exe";
	}
	else if (isProcRunning("FiveM_b2060_GTAProcess.exe")) {
		off_world = offset::b2060::world_ptr;
		off_replay = offset::b2060::ReplayInterface_ptr;
		off_view = offset::b2060::viewport_ptr;
		procname = "FiveM_b2060_GTAProcess.exe";

	}
	else if (isProcRunning("FiveM_b2189_GTAProcess.exe")) {
		off_world = offset::b2189::world_ptr;
		off_replay = offset::b2189::ReplayInterface_ptr;
		off_view = offset::b2189::viewport_ptr;
		procname = "FiveM_b2189_GTAProcess.exe";

	}
	else if (isProcRunning("FiveM_b2372_GTAProcess.exe")) {
		off_world = offset::b2372::world_ptr;
		off_replay = offset::b2372::ReplayInterface_ptr;
		off_view = offset::b2372::viewport_ptr;
		procname = "FiveM_b2372_GTAProcess.exe";

	}
	else if (isProcRunning("FiveM_b2545_GTAProcess.exe")) {
		off_world = offset::b2545::world_ptr;
		off_replay = offset::b2545::ReplayInterface_ptr;
		off_view = offset::b2545::viewport_ptr;
		procname = "FiveM_b2545_GTAProcess.exe";

	}
	else if (isProcRunning("FiveM_b2612_GTAProcess.exe")) {
		off_world = offset::b2612::world_ptr;
		off_replay = offset::b2612::ReplayInterface_ptr;
		off_view = offset::b2612::viewport_ptr;
		procname = "FiveM_b2612_GTAProcess.exe";
	}
	else if (isProcRunning("FiveM_b2699_GTAProcess.exe")) {
		off_world = offset::b2699::world_ptr;
		off_replay = offset::b2699::ReplayInterface_ptr;
		off_view = offset::b2699::viewport_ptr;
		procname = "FiveM_b2699_GTAProcess.exe";
	}
	if (!attach_window) {
		MessageBox(NULL, "Could not find FiveM process.", "TD Cheats", MB_ICONERROR);
		ExitProcess(0);
	}

	//fps();

	DWORD pid;
	GetWindowThreadProcessId(attach_window, &pid);

	process_handle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);

	base_address = get_module_base64(pid, (procname).c_str());

	ReplayInterface_ptr = read_mem<uintptr_t>(base_address.modBaseAddr + (off_replay));

	world_ptr = read_mem<uintptr_t>(base_address.modBaseAddr + (off_world));

	viewport_ptr = read_mem<uintptr_t>(base_address.modBaseAddr + (off_view));

	localplayer = read_mem<uintptr_t>(world_ptr + (0x8));


	setupWindow();
	if (!menu_window) { return 1; }

	CreateThread(nullptr, NULL, (LPTHREAD_START_ROUTINE)ReallockLocalPlayer, nullptr, NULL, nullptr);
	tema();
	while (!glfwWindowShouldClose(menu_window)) {
		handleKeyPresses();
		Render();
	}

	cleanupWindow();
}

void openLink(const std::string& url) {
    ShellExecuteA(nullptr, "open", url.c_str(), nullptr, nullptr, SW_SHOWNORMAL);
}

#ifdef _WINDLL
BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpReserved) {
	if (fdwReason == DLL_PROCESS_ATTACH) {
		DisableThreadLibraryCalls(hinstDLL);
		CreateThread(nullptr, NULL, (LPTHREAD_START_ROUTINE)initialize, nullptr, NULL, nullptr);
		return TRUE;

	}
	return TRUE;
}
#else
api KeyAuthApp(name, ownerid, secret, version, url);

int main() {
	if (isDebuggerPresent()) {
		std::cerr << "Debugger detected, exiting.\n";
		return 1;
	}
	srand(static_cast<unsigned int>(time(nullptr)));
	std::thread titleThread(animateConsoleTitle);

	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
	SetConsoleTextAttribute(hConsole, 4);
		std::cout <<
			R"(
$$$$$$$$\ $$$$$$$\         $$$$$$\  $$\   $$\ $$$$$$$$\  $$$$$$\ $$$$$$$$\  $$$$$$\  
\__$$  __|$$  __$$\       $$  __$$\ $$ |  $$ |$$  _____|$$  __$$\\__$$  __|$$  __$$\ 
   $$ |   $$ |  $$ |      $$ /  \__|$$ |  $$ |$$ |      $$ /  $$ |  $$ |   $$ /  \__|
   $$ |   $$ |  $$ |      $$ |      $$$$$$$$ |$$$$$\    $$$$$$$$ |  $$ |   \$$$$$$\  
   $$ |   $$ |  $$ |      $$ |      $$  __$$ |$$  __|   $$  __$$ |  $$ |    \____$$\ 
   $$ |   $$ |  $$ |      $$ |  $$\ $$ |  $$ |$$ |      $$ |  $$ |  $$ |   $$\   $$ |
   $$ |   $$$$$$$  |      \$$$$$$  |$$ |  $$ |$$$$$$$$\ $$ |  $$ |  $$ |   \$$$$$$  |
   \__|   \_______/        \______/ \__|  \__|\________|\__|  \__|  \__|    \______/ 
                                                                                     
                                                                                     
                                                                                     

                                     TD cheats 


        )";


	SetConsoleTextAttribute(hConsole, 15);
	std::cout << skCrypt("\n\n Connecting...\n");
	KeyAuthApp.init();
	if (!KeyAuthApp.data.success)
	{
		std::cout << skCrypt("\n Status: ") << KeyAuthApp.data.message;
		Sleep(1500);
		exit(0);
	}

	//1

	SetConsoleTextAttribute(hConsole, 10);

	Sleep(2000);  // Display for 2 seconds (2000 milliseconds)
	std::cout << " Successfully Connected.\n\n";
	renameFile();
	SetConsoleTextAttribute(hConsole, 15);  // Reset color to default
	
	if (std::filesystem::exists("TDlogin.json"))
	{
		if (!CheckIfJsonKeyExists("TDlogin.json", "username"))
		{
			std::string key = ReadFromJson("TDlogin.json", "license");
			KeyAuthApp.license(key);
			if (!KeyAuthApp.data.success)
			{
				std::remove("TDlogin.json");
				std::cout << skCrypt("\n Status: ") << KeyAuthApp.data.message;
				Sleep(1500);
				exit(1);
			}
			std::cout << skCrypt("\n\n [TD Loader] Automatically Logged In.\n");
		}
		else
		{
			std::string username = ReadFromJson("TDlogin.json", "username");
			std::string password = ReadFromJson("TDlogin.json", "password");
			KeyAuthApp.login(username, password);
			if (!KeyAuthApp.data.success)
			{
				std::remove("TDlogin.json");
				std::cout << skCrypt("\n Status: ") << KeyAuthApp.data.message;
				Sleep(1500);
				exit(1);
			}
			std::cout << skCrypt("\n\n [TD Loader] Automatically Logged In.\n");
		}
	}
	else
	{
		SetConsoleTextAttribute(hConsole, 15);  // Set color

		Sleep(1000);
		std::cout << "\n Logging in will automatically initiate the cheat; please ensure that FiveM is running before doing so.\n\n";
		Sleep(1000);


		std::cout << skCrypt("\n [1] Login\n ");
		std::cout << skCrypt("\n [2] Register\n ");
		SetConsoleTextAttribute(hConsole, 7);
		SetConsoleTextAttribute(hConsole, 4);

		std::cout << skCrypt("\n Selection: ");

		SetConsoleTextAttribute(hConsole, 7); //reset color

		int option;
		std::string username;
		std::string password;
		std::string key;

		std::cin >> option;
		switch (option)
		{
		case 1:
			std::cout << skCrypt("\n\n Enter username: ");
			std::cin >> username;
			std::cout << skCrypt("\n Enter password: ");
			std::cin >> password;
			KeyAuthApp.login(username, password);
			break;
		case 2:
			std::cout << skCrypt("\n\n Enter username: ");
			std::cin >> username;
			std::cout << skCrypt("\n Enter password: ");
			std::cin >> password;
			std::cout << skCrypt("\n Enter license key: ");
			std::cin >> key;
			KeyAuthApp.regstr(username, password, key);
			break;
		default:
			std::cout << skCrypt("\n\n Status: Failure: Invalid Selection");
			Sleep(3000);
			exit(0);
		}

		if (!KeyAuthApp.data.success)
		{
			std::cout << skCrypt("\n Status: ") << KeyAuthApp.data.message;
			Sleep(2500);
			exit(0);
		}
		if (username.empty() || password.empty())
		{
			WriteToJson("TDlogin.json", "license", key, false, "", "");
			std::cout << skCrypt("\n\n [TD Loader] Successfully Created File For Auto Login (TDlogin.json)\n");
		}
		else
		{
			WriteToJson("TDlogin.json", "username", username, true, "password", password);
			std::cout << skCrypt("\n\n [TD Loader] Successfully Created File For Auto Login (TDlogin.json)\n");
		}
	}

	std::cout << skCrypt("\n ");
	SetConsoleTextAttribute(hConsole, 10);
	std::cout << skCrypt("\n Initializing.");
	Sleep(2500);
	std::cout << skCrypt("\n ");
	SetConsoleTextAttribute(hConsole, 7);
	SetConsoleTextAttribute(hConsole, 4);
	std::cout << skCrypt("\n Starting in 3 seconds.");
	SetConsoleTextAttribute(hConsole, 7);
	Sleep(3000);
	initialize();
	titleThread.join();

	return 0;
}

std::string tm_to_readable_time(tm ctx) {
	char buffer[80];

	strftime(buffer, sizeof(buffer), "%a %m/%d/%y %H:%M:%S %Z", &ctx);

	return std::string(buffer);
}

static std::time_t string_to_timet(std::string timestamp) {
	auto cv = strtol(timestamp.c_str(), NULL, 10); // long

	return (time_t)cv;
}

static std::tm timet_to_tm(time_t timestamp) {
	std::tm context;

	localtime_s(&context, &timestamp);

	return context;

}
#endif}