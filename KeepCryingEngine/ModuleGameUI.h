#ifndef _MODULEGAMEUI_H_
#define _MODULEGAMEUI_H_

#include "Module.h"

class Canvas;
class Image;
class Button;
class Text;
class InputText;
class GameObject;
class Component;

class ModuleGameUI : public Module
{
public:
	ModuleGameUI();
	~ModuleGameUI();

	update_status Update() override;
	bool CleanUp() override;

	void SetCanvas(Canvas& canvas);
	Canvas* GetCanvas();
	void ClearCanvas();
	GameObject* GetFocusGameObject();
	GameObject* GetHoveringGameObject();

private:
	void UpdateRecursivePreOrder(GameObject* g);
	void UpdateNode(GameObject* g);

	void UpdateComponent(Component * component);

	void CheckUIStatus();

	void PreOrdenZCheck(GameObject* currentNode);

	bool CheckIfMouseOver(GameObject* g);

	void NextFocus();
	bool NextFocusPreOrderZCheck(GameObject* currentNode);

	void UpdateCanvas(Canvas* canvas);
	void UpdateImage(Image* image);
	void UpdateButton(Button* button);
	void UpdateText(Text* text);
	void UpdateInputText(InputText* inputText);

private:
	Canvas* root = nullptr;
	GameObject* focus = nullptr;
	GameObject* hovering = nullptr;
	GameObject* pressed = nullptr;

	//Internal flag for NextFocus function
	bool nextItsFocusFlag = false;
	bool firstFocusAvailableFlag = false;
	GameObject* firstFocusAvailable = nullptr;

	//To make nextFocus only once per tab
	bool alreadyPressed = false;

};

#endif // !_MODULEGAMEUI_H_

