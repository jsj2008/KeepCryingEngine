#include "Animator.h"

#include "Transform.h"
#include "ModuleAnim.h"
#include "GameObject.h"
#include "Application.h"
#include "json_serializer.h"
#include "MeshFilter.h"

using namespace std;

Animator::Animator() : 
	Component(Animator::TYPE)
{ }

Animator::~Animator()
{ }

void Animator::Start()
{
	vector<MeshFilter*> meshFilters = gameObject->GetParent()->GetComponentsInChildren<MeshFilter>();

	for(MeshFilter* meshFilter : meshFilters)
	{
		meshFilter->GetMesh()->SetDynamicDraw(true);
	}
}

void Animator::RealUpdate()
{
	if(HasValidAnimationInstance())
	{
		for(GameObject* child : gameObject->GetChildren())
		{
			UpdateTransformRecursive(animInstanceId, child);
		}
	}
}

void Animator::DrawUI()
{
	if(ImGui::CollapsingHeader("Animator"))
	{
		ImGui::Checkbox("Active", &enabled); ImGui::SameLine();
		if(ImGui::Button("Delete Component"))
		{
			gameObject->RemoveComponent(this);
		}

		static char animatorLoadBuffer[252] = "ArmyPilot/Animations/ArmyPilot_Run_Forwards.fbx";
		ImGui::InputText("##loadAnimation", animatorLoadBuffer, 252); ImGui::SameLine();
		if(ImGui::Button("Load"))
		{
			string s = "Assets/";
			s += animatorLoadBuffer;

			std::experimental::filesystem::path path(s);
			LoadAnimInstance(path);
		}

		for(string animationName : animationNames)
		{
			ImGui::Text(animationName.c_str());

			ImGui::SameLine();

			ImGui::PushID(animationName.c_str());

			if(ImGui::Button("Play"))
			{
				PlayAnimInstance(animationName.c_str());
			}

			ImGui::PopID();

			if(animationName == currentAnimationName)
			{
				if(HasValidAnimationInstance())
				{
					ImGui::SameLine();

					ImGui::ProgressBar(App->anim->GetPercent(animInstanceId));
				}
			}
		}	
	}
}

vector<Component::Type> Animator::GetNeededComponents() const
{
	return { Transform::TYPE };
}

vector<Component::Type> Animator::GetProhibitedComponents() const
{
	return { Animator::TYPE };
}

unsigned int Animator::GetAnimInstanceId() const
{
	return animInstanceId;
}

void Animator::LoadAnimInstance(const std::experimental::filesystem::path& path)
{
	set<string> newAnimationNames = App->anim->Load(path);
	
	if (newAnimationNames.size() > 0)
	{
		animationPaths.insert(path);
	}

	animationNames.insert(newAnimationNames.begin(), newAnimationNames.end());
}

void Animator::PlayAnimInstance(const char* name)
{
	currentAnimationName = name;

	if(HasValidAnimationInstance())
	{
		App->anim->BlendTo(animInstanceId, name, 1000);
	}
	else
	{
		animInstanceId = App->anim->Play(name);
	}
}

bool Animator::HasValidAnimationInstance() const
{
	return animInstanceId != -1;
}

void Animator::PreLoad(const nlohmann::json & json)
{
	Component::PreLoad(json);
	currentAnimationName = json["currentAnimationName"].get<string>();
	
	for (const nlohmann::json& j : json["paths"])
	{
		LoadAnimInstance(j.get<string>()); //I do not agree with this - Xavi.
	}
}

void Animator::Save(nlohmann::json & json) const
{
	Component::Save(json);
	json["currentAnimationName"] = currentAnimationName;

	vector<std::string> animationPathsString;
	for each (std::experimental::filesystem::path path in animationPaths)
	{
		animationPathsString.push_back(path.string());
	}
	json["paths"] = animationPathsString;
}

void Animator::UpdateTransformRecursive(unsigned int animInstanceId, GameObject* gameObject)
{
	aiVector3D position;
	aiQuaternion rotation;

	App->anim->GetTransform(animInstanceId, gameObject->GetName().c_str(), position, rotation);

	Transform* transform = gameObject->GetTransform();

	transform->SetLocalPosition(float3(position.x, position.y, position.z));
	transform->SetLocalRotation(Quat(rotation.x, rotation.y, rotation.z, rotation.w));

	for(GameObject* child : gameObject->GetChildren())
	{
		UpdateTransformRecursive(animInstanceId, child);
	}
}