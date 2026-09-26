#include "CreateEAXReverbZoneDialog.h"

#include "imgui.h"
#include "backends/rlImGui.h"
#include "backends/imfilebrowser.h"

#include "eax_reverb_zone_props.h"
#include "backends/imgui_input_extended.h"

CreateEAXReverbZoneDialog::CreateEAXReverbZoneDialog() 
{
	okClicked = false;
	cancelClicked = false;
		
	spt_selection_index = -1;
	
}

void CreateEAXReverbZoneDialog::SetPointerToEffectsManager(EffectsManager* effects_manager){m_effects_manager_ptr = effects_manager;}

std::string& CreateEAXReverbZoneDialog::getNewName(){return name;}

void CreateEAXReverbZoneDialog::getNewPosition(float& x, float& y, float& z)
{
	x = xPosition;
	y = yPosition;
	z = zPosition;
}

float& CreateEAXReverbZoneDialog::getNewWidth(){return width;}
	
ReverbEAXProperties& CreateEAXReverbZoneDialog::getNewProperties(){return properties;}

static bool er_name_box_pressed = false;
static char er_char_name[32] = "name here";

static float xValueParam = 0.0f;

static float yValueParam = 0.0f;

static float zValueParam = 0.0f;

static float widthValueParam = 10.0f;

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

