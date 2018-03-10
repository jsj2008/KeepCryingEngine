#include "Button.h"

#include "Application.h"
#include "ModuleTexture.h"
#include "ModuleGameUI.h"
#include "ModuleInput.h"
#include "Image.h"
#include "GameObject.h"
#include <SDL.h>

Button::Button(): Component(Button::TYPE)
{
}

Button::~Button()
{}

void Button::Awake()
{
	isFocuseableUI = true;
	isHovereableUI = true;
	gameObject->SetFocuseableUI(true);
	gameObject->SetHovereableUI(true);
	for(int i = 0; i < (int)ButtonState::NUMBER_STATES; i++)
	{
		colors[i] = float4().one;
		textures[i] = nullptr;
	}
}

void Button::Destroy()
{
	isFocuseableUI = false;
	isHovereableUI = false;
	gameObject->CheckIfFocuseableUI();
	gameObject->CheckIfHovereableUI();
}

void Button::RealUpdate(float deltaTimeS, float realDeltaTimeS)
{
	Image* img = gameObject->GetComponent<Image>();
	if(img != nullptr)
	{
		switch(transitionType)
		{
			case Transition::COLOR:
			{
				img->SetTexture(*textures[(unsigned int)ButtonState::DISABLED]);
				img->SetColor(colors[(unsigned int)state]);
			}
				break;
			case Transition::SPRITE:
			{
				img->SetTexture(*textures[(unsigned int)state]);
			}
				break;
			default:
				assert(false);
				break;
		}
	}

	state = ButtonState::DISABLED;
}

void Button::DrawUI()
{
	if(ImGui::CollapsingHeader("Button"))
	{
		ImGui::Checkbox("Active", &enabled); ImGui::SameLine();
		if(ImGui::Button("Delete Component"))
		{
			gameObject->RemoveComponent(this);
		}
		static char textureDisabledPath[252] = "Lenna.png";
		ImGui::Text("Disabled"); ImGui::SameLine();
		ImGui::InputText("##buttonDisabledTexture", textureDisabledPath, 252); ImGui::SameLine();
		if(ImGui::Button("Set Texture"))
		{
			std::experimental::filesystem::path path = "Assets/";
			path /= textureDisabledPath;

			SetTextureByPath(path, ButtonState::DISABLED);
		}

		static int transitionMode = (int)transitionType;
		if(ImGui::Combo("Transition", &transitionMode, "Color\0Sprite"))
		{
			transitionType = (Transition)transitionMode;
		}

		switch(transitionType)
		{
			case Transition::COLOR:
			{
				ImGui::DragFloat4("Disabled", colors[(unsigned int)ButtonState::DISABLED].ptr(), 0.01f, 0.0f, 1.0f);
				ImGui::DragFloat4("Hover", colors[(unsigned int)ButtonState::HOVER].ptr(), 0.01f, 0.0f, 1.0f);
				ImGui::DragFloat4("Pressed", colors[(unsigned int)ButtonState::PRESSED].ptr(), 0.01f, 0.0f, 1.0f);
			}
			break;
			case Transition::SPRITE:
			{
				ImGui::PushID(gameObject->GetId());
				static char textureHoverPath[252] = "rock.jpg";
				ImGui::Text("Hover"); ImGui::SameLine();
				ImGui::InputText("##buttonHoverTexture", textureHoverPath, 252); ImGui::SameLine();
				if(ImGui::Button("Set Texture"))//TODO:CHANGE NAME/ID
				{
					std::experimental::filesystem::path path = "Assets/";
					path /= textureHoverPath;

					SetTextureByPath(path, ButtonState::HOVER);
				}

				ImGui::PushID(gameObject->GetId() + 1);
				static char texturePressedPath[252] = "exodia.dds";
				ImGui::Text("Pressed"); ImGui::SameLine();
				ImGui::InputText("##buttonPressedTexture", texturePressedPath, 252); ImGui::SameLine();
				if(ImGui::Button("Set Texture"))//TODO:CHANGE NAME/ID
				{
					std::experimental::filesystem::path path = "Assets/";
					path /= texturePressedPath;

					SetTextureByPath(path, ButtonState::PRESSED);
				}
				ImGui::PopID();

				ImGui::PopID();
			}
			break;
			default:
				assert(false);
				break;
		}
	}
}

void Button::OnHovering()
{
	state = ButtonState::HOVER;
}

void Button::OnClick()
{
	state = ButtonState::PRESSED;
}

void Button::SetTextureByPath(const std::experimental::filesystem::path & path, ButtonState state)
{
	if(textures[(unsigned int)state] != nullptr)
	{
		App->texture->Release(textures[(unsigned int)state]);
	}

	textures[(unsigned int)state] = App->texture->GetAsset(path);
}

void Button::SetColor(float4 newColor, ButtonState state)
{
	colors[(unsigned int)state] = newColor;
}

void Button::SetTextGameObject(GameObject& g)
{
	textGameObject = &g;
}

Texture * Button::GetTexture(ButtonState state) const
{
	return textures[(unsigned int) state];
}

float4 Button::GetColor(ButtonState state) const
{
	return colors[(unsigned int)state];
}

GameObject * Button::GetTextGameObject() const
{
	return textGameObject;
}
