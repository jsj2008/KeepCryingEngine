#include "ModuleUI.h"

#include <imgui.h>
#include <imgui_impl_sdl_gl3.h>
#include <GL/glew.h>
#include <Windows.h>
#include <fstream>

#include "Application.h"
#include "ModuleWindow.h"
#include "ModuleCamera.h"
#include "ModuleRender.h"
#include "ModuleScene.h"
#include "GameObject.h"

ModuleUI::ModuleUI()
{ }

ModuleUI::~ModuleUI()
{ }

bool ModuleUI::Init()
{
	ImGui_ImplSdlGL3_Init(App->window->window);
	ImGui::StyleColorsClassic();

	return true;
}

bool ModuleUI::Start()
{
	nearPlane = App->camera->GetNearPlane();
	farPlane = App->camera->GetFarPlane();
	aspectRatio = App->camera->GetAspectRatio();
	verticalFOV = App->camera->GetFOV();
	position = App->camera->GetPosition();
	up = App->camera->GetUpVector();
	front = App->camera->GetFrontVector();
	side = App->camera->GetSideVector();
	movementSpeed = App->camera->GetMoveSpeed();
	rotationSpeed = App->camera->GetRotationSpeed();
	dragSpeed = App->camera->GetDragSpeed();
	orbitSpeed = App->camera->GetOrbitSpeed();
	zoomSpeed = App->camera->GetZoomSpeed();

	SetUpTextEditor();

	return true;
}

