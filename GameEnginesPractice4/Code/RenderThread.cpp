#include "RenderEngine.h"

// Creating Critical section interface
std::mutex RC_CriticalSection;
#define LOADINGCOMMAND_CRITICAL_SECTION std::scoped_lock<std::mutex> criticalSection (RC_CriticalSection);

// Function to run render thread
static unsigned RunThisThread(void* thisPtr)
{
	RenderThread* const self = (RenderThread*)thisPtr;
	self->Run();

	return 0;
}

template<typename T>
inline void AddRaw(byte*& ptr, const T Val)
{
  *(T*)ptr = Val;
  ptr += sizeof(T);
}

inline void AddBytes(byte*& ptr, byte* copy, uint32_t sz)
{
  memcpy(ptr, copy, sz);
  ptr += sz;
}


RenderThread::RenderThread(RenderEngine* pRenderEngine) :
	m_pRenderEngine(pRenderEngine),
	m_nRenderThreadId(0),
	m_nCurrentFrame(0),
	m_nFrameFill(1),
	m_nFlush(0),
	m_pThread(nullptr)
{
	m_nMainThreadId = ::GetCurrentThreadId();

	m_Commands[0].clear();
	m_Commands[1].clear();
}

RenderThread::~RenderThread()
{

}

// Render Loop
void RenderThread::Run()
{
	m_nRenderThreadId = ::GetCurrentThreadId();

	while (true)
	{
		WaitForMainThreadSignal();

		RC_BeginFrame();

		ProcessCommands();

		SignalMainThread();

		m_pRenderEngine->Update();

		RC_EndFrame();

		if (m_pRenderEngine->GetQuit())
			break;
	}
}

void RenderThread::Start()
{
	if (!m_pThread)
	{
		m_pThread = std::unique_ptr<std::thread>(new std::thread(RunThisThread, this));
	}
}

bool RenderThread::IsRenderThread()
{
	return m_nRenderThreadId == ::GetCurrentThreadId();
}

// We have 2 buffers. One we fill, another - execute
void RenderThread::NextFrame()
{
	m_nCurrentFrame = (m_nCurrentFrame + 1) & 1;
	m_nFrameFill = (m_nFrameFill + 1) & 1;
}

bool RenderThread::CheckFlushCond()
{
	return *(int*)&m_nFlush != 0;
}

// Signal main thread, that he can continue his work
void RenderThread::SignalMainThread()
{
	m_nFlush = 0;
}

// Signal render thread, that he can continue his work
void RenderThread::SignalRenderThread()
{
	m_nFlush = 1;
}

// Process commands that render thread received from main thread
void RenderThread::ProcessCommands()
{
	assert(IsRenderThread());

	if (!CheckFlushCond())
		return;

	int n = 0;

	while (n < m_Commands[m_nCurrentFrame].capacity())
	{
		byte* ptr = m_Commands[m_nCurrentFrame].data() + n;
		n += sizeof(UINT32);
		UINT32 nCommandType =*((UINT32*)ptr);

		switch (nCommandType)
		{
		case eRC_Init:
		{
			m_pRenderEngine->RT_Init();
			break;
		}
		case eRC_SetupDefaultCamera:
		{
			m_pRenderEngine->RT_SetupDefaultCamera();
			break;
		}
		case eRC_SetupDefaultCompositor:
		{
			m_pRenderEngine->RT_SetupDefaultCompositor();
			break;
		}
		case eRC_LoadDefaultResources:
		{
			m_pRenderEngine->RT_LoadDefaultResources();
			break;
		}
		case eRC_LoadOgreHead:
		{
			m_pRenderEngine->RT_LoadOgreHead();
			break;
		}
		case eRC_SetupDefaultLight:
		{
			m_pRenderEngine->RT_SetupDefaultLight();
			break;
		}
		case eRC_OscillateCamera:
		{
			float time = ReadCommand<float>(n);
			m_pRenderEngine->RT_OscillateCamera(time);
			break;
		}
		case eRC_MoveOgreHead:
		{
			float offset = ReadCommand<float>(n);
			m_pRenderEngine->RT_MoveOgreHead(offset);
			break;
		}
    case eRC_SceneNodeSetPosition:
    {
      Ogre::SceneNode *node = ReadCommand<Ogre::SceneNode*>(n);
      Ogre::Vector3 pos = ReadCommand<Ogre::Vector3>(n);
      m_pRenderEngine->RT_SceneNodeSetPosition(node, pos);
      break;
    }
    case eRC_CreateSceneNode:
    {
      uint32_t index = ReadCommand<uint32_t>(n);
      uint32_t len = ReadCommand<uint32_t>(n);

      Ogre::String str;
      str.resize(len);
      ReadMem(n, (byte*)str.c_str(), len);

      Ogre::SceneNode *node = m_pRenderEngine->RT_CreateSceneNode(str);
      m_deliveryQueue[m_nFrameFill][index] = node;
      break;
    }
		}
	}

	m_Commands[m_nCurrentFrame].shrink_to_fit();
}

// We process comands via byte* using std::vector as raw data.
template <class T>
T RenderThread::ReadCommand(int& nIndex)
{
	byte* Res = m_Commands[m_nCurrentFrame].data() + nIndex;
	nIndex += sizeof(T);
	return *reinterpret_cast<const T*>(Res);
}

void RenderThread::ReadMem(int& nIndex, byte *dest, int sz)
{
	byte* Res = m_Commands[m_nCurrentFrame].data() + nIndex;
  memcpy(dest, Res, sz);
  nIndex += sz;
}