void CreateEAXReverbZoneDialog::DrawDialog()
{
	ImGui::OpenPopup("Create EAX Reverb Zone");

	// Always center this window when appearing
	ImVec2 center = ImGui::GetMainViewport()->GetCenter();
	ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

	if (ImGui::BeginPopupModal("Create EAX Reverb Zone", NULL, ImGuiWindowFlags_AlwaysAutoResize))
	{

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
	
	if(okClicked)
	{
		name = std::string(er_char_name);
		xPosition = xValueParam;
		yPosition = yValueParam;
		zPosition = zValueParam;
		width = widthValueParam;
		
		
		properties.flDensity = densityValueParam;
		properties.flDiffusion = diffusionValueParam;
		properties.flGain = gainValueParam;
		properties.flGainHF = gainValueHFParam;
		properties.flDecayTime = decayValueParam;
		properties.flDecayHFRatio = decayHFValueParam;
		properties.flReflectionsDelay = reflectionsDelayValueParam;
		properties.flReflectionsGain = reflectionsGainValueParam;
		properties.flLateReverbDelay = lateReverbDelayValueParam;
		properties.flLateReverbGain = lateReverbGainValueParam;
		properties.flAirAbsorptionGainHF = airAbsorptionGainValueParam;
		properties.flRoomRolloffFactor = roomRolloffValueParam;
		
		properties.flEchoTime = echoTimeValueParam;
		properties.flEchoDepth = echoDepthValueParam;
		properties.flModulationTime = modTimeValueParam;
		properties.flModulationDepth = modDepthValueParam;
		properties.flHFReference = hfRefValueParam;
		properties.flLFReference = lfRefValueParam;
		
	}
    /*
	
	//add contents of soundproducers to listbox
	if(m_effects_manager_ptr->GetReferenceToSoundProducerTracksVector()->size() > 0)
	{
		for(size_t i = 0; i < m_effects_manager_ptr->GetReferenceToSoundProducerTracksVector()->size(); i++)
		{
			SoundProducer* thisSoundProducer =  m_effects_manager_ptr->GetReferenceToSoundProducerTracksVector()->at(i)->GetReferenceToSoundProducerManipulated();
			if(thisSoundProducer)
			{
				wxString mystring( thisSoundProducer->GetNameString() );
				listboxSoundProducers ->Append(mystring);
			}
		}
	}
	
	
	previewButton = new wxButton(this, wxID_ANY, wxT("Preview"), 
								wxDefaultPosition, wxSize(70, 30));
	
	previewButton->Bind(wxEVT_BUTTON,&CreateEAXReverbZoneDialog::OnPreview,this);
	*/
}



void CreateEAXReverbZoneDialog::Preview()
{
	/*
	if(m_effects_manager_ptr->GetReferenceToSoundProducerTracksVector()->size() > 0)
	{
		
		//get sound producer track of first sound producer track
		std::vector <SoundProducerTrack*> *ref_track_vec = m_effects_manager_ptr->GetReferenceToSoundProducerTracksVector();
		
		if(spt_selection_index != -1)
		{
			SoundProducerTrack* thisTrack = ref_track_vec->at(spt_selection_index);
		
			//if track has a sound producer
			if(thisTrack->GetReferenceToSoundProducerManipulated() != nullptr)
			{
				//Create temporary reverb zone
				EchoZone tempZone;
				
				name = textFieldName->GetLineText(0).ToStdString();
				( textFieldX->GetLineText(0) ).ToDouble(&xPosition);
				( textFieldY->GetLineText(0) ).ToDouble(&yPosition);
				( textFieldZ->GetLineText(0) ).ToDouble(&zPosition);
				( textFieldWidth->GetLineText(0) ).ToDouble(&width);
				
				( textField_flDelay->GetLineText(0) ).ToDouble(&properties.flDelay);
				( textField_flLRDelay->GetLineText(0) ).ToDouble(&properties.flLRDelay);
				( textField_flDamping->GetLineText(0) ).ToDouble(&properties.flDamping);
				( textField_flFeedback->GetLineText(0) ).ToDouble(&properties.flFeedback);
				( textField_flSpread->GetLineText(0) ).ToDouble(&properties.flSpread);
				
				tempZone.InitEchoZone(name,xPosition,yPosition,zPosition,width,properties);
				
				//apply effect to sound producer track
				m_effects_manager_ptr->ApplyThisEffectZoneEffectToThisTrack(thisTrack, &tempZone);
				
				//play track

				m_effects_manager_ptr->m_track_manager_ptr->PlayThisTrackFromSoundProducerTrackVector(spt_selection_index);
				
				//delay for a few seconds 				
				double duration = 4; //seconds
				long endtime = ::wxGetLocalTime() + duration;
				while( ::wxGetLocalTime() < endtime )
				{	
					::wxMilliSleep(100);
				}
				
				//pause instead of stop to avoid crash with stopping source with effect applied
				m_effects_manager_ptr->m_track_manager_ptr->PauseThisTrackFromSoundProducerTrackVector(spt_selection_index);
				
				//remove effect from sound producer track
				m_effects_manager_ptr->RemoveEffectFromThisTrack(thisTrack);
				
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

bool CreateEAXReverbZoneDialog::CancelClickedOn(){return cancelClicked;}

bool CreateEAXReverbZoneDialog::OkClickedOn(){return okClicked;}

void CreateEAXReverbZoneDialog::resetConfig()
{
	okClicked = false;
	cancelClicked = false;
	
	xValueParam = 0.0f;

	yValueParam = 0.0f;

	zValueParam = 0.0f;

	widthValueParam = 10.0f;

	densityValueParam = default_density;

	diffusionValueParam = default_diffusion;

	gainValueParam = default_gain;

	gainValueHFParam = default_gain_hf;

	decayValueParam = default_decay;

	decayHFValueParam = default_decay_hf;

	reflectionsGainValueParam = default_reflections_gain;

	reflectionsDelayValueParam = default_reflections_delay;

	lateReverbGainValueParam = default_late_reverb_gain;

	lateReverbDelayValueParam = default_late_reverb_delay;

	airAbsorptionGainValueParam = default_air_absorption;

	roomRolloffValueParam = default_room_rolloff;

	echoTimeValueParam = default_echo_time;

	echoDepthValueParam = default_echo_depth;

	modTimeValueParam = default_mod_time;

	modDepthValueParam = default_mod_depth;

	hfRefValueParam = default_hf_ref;

	lfRefValueParam = default_lf_ref;
	
	memset(er_char_name, 0, sizeof(er_char_name));
	strncpy(er_char_name, "name here", 32);
	er_char_name[31] = '\0';
}