update_status ModuleUI::Update(float deltaTimeS, float realDeltaTimeS)
{	
	DrawMainMenu();

	glViewport(0, 0, (int)ImGui::GetIO().DisplaySize.x, (int)ImGui::GetIO().DisplaySize.y);
	glClearColor(clearColor[0], clearColor[1], clearColor[2], 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	return update_status::UPDATE_CONTINUE;
}

update_status ModuleUI::PostUpdate(float deltaTimeS, float realDeltaTimeS)
{
	ImGui::Render();

	return update_status::UPDATE_CONTINUE;
}

bool ModuleUI::CleanUp()
{
	ImGui_ImplSdlGL3_Shutdown();
	return true;
}

void ModuleUI::DrawMainMenu()
{
	ImVec2 buttonSize = { 120.0f, 20.0f };

	ImGui_ImplSdlGL3_NewFrame(App->window->window);
	{
		CallWindows();
		CallEntityCreation();

		if(ImGui::BeginMainMenuBar())
		{
			if(ImGui::BeginMenu("Game Object"))
			{
				if(ImGui::MenuItem("Add Empty", nullptr, &addEmptyGameObject))
				{
					addEmptyGameObject = true;
				}
				ImGui::MenuItem("Add Cube", nullptr, &addCubeGameObject);
				ImGui::MenuItem("Add Sphere", nullptr, &addSphereGameObject);				
				ImGui::EndMenu();
			}

			if(ImGui::BeginMenu("Windows"))
			{
				if (ImGui::Button("Inspector", buttonSize))
				{
					hierarchyWindow ^= 1;
				}
				if(ImGui::Button("Camera Controls", buttonSize))
				{
					cameraWindow ^= 1;
				}
				if(ImGui::Button("Speed Controls", buttonSize))
				{
					speedWindow ^= 1;
				}
				if(ImGui::Button("Style Controls", buttonSize))
				{
					styleWindow ^= 1;
				}
				ImGui::EndMenu();
			}

			static GLfloat color[] = { 0.0f, 0.0f, 0.0f, 0.0f };
			if(ImGui::BeginMenu("Parameters"))
			{
				ImGui::Checkbox(" Wireframe Mode", &wireframeEnabled);
				ImGui::Checkbox(" Textures", &textureEnabled);
				ImGui::Checkbox(" Depth Test", &depthEnabled);
				ImGui::Checkbox(" Cull Face", &cullEnabled);
				ImGui::Checkbox(" Color Material", &colormaterialEnabled);
				ImGui::Checkbox(" Antialiasing", &antialiasingEnabled);				
				ImGui::Checkbox(" Fog", &fogEnabled);

				if(ImGui::BeginMenu("Fog parameters"))
				{
					ImGui::SliderFloat(" Density", &fogDensity, 0.0f, 1.0f);
					ImGui::ColorPicker3(" Color", color);
					ImGui::EndMenu();
				}

				//FOG Parameters
				glFogf(GL_FOG_DENSITY, fogDensity);
				glFogfv(GL_FOG_COLOR, color);
				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Light"))
			{
				ImGui::Checkbox(" Lightning", &lightningEnabled);
				if (ImGui::ColorPicker3(" Ambient Light", App->renderer->globalAmbient))
				{
					if (!wireframeEnabled)
					{
						glLightModelfv(GL_LIGHT_MODEL_AMBIENT, App->renderer->globalAmbient);
					}				
				}
				ImGui::EndMenu();
			}

			SetAllParameters();

			static int shaderMode = 0;
			if(ImGui::BeginMenu("Shader Editor"))
			{
				ImGui::Combo("Shaders", &shaderMode, "None\0Fragment\0Vertex");
				if(ImGui::Button("Edit", buttonSize))
				{
					SetTextOnEditor(shaderMode);
					shaderEditorWindow ^= 1;
				}
				ImGui::EndMenu();
			}

			if(ImGui::BeginMenu("About"))
			{
				DrawAboutMenu();
				ImGui::EndMenu();
			}

			ImGui::EndMainMenuBar();
		}
	}
}

void ModuleUI::SetAllParameters()
{
	//glEnable Parameters
	if(textureEnabled)
	{
		if(glIsEnabled(GL_TEXTURE_2D) == GL_FALSE)
		{
			glEnable(GL_TEXTURE_2D);
		}
	}
	else
	{
		if(glIsEnabled(GL_TEXTURE_2D) == GL_TRUE)
		{
			glDisable(GL_TEXTURE_2D);
		}
	}

	if(depthEnabled)
	{
		if(glIsEnabled(GL_DEPTH_TEST) == GL_FALSE)
		{
			glEnable(GL_DEPTH_TEST);
		}
	}
	else
	{
		if(glIsEnabled(GL_DEPTH_TEST) == GL_TRUE)
		{
			glDisable(GL_DEPTH_TEST);
		}
	}

	if(cullEnabled)
	{
		if(glIsEnabled(GL_CULL_FACE) == GL_FALSE)
		{
			glEnable(GL_CULL_FACE);
		}
	}
	else
	{
		if(glIsEnabled(GL_CULL_FACE) == GL_TRUE)
		{
			glDisable(GL_CULL_FACE);
		}
	}

	if(lightningEnabled)
	{
		if(glIsEnabled(GL_LIGHTING) == GL_FALSE)
		{
			glEnable(GL_LIGHTING);
		}
	}
	else
	{
		if(glIsEnabled(GL_LIGHTING) == GL_TRUE)
		{
			glDisable(GL_LIGHTING);
		}
	}

	if(colormaterialEnabled)
	{
		if(glIsEnabled(GL_COLOR_MATERIAL) == GL_FALSE)
		{
			glEnable(GL_COLOR_MATERIAL);
		}
	}
	else
	{
		if(glIsEnabled(GL_COLOR_MATERIAL) == GL_TRUE)
		{
			glDisable(GL_COLOR_MATERIAL);
		}
	}

	if(fogEnabled)
	{
		if(glIsEnabled(GL_FOG) == GL_FALSE)
		{
			glEnable(GL_FOG);
		}
	}
	else
	{
		if(glIsEnabled(GL_FOG) == GL_TRUE)
		{
			glDisable(GL_FOG);
		}
	}

	if(antialiasingEnabled)
	{
		if(glIsEnabled(GL_LINE_SMOOTH) == GL_FALSE && glIsEnabled(GL_BLEND) == GL_FALSE)
		{
			glEnable(GL_BLEND);
			glEnable(GL_LINE_SMOOTH);
		}
	}
	else
	{
		if(glIsEnabled(GL_LINE_SMOOTH) == GL_TRUE && glIsEnabled(GL_BLEND) == GL_TRUE)
		{
			glDisable(GL_BLEND);
			glDisable(GL_LINE_SMOOTH);
		}
	}

	static GLint polygonMode;
	glGetIntegerv(GL_POLYGON_MODE, &polygonMode);
	if(wireframeEnabled)
	{
		if(polygonMode == GL_FILL)
		{
			GLfloat brightYellow[] = {255.0f, 255.0f, 0.1f, 1.0f};
			glLightModelfv(GL_LIGHT_MODEL_AMBIENT, brightYellow);
			cullEnabled = false;
			textureEnabled = false;
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		}		
	}
	else
	{
		if(polygonMode == GL_LINE)
		{
			glLightModelfv(GL_LIGHT_MODEL_AMBIENT, App->renderer->globalAmbient);
			cullEnabled = true;
			textureEnabled = true;
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}
	}
}

void ModuleUI::SetUpTextEditor()
{
	editor.SetLanguageDefinition(TextEditor::LanguageDefinition::GLSL());
}

void ModuleUI::SetTextOnEditor(int shaderMode)
{
	const char* shaderPath = "";

	switch(shaderMode)
	{
		case 0:
			shaderSavePath = nullptr;
			return;
		case 1:
			shaderPath = ".\\Assets\\Shaders\\fragmentShader.frag";
			break;
		case 2:
			shaderPath = ".\\Assets\\Shaders\\vertexShader.vert";
			break;
	}

	shaderSavePath = shaderPath;

	std::ifstream t(shaderPath);
	if(t.good())
	{
		std::string str((std::istreambuf_iterator<char>(t)), std::istreambuf_iterator<char>());
		editor.SetText(str);
	}

	t.close();
}

void ModuleUI::CallEntityCreation()
{
	if(addEmptyGameObject)
	{
		App->scene->AddEmpty(*App->scene->Get(selectedNodeID));
		addEmptyGameObject = false;
	}
	if(addCubeGameObject)
	{
		addCubeGameObject = false;
	}
	if(addSphereGameObject)
	{
		addSphereGameObject = false;
	}
}

void ModuleUI::CallWindows()
{
	if(cameraWindow)
	{
		DrawCameraWindow();
	}	
	if(speedWindow)
	{
		DrawSpeedWindow();
	}
	if(styleWindow)
	{
		DrawStyleWindow();
	}
	if(shaderEditorWindow)
	{
		DrawShaderWindow();
	}
	if (hierarchyWindow)
	{
		DrawHierarchyWindow();
	}
	if(inspectorWindow)
	{
		DrawInspectorWindow();
	}
}

void ModuleUI::DrawCameraWindow()
{
	ImGui::Begin("Camera Controls", &cameraWindow, ImGuiWindowFlags_MenuBar);
	ImGui::CloseCurrentPopup();
	ImGui::InputFloat3("Front", front.ptr(), 2);
	front = App->camera->GetFrontVector();
	ImGui::InputFloat3("Up", up.ptr(), 2);
	up = App->camera->GetUpVector();
	ImGui::InputFloat3("Side", side.ptr(), 2);
	side = App->camera->GetSideVector();
	ImGui::InputFloat3("Position", position.ptr(), 2);

	ImGui::NewLine();

	position = App->camera->GetPosition();
	ImGui::DragFloat("Near plane", &nearPlane, 0.01f, 0.01f, 2.0f, "%.2f");
	App->camera->SetNearPlane(nearPlane);
	ImGui::DragFloat("Far plane", &farPlane, 2.0f, 50.0f, 300.0f, "%.2f");
	App->camera->SetFarPlane(farPlane);
	ImGui::SliderFloat("Field of View", &verticalFOV, 0.1f, pi);
	App->camera->SetFOV(verticalFOV);
	aspectRatio = App->camera->GetAspectRatio();
	ImGui::InputFloat("Aspect ratio", &aspectRatio);
	ImGui::End();
}

void ModuleUI::DrawSpeedWindow()
{
	ImGui::Begin("Speed Controls", &speedWindow, ImGuiWindowFlags_MenuBar);
	ImGui::DragFloat("Movement", &movementSpeed, 0.5f, 0.0f, 100.0f, "%.2f");
	App->camera->SetMoveSpeed(movementSpeed);
	ImGui::DragFloat("Rotation", &rotationSpeed, 0.5f, 0.0f, 100.0f, "%.2f");
	App->camera->SetRotationSpeed(rotationSpeed);
	ImGui::DragFloat("Drag", &dragSpeed, 0.5f, 0.0f, 100.0f, "%.2f");
	App->camera->SetDragSpeed(dragSpeed);
	ImGui::DragFloat("Orbit", &orbitSpeed, 0.5f, 0.0f, 100.0f, "%.2f");
	App->camera->SetOrbitSpeed(orbitSpeed);
	ImGui::DragFloat("Zoom", &zoomSpeed, 0.5f, 0.0f, 100.0f, "%.2f");
	App->camera->SetZoomSpeed(zoomSpeed);
	ImGui::End();
}

void ModuleUI::DrawStyleWindow()
{
	ImGui::Begin("Style Controls", &styleWindow, ImGuiWindowFlags_MenuBar);
	ImGui::DragFloat3("Background", clearColor, 0.01f, 0.0f, 1.0f, "%.2f");
	ImGui::End();
}

void ModuleUI::DrawShaderWindow()
{
	ImGui::Begin("Text Editor Demo", &shaderEditorWindow, ImGuiWindowFlags_HorizontalScrollbar | ImGuiWindowFlags_MenuBar);
	ImGui::SetWindowSize(ImVec2(800, 600), ImGuiCond_FirstUseEver);
	if(ImGui::BeginMenuBar())
	{
		if(ImGui::BeginMenu("File"))
		{
			if(ImGui::MenuItem("Save"))
			{
				std::string textToSave = editor.GetText();
				if(shaderSavePath != nullptr)
				{
					std::ofstream shaderSave(shaderSavePath);
					shaderSave << textToSave;
					shaderSave.close();
				}
			}
			ImGui::EndMenu();
		}
		if(ImGui::BeginMenu("View"))
		{
			if(ImGui::MenuItem("Dark palette"))
				editor.SetPalette(TextEditor::GetDarkPalette());
			if(ImGui::MenuItem("Light palette"))
				editor.SetPalette(TextEditor::GetLightPalette());
			ImGui::EndMenu();
		}
		ImGui::EndMenuBar();
	}

	ImGui::Text("BONES TARDES TINGUIN VOSTES :)");

	editor.Render("TextEditor");
	ImGui::End();
}

void ModuleUI::DrawHierarchyWindow()
{
	ImGui::Begin("Game Object Inspector", &hierarchyWindow, ImGuiWindowFlags_MenuBar);

	ImGuiTreeNodeFlags nodeFlags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick;

	bool opened = ImGui::TreeNodeEx(App->scene->GetRoot()->GetName().c_str(), nodeFlags);

	if(opened)
	{	
		PrintChildrenOnHierarchy(App->scene->GetRoot()->GetChildren());
		ImGui::TreePop();
	}
	
	ImGui::End();
}

void ModuleUI::DrawInspectorWindow()
{
	static int potato = 0;
	ImGui::Begin("Inspector", &inspectorWindow, ImGuiWindowFlags_MenuBar);
	
	GameObject* temp = App->scene->Get(selectedNodeID);
	temp->DrawUI();

	ImGui::End();
}

void ModuleUI::PrintChildrenOnHierarchy(std::vector<GameObject*> children)
{
	ImGuiTreeNodeFlags nodeFlags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick;

	for(GameObject* child : children)
	{
		if(selectedNodeID == child->GetId())
		{
			nodeFlags |= ImGuiTreeNodeFlags_Selected;
		}
		else
		{
			nodeFlags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick;
		}

		if (child->GetChildCount() > 0)
		{
			bool opened = ImGui::TreeNodeEx(child->GetName().c_str(), nodeFlags);

			if(ImGui::IsItemClicked())
			{
				if(!inspectorWindow)
				{
					inspectorWindow = true;
				}
				selectedNodeID = child->GetId();
				int a = 0;
				a++;
			}

			if(opened)
			{
				PrintChildrenOnHierarchy(child->GetChildren());
				ImGui::TreePop();
			}

		}
		else
		{
			bool opened = ImGui::TreeNodeEx(child->GetName().c_str(), nodeFlags);

			if(ImGui::IsItemClicked())
			{
				if(!inspectorWindow)
				{
					inspectorWindow = true;
				}

				selectedNodeID = child->GetId();
				int a = 0;
				a++;
			}

			if(opened)
			{
				ImGui::TreePop();
			}
		}
	}
}

void ModuleUI::DrawAboutMenu()
{
	if(ImGui::Button("KeepCrying Engine"))
	{
		ShellExecute(nullptr, TEXT("open"), TEXT("https://github.com/KeepCryingEngine/KeepCryingEngine"), nullptr, nullptr, 0);
	}

	ImGui::Text("Improved CryEngine version");

	ImGui::NewLine();

	ImGui::Text("Authors:");

	ImGui::Indent();
	if(ImGui::Button("Adrian Leroy Calle"))
	{
		ShellExecute(nullptr, TEXT("open"), TEXT("https://github.com/Moaif"), nullptr, nullptr, 0);
	}
	if(ImGui::Button("Gerard Pros Figueras"))
	{
		ShellExecute(nullptr, TEXT("open"), TEXT("https://github.com/gerardpf2"), nullptr, nullptr, 0);
	}
	if(ImGui::Button("Pere Viader Masuet"))
	{
		ShellExecute(nullptr, TEXT("open"), TEXT("https://github.com/PereViader"), nullptr, nullptr, 0);
	}
	if(ImGui::Button("Xavier Bravo Guillen"))
	{
		ShellExecute(nullptr, TEXT("open"), TEXT("https://github.com/BravoXavi"), nullptr, nullptr, 0);
	}
	ImGui::Unindent();

	ImGui::NewLine();

	ImGui::Text("3rd Party Libraries used:");

	ImGui::Indent();
	ImGui::BulletText("SDL 2.0");
	ImGui::BulletText("Glew 2.1");
	ImGui::BulletText("ImGui 1.53");
	ImGui::BulletText("MathGeolib 1.5");
	ImGui::BulletText("OpenGL 3.1");
	ImGui::Unindent();

	ImGui::NewLine();

	ImGui::Text("License:");

	ImGui::Indent();
	if(ImGui::Button("MIT License"))
	{
		ShellExecute(nullptr, TEXT("open"), TEXT("https://github.com/KeepCryingEngine/KeepCryingEngine/blob/master/LICENSE"), nullptr, nullptr, 0);
	}
	ImGui::Unindent();
}