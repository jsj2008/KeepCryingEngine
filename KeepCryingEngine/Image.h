#ifndef _IMAGE_H_
#define _IMAGE_H_

#include "Component.h"
#include "GameUI.h"

#include <float4.h>
#include <experimental/filesystem>

class Texture;

class Image : public Component,public GameUI
{
public:
	static const Component::Type TYPE = Component::Type::Image;

	Image();
	virtual ~Image();

	void DrawUI() override;

	void SetTextureByPath(const std::experimental::filesystem::path& path);
	void SetTexture(Texture& texture);
	void SetColor(float4 color);

	float4 GetColor() const;
	Texture * GetTexture() const;
private:
	Texture * texture = nullptr;
	float4 color = float4().one;
};

#endif // !_IMAGE_H_

