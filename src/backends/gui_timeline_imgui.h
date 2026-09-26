/*******************************************************************************************
*
*   LICENSE: zlib/libpng
*
*   Copyright (c) 2023 Pablo Camacho
*
*   This software is provided "as-is", without any express or implied warranty. In no event
*   will the authors be held liable for any damages arising from the use of this software.
*
*   Permission is granted to anyone to use this software for any purpose, including commercial
*   applications, and to alter it and redistribute it freely, subject to the following restrictions:
*
*     1. The origin of this software must not be misrepresented; you must not claim that you
*     wrote the original software. If you use this software in a product, an acknowledgment
*     in the product documentation would be appreciated but is not required.
*
*     2. Altered source versions must be plainly marked as such, and must not be misrepresented
*     as being the original software.
*
*     3. This notice may not be removed or altered from any source distribution.
*
**********************************************************************************************/
#ifndef GUI_3D_OBJECT_TIMELINE_H
#define GUI_3D_OBJECT_TIMELINE_H

#include "imgui.h"

#include <stdlib.h>
#include <string.h>

//3d Object Timeline

const ImU32 white_color = 0xAAFFFFFF;
const ImU32 light_blue_color = 0xAAFFFFA0;

//for drawing general timeline ticks and frame line
struct TimelineSettings_ImGui
{
	bool editMode;
	
	bool valueChanged;
	bool addPoint;
	bool showPropertiesBox;
	
	int scroll_value; //sets leftmost and rightmost frame
	int max_scroll_value; //max value for scrolling
	int* leftmost_frame_ptr; //frame that is on the left, use with gui scroll panel
	int* rightmost_frame_ptr; //frame that is on the right, use with gui scroll panel
	int max_num_frames_displayed; //max number of frames to be displayed on screen
	int frame_scroll_value; //value of frame that scroll adds
	int frame_scroll_rate; //rate at which 1 scroll value increases frame_scroll_value
	
	//frame selection variables, 
	size_t current_timeline_frame;
	bool frameSelected;
	float frameDrawX;
	
	//area that mouse can click to add timeline points
	Rectangle mouseArea;
	bool mouse_control;
	
	size_t max_num_frames;
	
	
};

//for drawing timeline points
struct TimelineParameterSettings_ImGui
{
		
	size_t max_num_frames;
	
	int* leftmost_frame_ptr; //frame that is on the left, use with gui scroll panel
	int* rightmost_frame_ptr; //frame that is on the right, use with gui scroll panel
	int max_num_frames_displayed; //max number of frames to be displayed on screen
	
	bool* array_points_ptr; //array of points to render
	
	bool showPropertiesBox;
	
	//where to draw points
	float draw_start_x;
	float draw_y;
	
	ImU32 draw_color;
	
} ;


TimelineSettings_ImGui InitTimelineSettings_ImGui(size_t max_num_frames);
TimelineParameterSettings_ImGui InitTimelineParameterSettings_ImGui(size_t max_num_frames, bool* points_ptr, float x, float y,
														int* left_frame_ptr, int* right_frame_ptr, int max_num_frames_to_display);

//Draws timeline 
//returns if timeline was clicked on
void Timeline_ImGui(TimelineSettings_ImGui* settings);

void Timeline_Parameter_ImGui(TimelineParameterSettings_ImGui* settings);

TimelineSettings_ImGui InitTimelineSettings_ImGui(size_t max_num_frames, int* left_frame_ptr, int* right_frame_ptr,int max_num_frames_to_display)
{
	TimelineSettings_ImGui settings = {0};
	
	settings.editMode = true;
	settings.current_timeline_frame = 0;
	settings.max_num_frames = max_num_frames;
	settings.mouse_control = true;
	
	settings.max_num_frames_displayed = max_num_frames_to_display;
	
	settings.leftmost_frame_ptr = left_frame_ptr;
	settings.rightmost_frame_ptr = right_frame_ptr;
	settings.scroll_value = 0;
	settings.frame_scroll_value = 0;
	settings.frame_scroll_rate = 1;
	settings.max_scroll_value = (max_num_frames / settings.frame_scroll_rate) - 1;
	
	return settings;
}

