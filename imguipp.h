#ifndef IMGUIPP_H
#include "imgui/imgui.h"
#include "imgui/imgui_internal.h"
#include <string>

namespace imguipp
{
	float getx();
	float gety();

	void button(const char* label, int& currentTab, int newTab, ImVec2 size = ImVec2());

	void line(int newId);
	void linevertical();

	void center_text(const char* text, int lineId, bool separator, bool textDiabled = false);
	void center_colored_text(const char* text, int lineId, bool separator, ImVec4 color);
	void center_text_ex(const char* text, float width_available, int lineId, bool separator);
	void ImGuiCenterText(const char* text, ...);

	namespace other
	{
		float get_window_size_x();
		float get_window_size_y();

		ImVec2 get_window_size();
		char* get_window_name();
		ImDrawList* get_drawlist();
	}

	ImVec4 to_vec4(float r, float g, float b, float a);

}
#define IMGUIPP_H
#endif