#include "GCore.h"
#include "GCameraController.h"
#include <filesystem>
#include <string>

using namespace std;
using namespace std::filesystem;
GCore::~GCore()
{
	mScene->Serialize(L"file.data");
}

GCore& GCore::GetCore()
{
	static GCore* instance = new GCore();

	return *instance;
}

void GCore::RenderFrame()
{
	Update();

	Draw();
}

void GCore::Initialize(HWND OutputWindow, double width, double height)
{
	mRenderer->PreInitialize(OutputWindow, width, height);


	mFactory = mRenderer->GetFactory();

	LoadScene();
	mRenderer->SetScene(mScene.get());

	if (!LoadAllTexture())
		return;
	mRenderer->SyncTextures(mTextures);

	LoadCamera();
	mRenderer->SetCamera(m_pCamera.get());


	mRenderer->Initialize();
}

void GCore::MsgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	
}

void GCore::OnResize()
{
	mRenderer->OnResize();
}

void GCore::Update()
{
	float dt = mTimer->DeltaTime();

	GCameraController::UpdataCamera(m_pCamera.get(), m_CameraMode, dt);

	mRenderer->Update(dt);
}

void GCore::Draw()
{
	float dt = mTimer->DeltaTime();

	mRenderer->Draw(dt);
}

bool GCore::LoadAllTexture()
{
	unique_ptr<GRiTextureLoader> texloader(mFactory->CreateTextureLoader());

	path str("Texture");
	if (!exists(str))		//�����ȼ��Ŀ¼�Ƿ���ڲ���ʹ���ļ����.
		return 1;
	directory_entry entry(str);		//�ļ����
	if (entry.status().type() == file_type::directory)	//��������C++11��ǿö������
		;// cout << "��·����һ��Ŀ¼" << endl;
	directory_iterator list(str);	        //�ļ��������

	
	int i = 0;
	for (auto& it : list)
	{
		wstring file(L"Texture\\");
		file += it.path().filename().generic_wstring();

		auto tex = texloader->LoadTexture(file);
		//tex->UniqueFileName = (m_TexNames[i]);
		unique_ptr<GRiTexture> temp(tex);

		mTextures.insert(std::pair<string, unique_ptr<GRiTexture>>(it.path().stem().generic_string(), move(temp)));
// 		m_TexNames.emplace_back(it.path().stem().generic_string());
// 		m_NameMap.insert(std::pair<string, int>(m_TexNames[i], i));
// 
// 		
// 		m_Textures.push_back(move(temp));
	}

	return true;
}

void GCore::LoadCamera()
{
	FirstPersonCamera cam;
	//camera->SetViewPort(0.0f, 0.0f, (float)width, (float)height);

	cam.LookAt(XMVectorSet(0.0f, 0.0f, -5.0f, 0.0f), XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f),
		XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f));

	m_pCamera = std::make_unique<FirstPersonCamera>(cam);


	// ��ʼ�����ڴ��ڴ�С�䶯ʱ�޸ĵ�ֵ
	m_pCamera->SetFrustum(XM_PI / 3, mRenderer->AspectRatio(), 0.5f, 1000.0f);

}

void GCore::LoadScene()
{
	mScene->AntiSerialize(L"file.data");
}

GCore::GCore()
{
	mTimer = std::make_unique<GRiTimer>();

	mRenderer = &GDxRenderer::GetRenderer();

	mScene = std::make_unique<GScene>();
}
