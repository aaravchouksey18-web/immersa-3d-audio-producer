#include "EditMultipleEAXReverbZonesDialog.h"

#include "global_variables.h"

#include "imgui.h"
#include "backends/rlImGui.h"
#include "backends/imfilebrowser.h"

#include "eax_reverb_zone_props.h"
#include "backends/imgui_input_extended.h"

#include <string>

EditMultipleEAXReverbZonesDialog::EditMultipleEAXReverbZonesDialog()
{
	okClicked = false;
	cancelClicked = false;
	
	m_selection_index = 0;
	spt_selection_index = 0;
}

void EditMultipleEAXReverbZonesDialog::SetCurrentZoneIndexForEditing(size_t index){m_selection_index = index;}

void EditMultipleEAXReverbZonesDialog::SetPointerToEffectsManager(EffectsManager* effects_manager){m_effects_manager_ptr = effects_manager;}

bool EditMultipleEAXReverbZonesDialog::OkClickedOn(){return okClicked;}

bool EditMultipleEAXReverbZonesDialog::CancelClickedOn(){return cancelClicked;}

static std::string eax_reverb_zone_choices = "";

static int editer_listview_activeIndex = 0;
static int editer_listview_itemsCount = 0;

static bool er_name_box_pressed = false;
static char er_char_name[20] = "name here";

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

static float echoTimeValueParam = default_echo_time;

static float echoDepthValueParam = default_echo_depth;

static float modTimeValueParam = default_mod_time;

static float modDepthValueParam = default_mod_depth;

static float hfRefValueParam = default_hf_ref;

static float lfRefValueParam = default_lf_ref;

static std::vector <std::string> eax_reverb_zone_items;

void EditMultipleEAXReverbZonesDialog::InitGUI()
{
	if(m_effects_manager_ptr)
	{

		int listview_itemsCount = (int)m_effects_manager_ptr->eax_reverb_zones_vector.size();
		eax_reverb_zone_items.resize(listview_itemsCount);

		for(size_t i = 0; i < m_effects_manager_ptr->eax_reverb_zones_vector.size(); i++)
		{
			//add text so that imgui does not complain about unique labels
			eax_reverb_zone_items[i] = m_effects_manager_ptr->eax_reverb_zones_vector.at(i).GetNameString() + 
														" ( " + std::to_string(i) + " )";
		}
		
		//initialize values in GUI text boxes based on first choice
		EditMultipleEAXReverbZonesDialog::ReverbZoneSelectedInListBox(m_selection_index);
	}
}