TimelineParameterSettings_ImGui InitTimelineParameterSettings_ImGui(size_t max_num_frames, bool* points_ptr, float x, float y,
														int* left_frame_ptr, int* right_frame_ptr, int max_num_frames_to_display)
{
	TimelineParameterSettings_ImGui settings = { 0 };
	
	settings.array_points_ptr = points_ptr;
	settings.max_num_frames = max_num_frames;
	
	settings.draw_start_x = x;
	settings.draw_y = y;
	
	//default color is white
	settings.draw_color = white_color;
	
	settings.leftmost_frame_ptr = left_frame_ptr;
	settings.rightmost_frame_ptr = right_frame_ptr;
	
	settings.max_num_frames_displayed = max_num_frames_to_display;
	
	return settings;
}

//only show every 20th frame
static int show_frame_modulo_factor = 10;

static float pixel_frame_offset = 100;

void Timeline_ImGui(TimelineSettings_ImGui* settings)
{
	//draw timeline
	//horizontal axis is the time
	//vertical axis is nothing
	
	ImDrawList* draw_list = ImGui::GetWindowDrawList();
	ImVec2 canvas_pos = ImGui::GetCursorScreenPos(); // ImDrawList API uses screen coordinates!
	ImVec2 canvas_size = ImGui::GetContentRegionAvail(); 
	float tickmark_width = 5;
	
	pixel_frame_offset = canvas_pos.x;
	
	ImGuiIO& io = ImGui::GetIO();
	
	settings->frame_scroll_value = ImGui::GetScrollX();
	
	if(settings->frame_scroll_value > settings->max_num_frames - settings->max_num_frames_displayed)
	{
		settings->frame_scroll_value = settings->max_num_frames - settings->max_num_frames_displayed;
	}
	
	//set leftmost frame to be a multiple of the modulo factor
	*settings->leftmost_frame_ptr = show_frame_modulo_factor * (settings->frame_scroll_value / show_frame_modulo_factor);
	*settings->rightmost_frame_ptr = settings->frame_scroll_value + settings->max_num_frames_displayed;
	
	//draw frame marks and frame number
	for(int i = *settings->leftmost_frame_ptr; i <= *settings->rightmost_frame_ptr; i += show_frame_modulo_factor)
	{
		//tick mark
		ImVec2 pmin = ImVec2(canvas_pos.x +  tickmark_width*(i - *settings->leftmost_frame_ptr),
							canvas_pos.y);
											
		ImVec2 pmax = ImVec2(canvas_pos.x +  tickmark_width*(i - *settings->leftmost_frame_ptr) + tickmark_width,
							canvas_pos.y + 10);
		
		draw_list->AddRectFilled(pmin,pmax,white_color);
		
		//frame number		
		char c_buffer[4];
		sprintf(c_buffer,"%d",i);
		draw_list->AddText(pmax, white_color, &c_buffer[0]);
	}
		
	//if in edit mode
	if(settings->editMode && settings->mouse_control)
	{
		//draw cursor line where mouse pointer is

		//select frame based on where mouse pointer is clicked on 
		if (ImGui::IsMousePosValid())
		{			
			float cursor_x = io.MousePos.x;
			float rightBoundary = canvas_pos.x +  
								tickmark_width*(*settings->rightmost_frame_ptr - *settings->leftmost_frame_ptr);
			if(cursor_x < canvas_pos.x){cursor_x = canvas_pos.x;}
			if(cursor_x > rightBoundary)
			{
				cursor_x = rightBoundary;
			}

			ImU32 cursor_color = white_color;
			if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) )
			{
				settings->frameSelected = !settings->frameSelected;
				
				if(settings->frameSelected)
				{
					settings->frameDrawX = cursor_x;
				}
			}
			
			if(settings->frameSelected)
			{
				cursor_x = settings->frameDrawX;
				cursor_color = light_blue_color;
				
				if(settings->frameDrawX >= pixel_frame_offset)
				{
					settings->current_timeline_frame = static_cast <size_t> ( (settings->frameDrawX - pixel_frame_offset) / tickmark_width) + settings->frame_scroll_value;
				}
			}
			
			ImVec2 pmin = ImVec2(cursor_x,
							canvas_pos.y);
											
			ImVec2 pmax = ImVec2(cursor_x + tickmark_width,
							canvas_pos.y + canvas_size.y);
		
			draw_list->AddRectFilled(pmin,pmax,cursor_color);
		}
		else
		{
			if(settings->frameSelected)
			{
				ImU32 cursor_color = light_blue_color;
				ImVec2 pmin = ImVec2(settings->frameDrawX,
							canvas_pos.y);
											
				ImVec2 pmax = ImVec2(settings->frameDrawX + tickmark_width,
							canvas_pos.y + canvas_size.y);
							
				draw_list->AddRectFilled(pmin,pmax,cursor_color);
				
				if(settings->frameDrawX >= pixel_frame_offset)
				{
					settings->current_timeline_frame = static_cast <size_t> ( (settings->frameDrawX - pixel_frame_offset) / tickmark_width) + settings->frame_scroll_value;
				}
				
			}
		}
	}
	//if in edit mode, and no mouse countrol
	else if(settings->editMode && !settings->mouse_control)
	{
		ImU32 cursor_color = white_color;
		if(settings->frameSelected)
		{
			cursor_color = light_blue_color;
		}
		
		float cursor_x = ((settings->current_timeline_frame - settings->frame_scroll_value) * tickmark_width) + pixel_frame_offset;
		
		ImVec2 pmin = ImVec2(cursor_x,
					canvas_pos.y);
									
		ImVec2 pmax = ImVec2(cursor_x + tickmark_width,
					canvas_pos.y + canvas_size.y);
					
		draw_list->AddRectFilled(pmin,pmax,cursor_color);
	}
	//else if not in edit mode, i.e. playback mode
	else
	{
		//draw cursor at current timeline frame
		float cursor_x = ((settings->current_timeline_frame - settings->frame_scroll_value) * tickmark_width) + pixel_frame_offset;
		
		ImVec2 pmin = ImVec2(cursor_x,
					canvas_pos.y);
									
		ImVec2 pmax = ImVec2(cursor_x + tickmark_width,
					canvas_pos.y + canvas_size.y);
					
		draw_list->AddRectFilled(pmin,pmax,white_color);
	}

}

