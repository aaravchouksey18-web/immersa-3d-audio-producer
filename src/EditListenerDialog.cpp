#include "EditListenerDialog.h"

#include <string>

#include "imgui.h"
#include "backends/rlImGui.h"
#include "backends/imfilebrowser.h"

EditListenerDialog::EditListenerDialog(const std::string& title)
{
	//this dialog is a file-scope static in main_gui_editor.cpp and its
	//constructor body was empty, so every member below held indeterminate
	//garbage. SetPointerToListener() is not always reached before DrawDialog()
	//reads them, so a garbage pointer here meant a wild dereference.
	ptrListener = nullptr;
	
	okClicked = false;
	cancelClicked = false;
}


static float xValueParam = 0.0f;
static float yValueParam = 0.0f;
static float zValueParam = 0.0f;


bool editlt_tempFreeRoamBool = false;
bool editlt_free_roam_box_stat = false;

bool editlt_extOrientationBool = false;
bool editlt_ext_orient_box_stat = false;

void EditListenerDialog::SetPointerToListener(Listener* listener){ptrListener = listener;}

void EditListenerDialog::InitGUI()
{
	if(ptrListener == nullptr){std::cout << "listener pointer is nullptr! \n";}
	else
	{
		xValueParam = ptrListener->getPositionX();
		
		yValueParam = ptrListener->getPositionY();
		
		zValueParam = ptrListener->getPositionZ();
		
		editlt_tempFreeRoamBool = ptrListener->GetListenerFreeRoamBool();
		
		editlt_extOrientationBool = ptrListener->GetListenerExternalDeviceOrientationBool();
	}
}



void EditListenerDialog::DrawDialog()
{
	ImGui::OpenPopup("Edit Listener");

	// Always center this window when appearing
	ImVec2 center = ImGui::GetMainViewport()->GetCenter();
	ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

	if (ImGui::BeginPopupModal("Edit Listener", NULL, ImGuiWindowFlags_AlwaysAutoResize))
	{	
		//display x value float input box
		ImGui::InputFloat("x", &xValueParam, 0.01f, 1.0f, "%.3f");
		
		//display y value float input box
		ImGui::InputFloat("y", &yValueParam, 0.01f, 1.0f, "%.3f");
		
		//display z value float input box
		ImGui::InputFloat("z", &zValueParam, 0.01f, 1.0f, "%.3f");
		
		ImGui::Separator();
		
		//explicitly show the toggleable listener behaviours so OK always applies
		//exactly what the user sees -- and never wipes the external-orientation
		//flag with a stale hidden value
		ImGui::Checkbox("Free roam", &editlt_tempFreeRoamBool);
		ImGui::Checkbox("External orientation device", &editlt_extOrientationBool);
		
		ImGui::Separator();
		
		//display OK and Cancel button on the same lines
		if (ImGui::Button("OK", ImVec2(120, 0))) 
		{ 
			okClicked = true;
			ImGui::CloseCurrentPopup(); 
		}
		ImGui::SetItemDefaultFocus();
		ImGui::SameLine();
		if (ImGui::Button("Cancel", ImVec2(120, 0))) 
		{
			cancelClicked = true; 
			ImGui::CloseCurrentPopup(); 
		}
		
		ImGui::EndPopup();
	}
	
	if(okClicked)
	{
		EditListenerDialog::ChangeListenerAttributes();
	}
	
}


void EditListenerDialog::ChangeListenerAttributes()
{	
	if(ptrListener != nullptr)
	{
		//change position of selected sound producer based on what is in textfields
		
		ptrListener->setPositionX(xValueParam);
		ptrListener->setPositionY(yValueParam);
		ptrListener->setPositionZ(zValueParam);
		
		//change free roam status
		ptrListener->SetListenerFreeRoamBool(editlt_tempFreeRoamBool);
		
		//change external device orientation status
		ptrListener->SetListenerExternalDeviceOrientationBool(editlt_extOrientationBool);
		
	}
	
}

void EditListenerDialog::resetConfig()
{
	editlt_tempFreeRoamBool = false;
	editlt_free_roam_box_stat = false;

	editlt_extOrientationBool = false;
	editlt_ext_orient_box_stat = false;
	
	okClicked = false;
	cancelClicked = false;
}

bool EditListenerDialog::OkClickedOn(){ return okClicked;}

bool EditListenerDialog::CancelClickedOn(){return cancelClicked;}
