#include "EditMultipleSoundProducersDialog.h"

#include "global_variables.h"

#include <cstring>
#include <string>

#include "imgui.h"
#include "backends/rlImGui.h"
#include "backends/imfilebrowser.h"

EditMultipleSoundProducersDialog::EditMultipleSoundProducersDialog(const std::string& title)
{
	current_sound_producer_editing_index = 0;
}

void EditMultipleSoundProducersDialog::Init(std::vector <std::unique_ptr <SoundProducer>> *sound_producer_vector){sound_producer_vector_ref = sound_producer_vector;}

void EditMultipleSoundProducersDialog::SetPointerToSoundBank(SoundBank* thisSoundBank){m_sound_bank_ptr = thisSoundBank;}

static float xValueTemp = 0.0f;
static float yValueTemp = 0.0f;
static float zValueTemp = 0.0f;

static int soundAccountTemp = 0;

static char editsp_char_name[32] = "name here";
static bool editsp_name_box_pressed = false;

static bool editsp_free_roam_box_stat = false;

static std::array <std::string,10> sound_items;
static std::vector <std::string> soundproducer_items;

void EditMultipleSoundProducersDialog::DrawDialog()
{
	ImGui::OpenPopup("Edit Sound Producers");

	// Always center this window when appearing
	ImVec2 center = ImGui::GetMainViewport()->GetCenter();
	ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

	if (ImGui::BeginPopupModal("Edit Sound Producers", NULL, ImGuiWindowFlags_AlwaysAutoResize))
	{
		//render sound producer dropdown 
		static int sp_item_current_idx = 0; // Here we store our selection data as an index.
		// the items vector is rebuilt from the LIVE producer count on every
		// InitGUI(), so a stale sp_item_current_idx (producers deleted since
		// it was last set) can exceed it. Clamp the preview index and skip
		// the combo entirely when there are no producers to list.
		const int sp_items_count = (int)soundproducer_items.size();
		const int sp_preview_idx = (sp_items_count > 0)
			? ((sp_item_current_idx >= sp_items_count)
			   ? sp_items_count - 1 : sp_item_current_idx)
			: 0;
		const char* sp_combo_preview_value = (sp_items_count > 0)
			? soundproducer_items[sp_preview_idx].c_str() : "";
					
		if (sp_items_count > 0 && ImGui::BeginCombo("Sound Producer", sp_combo_preview_value, (ImGuiComboFlags)0))
		{
			for (int n = 0; n < soundproducer_items.size(); n++)
			{
				const bool is_selected = (sp_item_current_idx == n);
				if (ImGui::Selectable(soundproducer_items[n].c_str(), is_selected))
				{
					sp_item_current_idx = n;
					EditMultipleSoundProducersDialog::SoundProducerSelectedInListBox(sp_item_current_idx);
				}

				// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
				if (is_selected)
					ImGui::SetItemDefaultFocus();
			}
			
			ImGui::EndCombo();
		}
	
		ImGui::Separator();

		//display name input box
		if(ImGui::InputText("Name", editsp_char_name, 32))
		{
			editsp_name_box_pressed = !editsp_name_box_pressed;
		}

		ImGui::Separator();

		//display x value float input box
		ImGui::InputFloat("x", &xValueTemp, 0.01f, 1.0f, "%.3f");
		
		//display y value float input box
		ImGui::InputFloat("y", &yValueTemp, 0.01f, 1.0f, "%.3f");
		
		//display z value float input box
		ImGui::InputFloat("z", &zValueTemp, 0.01f, 1.0f, "%.3f");

		ImGui::Separator();

		ImGui::Checkbox("Free Roam",&tempFreeRoamBool );

		ImGui::Separator();

		//render sound choices dropdown 

		if(m_sound_bank_ptr)
		{
			static int item_current_idx = 0; // Here we store our selection data as an index.
			// sound_items is rebuilt from the account lookup table on every
			// InitGUI(); clamp a stale static selection and skip the combo
			// when the table is empty (F2 pattern, keeps this in-bounds)
			const int sound_items_count = (int)sound_items.size();
			const int sound_preview_idx = (sound_items_count > 0)
				? ((item_current_idx >= sound_items_count)
				   ? sound_items_count - 1 : item_current_idx)
				: 0;
			const char* combo_preview_value = (sound_items_count > 0)
				? sound_items[sound_preview_idx].c_str() : "";
			static ImGuiComboFlags flags = 0;
						
			if (sound_items_count > 0 && ImGui::BeginCombo("Sound Account", combo_preview_value, flags))
			{
				for (int n = 0; n < sound_items.size(); n++)
				{
					const bool is_selected = (item_current_idx == n );
					if (ImGui::Selectable(sound_items[n].c_str(), is_selected))
					{
						item_current_idx = n;
						soundAccountTemp = item_current_idx;
					}

					// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
					if (is_selected)
						ImGui::SetItemDefaultFocus();
				}
				ImGui::EndCombo();
			}
			
		}

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
		EditMultipleSoundProducersDialog::ChangeSoundProducerAttributes();
	}
}