byte* RenderThread::AddCommand(RenderCommand eRC, size_t nParamBytes)
{
	UINT32 cmdSize = sizeof(RenderCommand) + nParamBytes;
	byte* storage = new byte[m_Commands[m_nFrameFill].capacity()];

	memcpy(storage, m_Commands[m_nFrameFill].data(), m_Commands[m_nFrameFill].capacity());
	m_Commands[m_nFrameFill].reserve(m_Commands[m_nFrameFill].capacity() * sizeof(byte) + cmdSize);
	memcpy(m_Commands[m_nFrameFill].data(), storage, m_Commands[m_nFrameFill].capacity() - cmdSize);

	byte* ptr = m_Commands[m_nFrameFill].data() + m_Commands[m_nFrameFill].capacity() * sizeof(byte) - cmdSize;
	AddDWORD(ptr, eRC);
  delete[] storage;
	return ptr;
}

void RenderThread::AddDWORD(byte*& ptr, UINT32 nVal)
{
	*(UINT32*)ptr = nVal;
	ptr += sizeof(UINT32);
}

void RenderThread::AddFloat(byte*& ptr, const float fVal)
{
	*(float*)ptr = fVal;
	ptr += sizeof(float);
}

void RenderThread::RC_Init()
{
	if (IsRenderThread())
	{
		m_pRenderEngine->RT_Init();
		return;
	}

	LOADINGCOMMAND_CRITICAL_SECTION;
	byte* p = AddCommand(eRC_Init, 0);
}

void RenderThread::RC_SetupDefaultCamera()
{
	if (IsRenderThread())
	{
		m_pRenderEngine->RT_SetupDefaultCamera();
		return;
	}

	LOADINGCOMMAND_CRITICAL_SECTION;
	byte* p = AddCommand(eRC_SetupDefaultCamera, 0);
}

void RenderThread::RC_SetupDefaultCompositor()
{
	if (IsRenderThread())
	{
		m_pRenderEngine->RT_SetupDefaultCompositor();
		return;
	}

	LOADINGCOMMAND_CRITICAL_SECTION;
	byte* p = AddCommand(eRC_SetupDefaultCompositor, 0);
}

void RenderThread::RC_LoadDefaultResources()
{
	if (IsRenderThread())
	{
		m_pRenderEngine->RT_LoadDefaultResources();
		return;
	}

	LOADINGCOMMAND_CRITICAL_SECTION;
	byte* p = AddCommand(eRC_LoadDefaultResources, 0);
}

void RenderThread::RC_LoadOgreHead()
{
	if (IsRenderThread())
	{
		m_pRenderEngine->RT_LoadOgreHead();
		return;
	}

	LOADINGCOMMAND_CRITICAL_SECTION;
	byte* p = AddCommand(eRC_LoadOgreHead, 0);
}

void RenderThread::RC_SetupDefaultLight()
{
	if (IsRenderThread())
	{
		m_pRenderEngine->RT_SetupDefaultLight();
		return;
	}

	LOADINGCOMMAND_CRITICAL_SECTION;
	byte* p = AddCommand(eRC_SetupDefaultLight, 0);
}

void RenderThread::RC_OscillateCamera(float time)
{
	if (IsRenderThread())
	{
		m_pRenderEngine->RT_OscillateCamera(time);
		return;
	}

	LOADINGCOMMAND_CRITICAL_SECTION;
	byte* p = AddCommand(eRC_OscillateCamera, sizeof(float));
	AddFloat(p, time);
}

void RenderThread::RC_MoveOgreHead(float offset)
{
	if (IsRenderThread())
	{
		m_pRenderEngine->RT_MoveOgreHead(offset);
		return;
	}

	LOADINGCOMMAND_CRITICAL_SECTION;
	byte* p = AddCommand(eRC_MoveOgreHead, sizeof(float));
	AddFloat(p, offset);
}

void RenderThread::RC_SceneNodeSetPosition(Ogre::SceneNode *n, const Ogre::Vector3 &pos)
{
	if (IsRenderThread())
	{
		m_pRenderEngine->RT_SceneNodeSetPosition(n, pos);
		return;
	}

	LOADINGCOMMAND_CRITICAL_SECTION;
	byte* p = AddCommand(eRC_SceneNodeSetPosition, sizeof(Ogre::SceneNode*) + sizeof(Ogre::Vector3));
	AddRaw(p, n);
	AddRaw(p, pos);
}

uint32_t RenderThread::RC_CreateSceneNode(const Ogre::String &meshName)
{
	LOADINGCOMMAND_CRITICAL_SECTION;

  static uint32_t index = 0; // TODO: redo to reusable value instead

  uint32_t len = meshName.size();
	byte* p = AddCommand(eRC_CreateSceneNode, sizeof(uint32_t) + sizeof(uint32_t) + len);
	AddRaw(p, index);
	AddRaw(p, len);
  AddBytes(p, (byte*)meshName.c_str(), len);
  return index++;
}


void RenderThread::RC_BeginFrame()
{

}

void RenderThread::RC_EndFrame()
{
	if (IsRenderThread())
		return;

	SyncMainWithRender();
}

void RenderThread::SyncMainWithRender()
{
	assert(!IsRenderThread());

	WaitForRenderThreadSignal();

	// Switch buffers
	{
		LOADINGCOMMAND_CRITICAL_SECTION;
		NextFrame();
		m_Commands[m_nFrameFill].shrink_to_fit();
	}

	SignalRenderThread();
}

// Wait signal from main thread
void RenderThread::WaitForMainThreadSignal()
{
	assert(IsRenderThread());

	while (!m_nFlush)
	{
	}
}

// Wait signal from render thread
void RenderThread::WaitForRenderThreadSignal()
{
	assert(!IsRenderThread());

	while (m_nFlush)
	{
	}
}

std::map<uint32_t, void*> &RenderThread::GetDeliveryQueue()
{
  return m_deliveryQueue[m_nFrameFill];
}