void EditMultipleEAXReverbZonesDialog::DrawDialog()
{
	ImGui::OpenPopup("Create EAX Reverb Zone");

	// Always center this window when appearing
	ImVec2 center = ImGui::GetMainViewport()->GetCenter();
	ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

	if (ImGui::BeginPopupModal("Create EAX Reverb Zone", NULL, ImGuiWindowFlags_AlwaysAutoResize))
	{

		//render standard reverb zone dropdown 
		static int ez_item_current_idx = 0; // Here we store our selection data as an index.
		// the items vector is rebuilt from the LIVE zone count on every
		// InitGUI(), so a stale ez_item_current_idx (zones deleted since it
		// was last set) can exceed it. Clamp the preview index and skip the
		// combo entirely when there are no zones to list.
		const int ez_items_count = (int)eax_reverb_zone_items.size();
		const int ez_preview_idx = (ez_items_count > 0)
			? ((ez_item_current_idx >= ez_items_count)
			   ? ez_items_count - 1 : ez_item_current_idx)
			: 0;
		const char* ez_combo_preview_value = (ez_items_count > 0)
			? eax_reverb_zone_items[ez_preview_idx].c_str() : "";
					
		if (ez_items_count > 0 && ImGui::BeginCombo("EAX Reverb Zones", ez_combo_preview_value, (ImGuiComboFlags)0))
		{
			for (int n = 0; n < eax_reverb_zone_items.size(); n++)
			{
				const bool is_selected = (ez_item_current_idx == n);
				if (ImGui::Selectable(eax_reverb_zone_items[n].c_str(), is_selected))
				{
					ez_item_current_idx = n;
					EditMultipleEAXReverbZonesDialog::ReverbZoneSelectedInListBox(ez_item_current_idx);
				}

				// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
				if (is_selected)
					ImGui::SetItemDefaultFocus();
			}
			
			ImGui::EndCombo();
		}

		//display name input box
		if(ImGui::InputText("Name", er_char_name, 32))
		{
			er_name_box_pressed = !er_name_box_pressed;
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

		ImGui_Extended_InputFloat("echo time", &echoTimeValueParam, 0.01f, 1.0f, "%.3f",min_echo_time,max_echo_time);
		ImGui_Extended_InputFloat("echo depth", &echoDepthValueParam, 0.01f, 1.0f, "%.3f",min_echo_depth,max_echo_depth);
		ImGui_Extended_InputFloat("mod time", &modTimeValueParam, 0.01f, 1.0f, "%.3f",min_mod_time,max_mod_time);
		ImGui_Extended_InputFloat("mod depth", &modDepthValueParam, 0.01f, 1.0f, "%.3f",min_mod_depth,max_mod_depth);
		ImGui_Extended_InputFloat("HF ref", &hfRefValueParam, 0.01f, 1.0f, "%.3f",min_hf_ref,max_hf_ref);
		ImGui_Extended_InputFloat("LF ref", &lfRefValueParam, 0.01f, 1.0f, "%.3f",min_lf_ref,max_lf_ref);

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
	
	/*
	if( GuiDropdownBox((Rectangle){ 300,50,140,30 }, eax_reverb_zone_choices.c_str(), &editer_dropDownZoneActive, editer_dropDownEditMode) )
	{
		editer_dropDownEditMode = !editer_dropDownEditMode;
		m_selection_index = editer_dropDownZoneActive;
		EditMultipleEAXReverbZonesDialog::ReverbZoneSelectedInListBox(m_selection_index);
	}
	*/
											
	if(okClicked)
	{
		EditMultipleEAXReverbZonesDialog::ChangeEAXReverbZoneAttributes();
	}
	
	/*
	
    wxStaticText* spPreviewText = new wxStaticText(this, -1, wxT("Echo Zone, Sound Track to Preview:"), wxPoint(40, 20));
    
    //make horizontal box to put names in
	wxBoxSizer* hboxEchoZones = new wxBoxSizer(wxHORIZONTAL);
	
	//list box to contain names of Sound Producers to edit, single selection by default 
	listboxEchoZones = new wxListBox(this, wxID_ANY, wxPoint(0, 0), wxSize(100, 20)); 
	
	listboxEchoZones->Bind(wxEVT_LISTBOX,&EditMultipleEAXReverbZonesDialog::EchoZoneSelectedInListBox,this);
	
	//add contents of reverb zones to listbox
	for(size_t i = 0; i < effects_manager_ptr->echo_zones_vector.size(); i++)
	{
		EchoZone* thisEchoZone = effects_manager_ptr->GetPointerToEchoZone(i);
					
		wxString mystring( thisEchoZone->GetNameString() );
		listboxEchoZones->Append(mystring);
	}
	
	//list box to contain names of Sound Producers to edit, single selection by default 
	listboxSoundProducers = new wxListBox(this, wxID_ANY,wxPoint(0, 0), wxSize(100, 20)); 
	
	listboxSoundProducers->Bind(wxEVT_LISTBOX,&EditMultipleEAXReverbZonesDialog::SoundProducerTrackSelectedInListBox,this);
	
	//add contents of soundproducers to listbox
	if(effects_manager_ptr->GetReferenceToSoundProducerTracksVector()->size() > 0)
	{
		for(size_t i = 0; i < effects_manager_ptr->GetReferenceToSoundProducerTracksVector()->size(); i++)
		{
			SoundProducer* thisSoundProducer =  effects_manager_ptr->GetReferenceToSoundProducerTracksVector()->at(i)->GetReferenceToSoundProducerManipulated();
			if(thisSoundProducer)
			{
				wxString mystring( thisSoundProducer->GetNameString() );
				listboxSoundProducers ->Append(mystring);
			}
		}
	}
	
	
    
	
	applyButton = new wxButton(this, wxID_ANY, wxT("Apply"), 
							wxDefaultPosition, wxSize(70, 30));
		
	
	previewButton = new wxButton(this, wxID_ANY, wxT("Preview"), 
								wxDefaultPosition, wxSize(70, 30));
	*/

	
}


void EditMultipleEAXReverbZonesDialog::ChangeEAXReverbZoneAttributes()
{
	
	if(m_selection_index != -1)
	{
		if(m_effects_manager_ptr->eax_reverb_zones_vector.size() > 0)
		{
			
			// clamp: m_selection_index can be stale (zones deleted since the
			// dialog was configured) and at() would throw out_of_range
			size_t ez_edit_idx = m_selection_index;
			size_t ez_count = m_effects_manager_ptr->eax_reverb_zones_vector.size();
			if (ez_edit_idx >= ez_count) { ez_edit_idx = ez_count - 1; }
			ReverbZone* thisReverbZone = &m_effects_manager_ptr->eax_reverb_zones_vector.at(ez_edit_idx);
			
			std::string name = std::string(er_char_name);
			thisReverbZone->SetNameString(name);	
			
			thisReverbZone->SetPositionX(xValueParam);
			thisReverbZone->SetPositionY(yValueParam);
			thisReverbZone->SetPositionZ(zValueParam);
			
			thisReverbZone->ChangeWidth(widthValueParam);
			
			
			tempEAXReverbProp.flDensity = densityValueParam;
			tempEAXReverbProp.flDiffusion = diffusionValueParam;
			tempEAXReverbProp.flGain = gainValueParam;
			tempEAXReverbProp.flGainHF = gainValueHFParam;
			tempEAXReverbProp.flDecayTime = decayValueParam;
			tempEAXReverbProp.flDecayHFRatio = decayHFValueParam;
			tempEAXReverbProp.flReflectionsDelay = reflectionsDelayValueParam;
			tempEAXReverbProp.flReflectionsGain = reflectionsGainValueParam;
			tempEAXReverbProp.flLateReverbDelay = lateReverbDelayValueParam;
			tempEAXReverbProp.flLateReverbGain = lateReverbGainValueParam;
			tempEAXReverbProp.flAirAbsorptionGainHF = airAbsorptionGainValueParam;
			tempEAXReverbProp.flRoomRolloffFactor = roomRolloffValueParam;
			
			tempEAXReverbProp.flEchoTime = echoTimeValueParam;
			tempEAXReverbProp.flEchoDepth = echoDepthValueParam;
			tempEAXReverbProp.flModulationTime = modTimeValueParam;
			tempEAXReverbProp.flModulationDepth = modDepthValueParam;
			tempEAXReverbProp.flHFReference = hfRefValueParam;
			tempEAXReverbProp.flLFReference = lfRefValueParam;
			
			thisReverbZone->ChangeEAXReverbZoneProperties(tempEAXReverbProp);
			
		}
			
	}
	
	
}
	


void EditMultipleEAXReverbZonesDialog::Preview()
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


void EditMultipleEAXReverbZonesDialog::ReverbZoneSelectedInListBox(size_t choice)
{
	
	if(m_effects_manager_ptr->eax_reverb_zones_vector.size() > 0)
	{
		// clamp: a stale choice (from a deleted zone) must not throw at()
		size_t ez_idx = choice;
		size_t ez_count = m_effects_manager_ptr->eax_reverb_zones_vector.size();
		if (ez_idx >= ez_count) { ez_idx = ez_count - 1; }
		ReverbZone* thisReverbZone = &m_effects_manager_ptr->eax_reverb_zones_vector.at(ez_idx);
		
		tempEAXReverbProp = thisReverbZone->GetEAXReverbZoneProperties();
		
		strncpy(er_char_name, thisReverbZone->GetNameString().c_str(), 20);
		er_char_name[19] = '\0';
		
		xValueParam = thisReverbZone->GetPositionX();

		yValueParam = thisReverbZone->GetPositionY();

		zValueParam = thisReverbZone->GetPositionZ();

		widthValueParam = thisReverbZone->GetWidth();

		densityValueParam = tempEAXReverbProp.flDensity;

		diffusionValueParam = tempEAXReverbProp.flDiffusion;

		gainValueParam = tempEAXReverbProp.flGain;

		gainValueHFParam = tempEAXReverbProp.flGainHF;

		decayValueParam = tempEAXReverbProp.flDecayTime;

		decayHFValueParam = tempEAXReverbProp.flDecayHFRatio;

		reflectionsGainValueParam = tempEAXReverbProp.flReflectionsGain;

		reflectionsDelayValueParam = tempEAXReverbProp.flReflectionsDelay;

		lateReverbGainValueParam = tempEAXReverbProp.flLateReverbGain;

		lateReverbDelayValueParam = tempEAXReverbProp.flLateReverbDelay;

		airAbsorptionGainValueParam = tempEAXReverbProp.flAirAbsorptionGainHF;

		roomRolloffValueParam = tempEAXReverbProp.flRoomRolloffFactor;

		echoTimeValueParam = tempEAXReverbProp.flEchoTime;

		echoDepthValueParam = tempEAXReverbProp.flEchoDepth;

		modTimeValueParam = tempEAXReverbProp.flModulationTime;

		modDepthValueParam = tempEAXReverbProp.flModulationDepth;

		hfRefValueParam = tempEAXReverbProp.flHFReference;

		lfRefValueParam = tempEAXReverbProp.flLFReference;
	}
}

void EditMultipleEAXReverbZonesDialog::SoundProducerTrackSelectedInListBox()
{
	//spt_selection_index = listboxSoundProducers->GetSelection();
}

void EditMultipleEAXReverbZonesDialog::resetConfig()
{
	okClicked = false;
	cancelClicked = false;
}