void EditMultipleSoundProducersDialog::ChangeSoundProducerAttributes()
{
	
	if(sound_producer_vector_ref != nullptr)
	{
		if(sound_producer_vector_ref->size() > 0)
		{
			// clamp a stale editing index (producers removed since this was set)
			// instead of letting at() throw out_of_range
			size_t sp_edit_idx = current_sound_producer_editing_index;
			size_t sp_edit_count = sound_producer_vector_ref->size();
			if (sp_edit_idx >= sp_edit_count) { sp_edit_idx = sp_edit_count - 1; }
			SoundProducer* thisSoundProducer = sound_producer_vector_ref->at(sp_edit_idx).get();
			
			std::string newname = std::string(editsp_char_name);
			thisSoundProducer->SetNameString(newname);
			
			//change position of selected sound producer based on what is in textfields
			xPosition = xValueTemp;
			yPosition = yValueTemp;
			zPosition = zValueTemp;
			thisSoundProducer->SetPositionX(xPosition);
			thisSoundProducer->SetPositionY(yPosition);
			thisSoundProducer->SetPositionZ(zPosition);
			
			thisSoundProducer->SetFreeRoamBool(tempFreeRoamBool);

			sound_bank_account_num = soundAccountTemp;
			thisSoundProducer->SetAccountNumber(sound_bank_account_num);
		}
		
	}
	
}


void EditMultipleSoundProducersDialog::resetConfig()
{
	current_sound_producer_editing_index = 0;
	
	xPosition = 0;
	yPosition = 0;
	zPosition = 0;
	
	xValueTemp = 0.0f;
	yValueTemp = 0.0f;
	zValueTemp = 0.0f;

	//sound_bank_account_num = 0;
	soundAccountTemp = 0;
	okClicked = false;
	cancelClicked = false;
	tempFreeRoamBool = false;
	memset(editsp_char_name, 0, sizeof(editsp_char_name));
}

void EditMultipleSoundProducersDialog::SoundProducerSelectedInListBox(size_t choice)
{
	current_sound_producer_editing_index = choice;
	
	if(sound_producer_vector_ref != nullptr)
	{
		if(sound_producer_vector_ref->size() > 0)
		{
			// clamp a stale choice (producers removed since the dialog was
			// configured) instead of letting at() throw out_of_range
			size_t sp_idx = choice;
			size_t sp_count = sound_producer_vector_ref->size();
			if (sp_idx >= sp_count) { sp_idx = sp_count - 1; }
			current_sound_producer_editing_index = sp_idx;
			SoundProducer* thisSoundProducer = sound_producer_vector_ref->at(sp_idx).get();
		
			tempFreeRoamBool = thisSoundProducer->GetFreeRoamBool();
			
			strncpy(editsp_char_name, thisSoundProducer->GetNameString().c_str(), 20);
			editsp_char_name[19] = '\0';
			
			//update position text fields to have current position of sound producer selected
			xPosition = thisSoundProducer->GetPositionX();
			yPosition = thisSoundProducer->GetPositionY();
			zPosition = thisSoundProducer->GetPositionZ();
			
			xValueTemp = xPosition;
			yValueTemp = yPosition;
			zValueTemp = zPosition;

			//update sound bank account number
			sound_bank_account_num = thisSoundProducer->GetAccountNumber();
			soundAccountTemp = sound_bank_account_num;
		}
		
	}
}

bool EditMultipleSoundProducersDialog::OkClickedOn(){ return okClicked;}

bool EditMultipleSoundProducersDialog::CancelClickedOn(){return cancelClicked;}

void EditMultipleSoundProducersDialog::InitGUI()
{
	
	if(m_sound_bank_ptr)
	{
		
		//put choices into specific format
		sound_items = m_sound_bank_ptr->GetAccountLookupTable();
		
		//add text so that imgui does not complain about unique labels
		for(size_t i = 0; i < sound_items.size(); i++)
		{
			sound_items[i] = sound_items[i] + " ( " + std::to_string(i) + " )";
		}
		
	}
	
	if(sound_producer_vector_ref)
	{
		// rebuild from the LIVE producer count on every InitGUI (no empty()
		// guard): the list must track producers added or removed between
		// dialog sessions, otherwise the combo goes stale — stale names, and
		// indices pointing at the wrong producer entirely
		{
			int edit_soundproducer_listview_itemsCount = (int)sound_producer_vector_ref->size();
			soundproducer_items.resize(edit_soundproducer_listview_itemsCount);

			for(size_t i = 0; i < sound_producer_vector_ref->size(); i++)
			{
				//add text so that imgui does not complain about unique labels
				soundproducer_items[i] = sound_producer_vector_ref->at(i)->GetNameString() + 
															" ( " + std::to_string(i) + " )";
			}
		}
				
	}
	
	//initialize values in GUI text boxes based on first choice
	EditMultipleSoundProducersDialog::SoundProducerSelectedInListBox(current_sound_producer_editing_index);
}

void EditMultipleSoundProducersDialog::SetCurrentSoundProducerEditedIndex(size_t index)
{
	current_sound_producer_editing_index = index;
}
