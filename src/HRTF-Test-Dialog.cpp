#include "HRTF-Test-Dialog.h"

#include "imgui.h"
#include "backends/rlImGui.h"
#include "backends/imfilebrowser.h"

HRTFTestDialog::HRTFTestDialog()
{
	okClicked = false;
}

void HRTFTestDialog::SetPointerToAudioEngine(OpenAlSoftAudioEngine* audioEngine){ptrAudioEngine = audioEngine;}

std::string testResult = "";
char* textToDisplay = nullptr;

void HRTFTestDialog::InitGUI()
{
	//Test HRTF and put test results in textBox
	ptrAudioEngine->TestHRTF();
	
	testResult = ptrAudioEngine->getHRTFTestResult();
	
	ptrAudioEngine->clear_testHRTFResults();
	textToDisplay = const_cast<char*>(testResult.c_str());
	
}

void HRTFTestDialog::DrawDialog()
{
	ImGui::OpenPopup("HRTF Test");

	// Always center this window when appearing
	ImVec2 center = ImGui::GetMainViewport()->GetCenter();
	ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

	if (ImGui::BeginPopupModal("HRTF Test", NULL, ImGuiWindowFlags_AlwaysAutoResize))
	{
		ImGui::Text(textToDisplay);

		//display OK and Cancel button on the same lines
		if (ImGui::Button("OK")) 
		{ 
			okClicked = true;
			ImGui::CloseCurrentPopup(); 
		}
		
		ImGui::EndPopup();
	}
	
}

bool HRTFTestDialog::OkClickedOn(){return okClicked;}

void HRTFTestDialog::resetConfig()
{
	testResult.clear();
	okClicked = false;
}
