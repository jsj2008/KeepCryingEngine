#include "Transform.h"

#include <MathGeoLib.h>

#include "GameObject.h"

using namespace std;

Transform::Transform() :Component(ComponentType::Transform), position(float3::zero), rotation(Quat::identity), scale(float3::one)
{ }

Transform::~Transform()
{ }

void Transform::DrawUI()
{
	if(ImGui::CollapsingHeader("Transform"))
	{
		ImGui::DragFloat3(" Position", position.ptr(),0.1f);
		float3 angles = RadToDeg(rotation.ToEulerXYZ());
		if(ImGui::DragFloat3(" Rotation", angles.ptr()), 0.1f)
		{
			angles = DegToRad(angles);
			rotation = Quat::FromEulerXYZ(angles.x, angles.y, angles.z);
		}
		ImGui::DragFloat3(" Scale", scale.ptr(), 0.1f);
	}
}

float4x4 Transform::GetAcumulatedTransform()
{
	if(gameObject->GetParent())
	{
		Transform* parent = (Transform*)gameObject->GetParent()->GetComponent(ComponentType::Transform);
		float4x4 parentTransformMatrix = parent->GetAcumulatedTransform();
		return parentTransformMatrix * float4x4::FromTRS(position, rotation.ToFloat4x4(), scale);
	}
	else 
	{
		return float4x4::identity;
	}
}

vector<ComponentType> Transform::GetProhibitedComponents() const
{
	return { ComponentType::Transform };
}