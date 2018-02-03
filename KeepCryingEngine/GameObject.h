#ifndef _GAMEOBJECT_H_
#define _GAMEOBJECT_H_

#include <list>
#include <vector>
// #include <assert.h>

#include "Component.h"

class GameObject
{
public:
	/* enum class State{
		ToAwake,
		ToEnable,
		ToStart,
		ToUpdate,
		ToDisable,
		ToDestroy
	}; */

	GameObject(const std::string& name);
	virtual ~GameObject();

	GameObject* GetParent() const;

	size_t GetChildCount() const;

	GameObject* GetChild(size_t index) const;
	const std::vector<GameObject*>& GetChildren() const;

	GameObject* GetChild(unsigned long long int gameObjectId) const;
	GameObject* GetSelfOrChild(unsigned long long int gameObjectId) const;

	const std::string& GetName() const;

	unsigned long long int GetId() const;

	void SetParent(GameObject& newParent);

	Component* AddComponent(ComponentType type);

	Component* GetComponent(ComponentType type);

	const std::vector<Component*>& GetComponents() const;

	std::vector<Component*> GetComponents(ComponentType type);

	void GetComponents(ComponentType type, std::vector<Component*>& components);

	void DrawUI();

	// void Awake() {}
	// void OnEnable() {}
	// void Start() {}
	// void PreUpdate() {}
	void Update(float deltaTimeS, float realDeltaTimeS);
	// void LateUpdate() {}
	// void OnDisable() {}
	void OnDestroy();
	
private:
	void CheckToStart();
	void CheckToDestroy();

	void AddInternalComponent(Component* component);

	// void DestroyAndRelease(Component* component) const;

private:
	// State state;

	GameObject* parent = nullptr;

	std::list<Component*> toStart;
	std::list<Component*> toDestroy;

	std::vector<GameObject*> children;
	std::vector<Component*> components;
	std::string name;

	bool enable = true;
	unsigned long long int id;
};

#endif