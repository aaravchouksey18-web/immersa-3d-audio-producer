#include "EditMultipleEchoZonesDialog.h"

#include "global_variables.h"

#include <string>

#include "imgui.h"
#include "backends/rlImGui.h"
#include "backends/imfilebrowser.h"

#include "echo_zone_props.h"
#include "backends/imgui_input_extended.h"

EditMultipleEchoZonesDialog::EditMultipleEchoZonesDialog()
{
	okClicked = false;
	cancelClicked = false;
	
	m_selection_index = 0;
	spt_selection_index = 0;
}

void EditMultipleEchoZonesDialog::SetCurrentZoneIndexForEditing(size_t index){m_selection_index = index;}

void EditMultipleEchoZonesDialog::SetPointerToEffectsManager(EffectsManager* effects_manager){m_effects_manager_ptr = effects_manager;}

bool EditMultipleEchoZonesDialog::OkClickedOn(){return okClicked;}

bool EditMultipleEchoZonesDialog::CancelClickedOn(){return cancelClicked;}


static bool ez_name_box_pressed = false;
static char ez_char_name[32] = "name here";

static float xValueParam = 0.0f;

static float yValueParam = 0.0f;

static float zValueParam = 0.0f;

static float widthValueParam = 10.0f;

static float delayValueParam = default_delay;

static float lrDelayValueParam = default_lr_delay;

static float dampingValueParam = default_damping;

static float feedbackValueParam = default_feedback;

static float spreadValueParam = default_spread;

static std::vector <std::string> echo_zone_items;

void EditMultipleEchoZonesDialog::InitGUI()
{
	if(m_effects_manager_ptr)
	{

		int listview_itemsCount = (int)m_effects_manager_ptr->echo_zones_vector.size();
		echo_zone_items.resize(listview_itemsCount);

		for(size_t i = 0; i < m_effects_manager_ptr->echo_zones_vector.size(); i++)
		{
			//add text so that imgui does not complain about unique labels
			echo_zone_items[i] = m_effects_manager_ptr->echo_zones_vector.at(i).GetNameString() + 
														" ( " + std::to_string(i) + " )";
		}
		
		//initialize values in GUI text boxes based on first choice
		EditMultipleEchoZonesDialog::EchoZoneSelectedInListBox(m_selection_index);
	}
}

void EditMultipleEchoZonesDialog::DrawDialog()
{
	ImGui::OpenPopup("Edit Echo Zones");

	// Always center this window when appearing
	ImVec2 center = ImGui::GetMainViewport()->GetCenter();
	ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

	if (ImGui::BeginPopupModal("Edit Echo Zones", NULL, ImGuiWindowFlags_AlwaysAutoResize))
	{

		if(m_effects_manager_ptr){

			//render echo zone dropdown 
			static int ez_item_current_idx = 0; // Here we store our selection data as an index.
			const char* ez_combo_preview_value = echo_zone_items[ez_item_current_idx].c_str();  // Pass in the preview value visible before opening the combo (it could be anything)
						
			if (ImGui::BeginCombo("Echo Zone", ez_combo_preview_value, (ImGuiComboFlags)0))
			{
				for (int n = 0; n < echo_zone_items.size(); n++)
				{
					const bool is_selected = (ez_item_current_idx == n);
					if (ImGui::Selectable(echo_zone_items[n].c_str(), is_selected))
					{
						ez_item_current_idx = n;
						EditMultipleEchoZonesDialog::EchoZoneSelectedInListBox(ez_item_current_idx);
					}

					// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
					if (is_selected)
						ImGui::SetItemDefaultFocus();
				}
				
				ImGui::EndCombo();
			}
		}

		//display name input box
		if(ImGui::InputText("Name", ez_char_name, 32))
		{
			ez_name_box_pressed = !ez_name_box_pressed;
		}

		ImGui::Separator();

		//display x value float input box
		ImGui::InputFloat("x", &xValueParam, 0.01f, 1.0f, "%.3f");

		//display y value float input box
		ImGui::InputFloat("y", &yValueParam, 0.01f, 1.0f, "%.3f");

		//display z value float input box
		ImGui::InputFloat("z", &zValueParam, 0.01f, 1.0f, "%.3f");

		//display width value float input box
		ImGui::InputFloat("width", &widthValueParam, 0.01f, 1.0f, "%.3f");

		ImGui::Separator();

		//display standard reverb parameters

		ImGui_Extended_InputFloat("delay", &delayValueParam, 0.01f, 1.0f, "%.3f",min_delay,max_delay);
		ImGui_Extended_InputFloat("lr delay", &lrDelayValueParam, 0.01f, 1.0f, "%.3f",min_lr_delay,max_lr_delay);
		ImGui_Extended_InputFloat("damping", &dampingValueParam, 0.01f, 1.0f, "%.3f",min_damping,max_damping);
		ImGui_Extended_InputFloat("feedback", &feedbackValueParam, 0.01f, 1.0f, "%.3f",min_feedback,max_feedback);
		ImGui_Extended_InputFloat("spread", &spreadValueParam, 0.01f, 1.0f, "%.3f",min_spread,max_spread);

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
		EditMultipleEchoZonesDialog::ChangeEchoZoneAttributes();
	}
	

	
}


