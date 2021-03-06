#ifndef _MODULEEDITORUI_H_
#define _MODULEEDITORUI_H_

#include <float3.h>
#include <TextEditor.h>

#include "Module.h"
#include "GameObject.h"

enum class ClickMode
{
	Drag,
	Pick
};

class ModuleEditorUI : public Module
{
public:
	ModuleEditorUI();
	virtual ~ModuleEditorUI();

	bool Init() override;
	bool Start() override;
	bool CleanUp() override;
	update_status Update() override;
	update_status PostUpdate() override;

	bool GetFrustumCulling()const;
	bool GetDebugMode()const;
	ClickMode GetClickMode() const;
	void SetSelectedNodeID(int id);
	int GetSelectedNode()const;
	void OpenInspectorWindow();
	void CloseInspectorWindow();

private:
	void SetUpTextEditor();
	void SetTextOnEditor(int shaderMode);

	void CallWindows();
	void SetAllParameters();
	void DrawEditorControler();
	void DrawMainMenu();
	void DrawAboutMenu();
	void DrawCameraWindow();
	void DrawSpeedWindow();
	void DrawShaderWindow();
	void DrawHierarchyWindow();
	void DrawInspectorWindow();
	void DrawGenerateGameObjectWindow();
	void DrawSpacePartitioningWindow();
	void DrawLoadedTexturesInfoWindow();
	void DrawPerformanceInfoWindow();

	void PrintChildrenOnHierarchy(std::vector<GameObject*> children);

	void CallGuizmo();
	void CallEntityCreation();

private:
	TextEditor editor;
	const char* shaderSavePath = nullptr;

	//Create entities
	bool addCubeGameObject = false;
	bool addSphereGameObject = false;
	bool addPlaneGameObject = false;
	bool addEmptyGameObject = false;
	bool addCameraGameObject = false;
	bool addCanvasGameObject = false;
	bool addImageGameObject = false;
	bool addButtonGameObject = false;
	bool addTextGameObject = false;
	bool addInputTextGameObject = false;

	//Parameter control bools
	bool frustumCulling = false;
	bool wireframeEnabled = false;
	bool antialiasingEnabled = true;
	bool textureEnabled = true;
	bool cullEnabled = true;
	bool lightningEnabled = true;
	bool depthEnabled = true;
	bool colormaterialEnabled = true;
	bool fogEnabled = false;
	bool debugMode = false;

	//One bool control for each window
	bool cameraWindow = false;
	bool speedWindow = false;
	bool shaderEditorWindow = false;
	bool hierarchyWindow = true;
	bool inspectorWindow = false;
	bool generateGameObjectWindow = false;
	bool spacePartitioningWindow = false;
	bool loadedTexturesInfoWindow = false;
	bool editorControler = true;
	bool performanceInfoWindow = false;

	//Editor click
	ClickMode clickMode = ClickMode::Pick;

	//Probably there's a better way of doing this thing...
	float movementSpeed = 10.0f;
	float rotationSpeed = 2.0f;
	float dragSpeed = 3.0f;
	float orbitSpeed = 3.0f;
	float zoomSpeed = 3.0f;
	float verticalFOV = 1.0f;
	float aspectRatio = 0.0f;
	float nearPlane = 0.1f;
	float farPlane = 50.0f;
	float clearColor[3] = { 0.0f, 0.0f, 0.0f };

	float fogDensity = 1.0f;

	int selectedNodeID = 0;
};

#endif // !_MODULEEDITORUI_H_