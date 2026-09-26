#include "CreateEchoZoneDialog.h"

#include "imgui.h"
#include "backends/rlImGui.h"
#include "backends/imfilebrowser.h"

#include "echo_zone_props.h"
#include "backends/imgui_input_extended.h"

CreateEchoZoneDialog::CreateEchoZoneDialog() 
{
	okClicked = false;
	cancelClicked = false;
		
	spt_selection_index = -1;
	
}

void CreateEchoZoneDialog::SetPointerToEffectsManager(EffectsManager* effects_manager){m_effects_manager_ptr = effects_manager;}

std::string& CreateEchoZoneDialog::getNewName(){return name;}

void CreateEchoZoneDialog::getNewPosition(float& x, float& y, float& z)
{
	x = xPosition;
	y = yPosition;
	z = zPosition;
}

float& CreateEchoZoneDialog::getNewWidth(){return width;}
	
EchoZoneProperties& CreateEchoZoneDialog::getNewProperties(){return properties;}

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

void CreateEchoZoneDialog::DrawDialog()
{
	ImGui::OpenPopup("Create Echo Zone");

	// Always center this window when appearing
	ImVec2 center = ImGui::GetMainViewport()->GetCenter();
	ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

	if (ImGui::BeginPopupModal("Create Echo Zone", NULL, ImGuiWindowFlags_AlwaysAutoResize))
	{

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
		name = std::string(ez_char_name);
		xPosition = xValueParam;
		yPosition = yValueParam;
		zPosition = zValueParam;
		width = widthValueParam;
		
		properties.flDamping = dampingValueParam;
		properties.flDelay = delayValueParam;
		properties.flFeedback = feedbackValueParam;
		properties.flLRDelay = lrDelayValueParam;
		properties.flSpread = spreadValueParam;
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
	
	previewButton->Bind(wxEVT_BUTTON,&CreateEchoZoneDialog::OnPreview,this);
	*/
}



void CreateEchoZoneDialog::Preview()
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

bool CreateEchoZoneDialog::CancelClickedOn(){return cancelClicked;}

bool CreateEchoZoneDialog::OkClickedOn(){return okClicked;}

void CreateEchoZoneDialog::resetConfig()
{
	okClicked = false;
	cancelClicked = false;
	
	xValueParam = 0.0f;
	yValueParam = 0.0f;
	zValueParam = 0.0f;
	widthValueParam = 10.0f;

	delayValueParam = default_delay;
	lrDelayValueParam = default_lr_delay;
	dampingValueParam = default_damping;
	feedbackValueParam = default_feedback;
	spreadValueParam = default_spread;
	
	memset(ez_char_name, 0, sizeof(ez_char_name));
	strncpy(ez_char_name, "name here", 20);
	ez_char_name[31] = '\0';
}