void EditMultipleEchoZonesDialog::ChangeEchoZoneAttributes()
{
	
	if(m_selection_index != -1)
	{
		if(m_effects_manager_ptr->echo_zones_vector.size() > 0)
		{
			
			EchoZone* thisEchoZone = &m_effects_manager_ptr->echo_zones_vector.at(m_selection_index);
			
			std::string name = std::string(ez_char_name);
			thisEchoZone->SetNameString(name);	
			
			thisEchoZone->SetPositionX(xValueParam);
			thisEchoZone->SetPositionY(yValueParam);
			thisEchoZone->SetPositionZ(zValueParam);
			
			thisEchoZone->ChangeWidth(widthValueParam);
			
			tempEchoProp.flDelay = delayValueParam;
			tempEchoProp.flDamping = dampingValueParam;
			tempEchoProp.flFeedback = feedbackValueParam;
			tempEchoProp.flLRDelay = lrDelayValueParam;
			tempEchoProp.flSpread = spreadValueParam;
			
			thisEchoZone->ChangeEchoZoneProperties(tempEchoProp);
			
		}
			
	}
	
	
}
	


void EditMultipleEchoZonesDialog::Preview()
{
	/*
	if(effects_manager_ptr->GetReferenceToSoundProducerTracksVector()->size() > 0)
	{
		
		//get sound producer track of first sound producer track
		std::vector <SoundProducerTrack*> *ref_track_vec = effects_manager_ptr->GetReferenceToSoundProducerTracksVector();
		
		if(spt_selection_index != -1 && m_selection_index != -1)
		{
			SoundProducerTrack* thisTrack = ref_track_vec->at(spt_selection_index);
		
			//if track has a sound producer
			if(thisTrack->GetReferenceToSoundProducerManipulated() != nullptr)
			{
				//Create temporary reverb zone
				EchoZone tempZone;
				
				( textFieldX->GetLineText(0) ).ToDouble(&xPosition);
				( textFieldY->GetLineText(0) ).ToDouble(&yPosition);
				( textFieldZ->GetLineText(0) ).ToDouble(&zPosition);
				( textFieldWidth->GetLineText(0) ).ToDouble(&width);
				
				EchoZone* thisEchoZone = &effects_manager_ptr->echo_zones_vector.at(m_selection_index);
					
				
					( textFieldX->GetLineText(0) ).ToDouble(&xPosition);
					( textFieldY->GetLineText(0) ).ToDouble(&yPosition);
					( textFieldZ->GetLineText(0) ).ToDouble(&zPosition);
					( textFieldWidth->GetLineText(0) ).ToDouble(&width);

					( textField_flDelay->GetLineText(0) ).ToDouble(&tempEchoProp.flDelay);
					( textField_flLRDelay->GetLineText(0) ).ToDouble(&tempEchoProp.flLRDelay);
					( textField_flDamping->GetLineText(0) ).ToDouble(&tempEchoProp.flDamping);
					( textField_flFeedback->GetLineText(0) ).ToDouble(&tempEchoProp.flFeedback);
					( textField_flSpread->GetLineText(0) ).ToDouble(&tempEchoProp.flSpread);

					tempZone.InitEchoZone(name,xPosition,yPosition,zPosition,width,tempEchoProp);
					
				
				//apply effect to sound producer track
				effects_manager_ptr->ApplyThisEffectZoneEffectToThisTrack(thisTrack, &tempZone);
				
				//play track
				effects_manager_ptr->m_track_manager_ptr->PlayThisTrackFromSoundProducerTrackVector(spt_selection_index);
				
				//delay for a few seconds 				
				double duration = 4; //seconds
				long endtime = ::wxGetLocalTime() + duration;
				while( ::wxGetLocalTime() < endtime )
				{	
					::wxMilliSleep(100);
				}
				
				//pause instead of stop to avoid crash with stopping source with effect applied
				effects_manager_ptr->m_track_manager_ptr->PauseThisTrackFromSoundProducerTrackVector(spt_selection_index);
				
				//remove effect from sound producer track
				effects_manager_ptr->RemoveEffectFromThisTrack(thisTrack);
				
				//free effect
				tempZone.FreeEffects();
				
			}
				
				
				
			
			else
			{
				wxMessageBox( wxT("Create a soundproducer. Set it to a track. Load audio to it with browse button!") );	
			}
		}
		else
		{
			wxMessageBox( wxT("Select a soundproducer!") );
		}
	}
	*/
}


void EditMultipleEchoZonesDialog::EchoZoneSelectedInListBox(size_t choice)
{
	
	if(m_effects_manager_ptr->echo_zones_vector.size() > 0)
	{
		EchoZone* thisEchoZone = &m_effects_manager_ptr->echo_zones_vector.at(choice);
		
		strncpy(ez_char_name, thisEchoZone->GetNameString().c_str(), 32);
		ez_char_name[31] = '\0';
		
		xValueParam = thisEchoZone->GetPositionX();

		yValueParam = thisEchoZone->GetPositionY();

		zValueParam = thisEchoZone->GetPositionZ();

		widthValueParam = thisEchoZone->GetWidth();

		tempEchoProp = thisEchoZone->GetEchoZoneProperties();
		
		delayValueParam = tempEchoProp.flDelay;

		lrDelayValueParam = tempEchoProp.flLRDelay;

		dampingValueParam = tempEchoProp.flDamping;

		feedbackValueParam = tempEchoProp.flFeedback;

		spreadValueParam = tempEchoProp.flSpread;

	}
	
}

void EditMultipleEchoZonesDialog::SoundProducerTrackSelectedInListBox()
{
	//spt_selection_index = listboxSoundProducers->GetSelection();
}

void EditMultipleEchoZonesDialog::resetConfig()
{
	okClicked = false;
	cancelClicked = false;
}
