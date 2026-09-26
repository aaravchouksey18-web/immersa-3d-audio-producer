#include "Change-HRTF-Dialog.h"


#include "imgui.h"
#include "backends/rlImGui.h"
#include "backends/imfilebrowser.h"
#include <cstring>

ChangeHRTFDialog::ChangeHRTFDialog()
{
	hrtf_selection_index = 0;
	okClicked = false;
}

void ChangeHRTFDialog::SetPointerToAudioEngine(OpenAlSoftAudioEngine* audioEngine){ptrAudioEngine = audioEngine;}


bool changehrtf_dropDownHRTFMode = false;
int changehrtf_dropDownHRTFActive = 0;
std::string hrtf_choices = "";
char log_message[100] = "";


void ChangeHRTFDialog::InitGUI()
{
	//add all HRTF names to listbox	
	ptrAudioEngine->GetAvailableHRTFNames(&hrtf_names);
	
	
	//initialize choices string
	for(size_t i = 0; i < hrtf_names.size(); i++)
	{
		hrtf_names[i] = hrtf_names[i] + " " + std::to_string(i);
	}
	
}

bool ChangeHRTFDialog::OkClickedOn(){return okClicked;}

void ChangeHRTFDialog::DrawDialog()
{

	ImGui::OpenPopup("HRTF Change");

	// Always center this window when appearing
	ImVec2 center = ImGui::GetMainViewport()->GetCenter();
	ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

	if (ImGui::BeginPopupModal("HRTF Change", NULL, ImGuiWindowFlags_AlwaysAutoResize))
	{

		if(hrtf_names.size() > 0)
		{
			static int item_current_idx = 0; // Here we store our selection data as an index.
			const char* combo_preview_value = hrtf_names[item_current_idx].c_str();  // Pass in the preview value visible before opening the combo (it could be anything)
			static ImGuiComboFlags flags = 0;
						
			if (ImGui::BeginCombo("HRTF", combo_preview_value, flags))
			{
				for (int n = 0; n < hrtf_names.size(); n++)
				{
					const bool is_selected = (item_current_idx == n);
					if (ImGui::Selectable(hrtf_names[n].c_str(), is_selected))
					{
						item_current_idx = n;
						hrtf_selection_index = item_current_idx;

						std::string message;
						ptrAudioEngine->SelectThisHRTFByIndex(hrtf_selection_index,message);
						
						strncpy(log_message, message.c_str(), 100);
						log_message[99] = '\0';
					}

					// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
					if (is_selected)
						ImGui::SetItemDefaultFocus();
				}
				ImGui::EndCombo();
			}
			
		}

		ImGui::Separator();
			
		ImGui::Text(log_message);

		//display OK and Cancel button on the same lines
		if (ImGui::Button("OK")) 
		{ 
			okClicked = true;
			ImGui::CloseCurrentPopup(); 
		}

		ImGui::EndPopup();
	}

}

void ChangeHRTFDialog::resetConfig()
{
	hrtf_names.clear();
	hrtf_choices.clear();
	okClicked = false;
	memset(log_message, 0, sizeof(log_message));
}
