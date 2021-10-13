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
		UINT32 nCommandType = *((UINT32*)ptr);

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
		case eRC_SetupDefaultLight:
		{
			m_pRenderEngine->RT_SetupDefaultLight();
			break;
		}
		case eRC_CreateSceneNode:
		{
			RenderNode* pRenderNode = ReadCommand<RenderNode*>(n);

			m_pRenderEngine->RT_CreateSceneNode(pRenderNode);
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

byte* RenderThread::AddCommand(RenderCommand eRC, size_t nParamBytes)
{
	UINT32 cmdSize = sizeof(RenderCommand) + nParamBytes;
	byte* storage = new byte[m_Commands[m_nFrameFill].capacity()];

	memcpy(storage, m_Commands[m_nFrameFill].data(), m_Commands[m_nFrameFill].capacity());
	m_Commands[m_nFrameFill].reserve(m_Commands[m_nFrameFill].capacity() * sizeof(byte) + cmdSize);
	memcpy(m_Commands[m_nFrameFill].data(), storage, m_Commands[m_nFrameFill].capacity() - cmdSize);

	byte* ptr = m_Commands[m_nFrameFill].data() + m_Commands[m_nFrameFill].capacity() * sizeof(byte) - cmdSize;
	AddRawData(ptr, eRC);
	delete[] storage;
	return ptr;
}

template<typename T>
void RenderThread::AddRawData(byte*& ptr, const T Val)
{
	*(T*)ptr = Val;
	ptr += sizeof(T);
}

void RenderThread::AddBytes(byte*& ptr, byte* copy, uint32_t sz)
{
	memcpy(ptr, copy, sz);
	ptr += sz;
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

void RenderThread::RC_CreateSceneNode(RenderNode* pRenderNode)
{
	LOADINGCOMMAND_CRITICAL_SECTION;

	if (IsRenderThread())
	{
		m_pRenderEngine->RT_CreateSceneNode(pRenderNode);
		return;
	}

	byte* p = AddCommand(eRC_CreateSceneNode, sizeof(RenderNode));
	AddRawData(p, pRenderNode);
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

