 #ifndef _TS_MUXER_H
#define _TS_MUXER_H
#include "TinyTSPacket.h"
#include "TinyPATPacket.h"
#include "TinyPMTPacket.h"
#include "TinyPESPacket.h"
#include <map>
#include "TinyTSCommonDef.h"

class LIB_TINY_TS_API CTinyTSMuxer
{
public:
	enum
	{
		DEFAULT_TRANSPORT_STREAM_ID = 1,
		DEFAULT_PMT_PID = 0X100,
		DEFAULT_PES_AUDIO_PID = 0X101,
		DEFAULT_PES_VIDEO_PID = 0X102,
	};

	enum{
		TS_TYPE_PAT = 0x01000000,
		TS_TYPE_PMT = 0x02000000,
		TS_TYPE_PES = 0x03f00000,

		TS_TYPE_PES_AUDIO         = 0x03100000,
		TS_TYPE_PES_VIDEO_I_FRAME = 0x03200000,
		TS_TYPE_PES_VIDEO_P_FRAME = 0x03400000,
		TS_TYPE_PES_VIDEO_E_FRAME = 0x03800000,
	};

	enum{
		SIZE_OF_MAX_PES_BUFFER = 1024*1024*2,
		SIZE_OF_MAX_PAT_BUFFER = 1024,
		SIZE_OF_MAX_PMT_BUFFER = 1024,
	};

	CTinyTSMuxer(int nStreamID = DEFAULT_TRANSPORT_STREAM_ID);
	~CTinyTSMuxer(void);
	typedef int (*PacketsDeliverer_cb)(const unsigned char* data, const unsigned long dataLength,
		unsigned int uTSType, const u64 millSec, void*vptr);

	typedef int (*AfterTSPacket_cb)(const unsigned char* data, const unsigned long dataLength,
		unsigned int uTSType, const u64 millSec, void*vptr);

	void SetPacketsDeliverer(PacketsDeliverer_cb func, void* vptr)
	{
		m_devliverFunc = func;
		m_devliverFuncParam = vptr;		
	};

	void SetAfterPacket(AfterTSPacket_cb func, void* vptr)
	{
		m_afterTSPacketFunc  = func;
		m_afterTSPacketParam = vptr;
	};

	CTinyPMTPacket *NewProgram(int nPid, int nPcrPid);
	CTinyPESPacket *NewStream(int nStreamID, int nPid, u32 uStreamType);

	void AddProgram(CTinyPMTPacket* program);
	void AddStream(CTinyPMTPacket* program, CTinyPESPacket *pes);

	int Deliver(u32 uTSType, u8 *buff, int nBuffLen, CTinyTSPacket *ts, u8 adaption, u8 *data, 
		int dataLength, u64 pcr, u64 pts, u64 dts);
	int DeliverPAT();
	int DeliverPMT();
	int Mux(u32 uFrameType, CTinyPESPacket *pes, int pcrPid,
		u8 *data, int dataLength, u64 pcr, u64 pts, u64 dts);
	
private:
	PacketsDeliverer_cb m_devliverFunc;
	void*               m_devliverFuncParam;

	AfterTSPacket_cb    m_afterTSPacketFunc;
	void*				m_afterTSPacketParam;
	map<int, CTinyTSPacket *> m_tsPIDMap;

	CTinyPATPacket *m_pat;
	u8 *m_pesBuffer;
	u8 *m_pmtBuff;
	u8 *m_patBuff;
};

#endif