void Timeline_Parameter_ImGui(TimelineParameterSettings_ImGui* settings)
{
	ImDrawList* draw_list = ImGui::GetWindowDrawList();
	ImVec2 canvas_pos = ImGui::GetCursorScreenPos();            // ImDrawList API uses screen coordinates!
	float tickmark_width = 5;

	//draw points
	if(settings->max_num_frames >= *settings->rightmost_frame_ptr &&
		settings->max_num_frames > *settings->leftmost_frame_ptr)
	{
		bool* element_ptr = settings->array_points_ptr + *settings->leftmost_frame_ptr;
		int i = 0;
		
		for(i = *settings->leftmost_frame_ptr; i < *settings->rightmost_frame_ptr; i++)
		{
			if(*element_ptr)
			{
											
				ImVec2 pmin = ImVec2(canvas_pos.x + settings->draw_start_x + tickmark_width*i, 
									canvas_pos.y + tickmark_width + settings->draw_y);
									
				ImVec2 pmax = ImVec2(canvas_pos.x + tickmark_width + settings->draw_start_x + tickmark_width*i,
					canvas_pos.y + tickmark_width + settings->draw_y + 50);
					
				draw_list->AddRectFilled(pmin,pmax,settings->draw_color);
			}
			
			element_ptr++;
		}
	}

	
}

#endif
