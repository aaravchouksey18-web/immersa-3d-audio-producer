#include "EditMultipleStandardReverbZonesDialog.h"

#include "global_variables.h"

#include "raygui/gui_valid_float.h"
#include "raygui/gui_dropdown_listview.h"

#include <string>

#include "imgui.h"
#include "backends/rlImGui.h"
#include "backends/imfilebrowser.h"

#include "standard_reverb_zone_props.h"
#include "backends/imgui_input_extended.h"

EditMultipleStandardReverbZonesDialog::EditMultipleStandardReverbZonesDialog()
{
	okClicked = false;
	cancelClicked = false;
	
	m_selection_index = 0;
	spt_selection_index = 0;
}

void EditMultipleStandardReverbZonesDialog::SetCurrentZoneIndexForEditing(size_t index){m_selection_index = index;}

void EditMultipleStandardReverbZonesDialog::SetPointerToEffectsManager(EffectsManager* effects_manager){m_effects_manager_ptr = effects_manager;}

bool EditMultipleStandardReverbZonesDialog::OkClickedOn(){return okClicked;}

bool EditMultipleStandardReverbZonesDialog::CancelClickedOn(){return cancelClicked;}


static bool sr_name_box_pressed = false;
static char sr_char_name[32] = "name here";


static float xValueParam = 0.0f;
static float yValueParam = 0.0f;
static float zValueParam = 0.0f;
static float widthValueParam = 0.0f;

static float densityValueParam = default_density;
static float diffusionValueParam = default_diffusion;
static float gainValueParam = default_gain;
static float gainValueHFParam = default_gain_hf;
static float decayValueParam = default_decay;
static float decayHFValueParam = default_decay_hf;
static float reflectionsGainValueParam = default_reflections_gain;
static float reflectionsDelayValueParam = default_reflections_delay;
static float lateReverbGainValueParam = default_late_reverb_gain;
static float lateReverbDelayValueParam = default_late_reverb_delay;
static float airAbsorptionGainValueParam = default_air_absorption;
static float roomRolloffValueParam = default_room_rolloff;

//for displaying choices for standard reverb zone to edit
static std::vector <std::string> standard_reverb_zone_items;

void EditMultipleStandardReverbZonesDialog::InitGUI()
{
	if(m_effects_manager_ptr)
	{

		int listview_itemsCount = (int)m_effects_manager_ptr->standard_reverb_zones_vector.size();
		standard_reverb_zone_items.resize(listview_itemsCount);

		for(size_t i = 0; i < m_effects_manager_ptr->standard_reverb_zones_vector.size(); i++)
		{
			//add text so that imgui does not complain about unique labels
			standard_reverb_zone_items[i] = m_effects_manager_ptr->standard_reverb_zones_vector.at(i).GetNameString() + 
														" ( " + std::to_string(i) + " )";
		}
		
		//initialize values in GUI text boxes based on first choice
		EditMultipleStandardReverbZonesDialog::ReverbZoneSelectedInListBox(m_selection_index);
	}
}

