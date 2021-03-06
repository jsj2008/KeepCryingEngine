#ifndef _JSON_SERIALIZER_H_
#define _JSON_SERIALIZER_H_

#include <Quat.h>
#include <float3.h>
#include <float4.h>
#include <json.hpp>
#include <Frustum.h>
#include <set>
#include <bass.h>
#include <experimental/filesystem>
#include "ENGINE_UUID.h"
#include "Texture.h"
#include "AudioClip.h"

void to_json(nlohmann::json& j, const float2& v);
void from_json(const nlohmann::json& j, float2& v);

void to_json(nlohmann::json& j, const float3& v);
void from_json(const nlohmann::json& j, float3& v);

void to_json(nlohmann::json& j, const float4& v);
void from_json(const nlohmann::json& j, float4& v);

void to_json(nlohmann::json& j, const Quat& v);
void from_json(const nlohmann::json& j, Quat& v);

void to_json(nlohmann::json& j, const Frustum& v);
void from_json(const nlohmann::json& j, Frustum& v);

void to_json(nlohmann::json& j, const BASS_DX8_I3DL2REVERB& v);
void from_json(const nlohmann::json& j, BASS_DX8_I3DL2REVERB& v);

void to_json(nlohmann::json& j, const ENGINE_UUID& v);
void from_json(const nlohmann::json& j, ENGINE_UUID& v);

void to_json(nlohmann::json& j, const TextureConfiguration& v);
void from_json(const nlohmann::json& j, TextureConfiguration& v);

void to_json(nlohmann::json& j, const AudioClipIdentifier& v);
void from_json(const nlohmann::json& j, AudioClipIdentifier& v);

#endif // !_JSON_SERIALIZER_H_