void EditMultipleStandardReverbZonesDialog::DrawDialog()
{
	ImGui::OpenPopup("Edit Standard Reverb Zones Dialog");

	// Always center this window when appearing
	ImVec2 center = ImGui::GetMainViewport()->GetCenter();
	ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

	if (ImGui::BeginPopupModal("Edit Standard Reverb Zones Dialog", NULL, ImGuiWindowFlags_AlwaysAutoResize))
	{
		//display dropdown for selecting standard reverb zone to edit

		if(m_effects_manager_ptr){

			//render standard reverb zone dropdown 
			static int sz_item_current_idx = 0; // Here we store our selection data as an index.
			// the items vector is rebuilt from the LIVE zone count on every
			// InitGUI(), so a stale sz_item_current_idx (zones deleted since
			// it was last set) can exceed it. Clamp the preview index and
			// skip the combo entirely when there are no zones to list.
			const int sz_items_count = (int)standard_reverb_zone_items.size();
			const int sz_preview_idx = (sz_items_count > 0)
				? ((sz_item_current_idx >= sz_items_count)
				   ? sz_items_count - 1 : sz_item_current_idx)
				: 0;
			const char* sz_combo_preview_value = (sz_items_count > 0)
				? standard_reverb_zone_items[sz_preview_idx].c_str() : "";
						
			if (sz_items_count > 0 && ImGui::BeginCombo("Sound Producer", sz_combo_preview_value, (ImGuiComboFlags)0))
			{
				for (int n = 0; n < standard_reverb_zone_items.size(); n++)
				{
					const bool is_selected = (sz_item_current_idx == n);
					if (ImGui::Selectable(standard_reverb_zone_items[n].c_str(), is_selected))
					{
						sz_item_current_idx = n;
						EditMultipleStandardReverbZonesDialog::ReverbZoneSelectedInListBox(sz_item_current_idx);
					}

					// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
					if (is_selected)
						ImGui::SetItemDefaultFocus();
				}
				
				ImGui::EndCombo();
			}
		}


		//display name input box
		if(ImGui::InputText("Name", sr_char_name, 32))
		{
			sr_name_box_pressed = !sr_name_box_pressed;
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

		ImGui_Extended_InputFloat("density", &densityValueParam, 0.01f, 1.0f, "%.3f",min_density,max_density);
		ImGui_Extended_InputFloat("diffusion", &diffusionValueParam, 0.01f, 1.0f, "%.3f",min_diffusion,max_diffusion);
		ImGui_Extended_InputFloat("gain", &gainValueParam, 0.01f, 1.0f, "%.3f",min_gain,max_gain);
		ImGui_Extended_InputFloat("gain HF", &gainValueHFParam, 0.01f, 1.0f, "%.3f",min_gain_hf,max_gain_hf);
		ImGui_Extended_InputFloat("decay", &decayValueParam, 0.01f, 1.0f, "%.3f",min_decay_hf,max_decay_hf);
		ImGui_Extended_InputFloat("decay HF", &decayHFValueParam, 0.01f, 1.0f, "%.3f",min_decay_hf,max_decay_hf);
		ImGui_Extended_InputFloat("reflections gain", &reflectionsGainValueParam, 0.01f, 1.0f, "%.3f",min_reflections_gain,max_reflections_gain);
		ImGui_Extended_InputFloat("reflections delay", &reflectionsDelayValueParam, 0.01f, 1.0f, "%.3f",min_reflections_delay,max_reflections_delay);
		ImGui_Extended_InputFloat("late reverb gain", &lateReverbGainValueParam, 0.01f, 1.0f, "%.3f",min_late_reverb_gain,max_late_reverb_gain);
		ImGui_Extended_InputFloat("late reverb delay", &lateReverbDelayValueParam, 0.01f, 1.0f, "%.3f",min_late_reverb_delay,max_late_reverb_delay);
		ImGui_Extended_InputFloat("air absorption gain", &airAbsorptionGainValueParam, 0.01f, 1.0f, "%.3f",min_air_absorption,max_air_absorption);
		ImGui_Extended_InputFloat("room rolloff", &roomRolloffValueParam, 0.01f, 1.0f, "%.3f",min_room_rolloff,max_room_rolloff);

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
		EditMultipleStandardReverbZonesDialog::ChangeStandardReverbZoneAttributes();	
	}

	bool exit = GuiWindowBox((Rectangle){180,60,620,540},"Edit Standard Reverb Zone");
	
	if(exit){cancelClicked = true;}
	
}


void EditMultipleStandardReverbZonesDialog::ChangeStandardReverbZoneAttributes()
{
	
	if(m_selection_index != -1)
	{
		if(m_effects_manager_ptr->standard_reverb_zones_vector.size() > 0)
		{
			
			// clamp: m_selection_index can be stale (zones deleted since the
			// dialog was configured) and at() would throw out_of_range
			size_t sr_edit_idx = m_selection_index;
			size_t sr_count = m_effects_manager_ptr->standard_reverb_zones_vector.size();
			if (sr_edit_idx >= sr_count) { sr_edit_idx = sr_count - 1; }
			ReverbZone* thisReverbZone = &m_effects_manager_ptr->standard_reverb_zones_vector.at(sr_edit_idx);
			
			std::string name = std::string(sr_char_name);
			thisReverbZone->SetNameString(name);	
			
			thisReverbZone->SetPositionX(xValueParam);
			thisReverbZone->SetPositionY(yValueParam);
			thisReverbZone->SetPositionZ(zValueParam);
			
			thisReverbZone->ChangeWidth(widthValueParam);
			
			
			tempStandardReverbProp.flDensity = densityValueParam;
			tempStandardReverbProp.flDiffusion = diffusionValueParam;
			tempStandardReverbProp.flGain = gainValueParam;
			tempStandardReverbProp.flGainHF = gainValueHFParam;
			tempStandardReverbProp.flDecayTime = decayValueParam;
			tempStandardReverbProp.flDecayHFRatio = decayHFValueParam;
			tempStandardReverbProp.flReflectionsDelay = reflectionsDelayValueParam;
			tempStandardReverbProp.flReflectionsGain = reflectionsGainValueParam;
			tempStandardReverbProp.flLateReverbDelay = lateReverbDelayValueParam;
			tempStandardReverbProp.flLateReverbGain = lateReverbGainValueParam;
			tempStandardReverbProp.flAirAbsorptionGainHF = airAbsorptionGainValueParam;
			tempStandardReverbProp.flRoomRolloffFactor = roomRolloffValueParam;
			
			thisReverbZone->ChangeStandardReverbZoneProperties(tempStandardReverbProp);
			
		}
			
	}
	
	
}
	


void EditMultipleStandardReverbZonesDialog::Preview()
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


void EditMultipleStandardReverbZonesDialog::ReverbZoneSelectedInListBox(size_t choice)
{
	
	if(m_effects_manager_ptr->standard_reverb_zones_vector.size() > 0)
	{
		// clamp: a stale choice (from a deleted zone) must not throw at()
		size_t sr_idx = choice;
		size_t sr_count = m_effects_manager_ptr->standard_reverb_zones_vector.size();
		if (sr_idx >= sr_count) { sr_idx = sr_count - 1; }
		ReverbZone* thisReverbZone = &m_effects_manager_ptr->standard_reverb_zones_vector.at(sr_idx);
		
		tempStandardReverbProp = thisReverbZone->GetStandardReverbZoneProperties();
		
		strncpy(sr_char_name, thisReverbZone->GetNameString().c_str(), 32);
		sr_char_name[31] = '\0';
		
		xValueParam = thisReverbZone->GetPositionX();

		yValueParam = thisReverbZone->GetPositionY();

		zValueParam = thisReverbZone->GetPositionZ();

		widthValueParam = thisReverbZone->GetWidth();

		densityValueParam = tempStandardReverbProp.flDensity;

		diffusionValueParam = tempStandardReverbProp.flDiffusion;

		gainValueParam = tempStandardReverbProp.flGain;

		gainValueHFParam = tempStandardReverbProp.flGainHF;

		decayValueParam = tempStandardReverbProp.flDecayTime;

		decayHFValueParam = tempStandardReverbProp.flDecayHFRatio;

		reflectionsGainValueParam = tempStandardReverbProp.flReflectionsGain;

		reflectionsDelayValueParam = tempStandardReverbProp.flReflectionsDelay;

		lateReverbGainValueParam = tempStandardReverbProp.flLateReverbGain;

		lateReverbDelayValueParam = tempStandardReverbProp.flLateReverbDelay;

		airAbsorptionGainValueParam = tempStandardReverbProp.flAirAbsorptionGainHF;

		roomRolloffValueParam = tempStandardReverbProp.flRoomRolloffFactor;
	}
	
}

void EditMultipleStandardReverbZonesDialog::SoundProducerTrackSelectedInListBox()
{
	//spt_selection_index = listboxSoundProducers->GetSelection();
}

void EditMultipleStandardReverbZonesDialog::resetConfig()
{
	okClicked = false;
	cancelClicked = false;
}
