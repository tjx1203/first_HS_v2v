#include "bytertc_video.h"
#include "util/thread_loop.h"
#include <vector>
#include <memory>

struct StuVideoCaptureConfig;
class RTCVideoEngineWrapper:public bytertc::IRTCVideoEventHandler,
	public bytertc::IRTCRoomEventHandler
{
public:
	RTCVideoEngineWrapper();
	~RTCVideoEngineWrapper();
	int init();
	int joinRoom();
	int destory();
	static RTCVideoEngineWrapper *instance();
protected:
	int initAudioDevice();
	int initVideoDevice();
	int initAudioConfig();
	int initVideoConfig();
	void pushExternalVideoFrame();
	void pushExternalAudioFrame();
protected:
	//bytertc::IRTCVideoEventHandler
	void onConnectionStateChanged(bytertc::ConnectionState state) override;
	void onPerformanceAlarms(bytertc::PerformanceAlarmMode mode, const char* room_id,
		bytertc::PerformanceAlarmReason reason, const bytertc::SourceWantedData& data) override;
	void onRemoteAudioStateChanged(
		const bytertc::RemoteStreamKey& key, bytertc::RemoteAudioState state, bytertc::RemoteAudioStateChangeReason reason) override;

	void onAudioDeviceStateChanged(const char* device_id, bytertc::RTCAudioDeviceType device_type,
            bytertc::MediaDeviceState device_state, bytertc::MediaDeviceError device_error) override;

	void onFirstLocalVideoFrameCaptured(bytertc::StreamIndex index, bytertc::VideoFrameInfo info) override;
	void onLocalVideoSizeChanged(bytertc::StreamIndex index, const bytertc::VideoFrameInfo& info) override;
	void onRemoteVideoSizeChanged(bytertc::RemoteStreamKey key, const bytertc::VideoFrameInfo& info) override;
	void onFirstRemoteVideoFrameRendered(const bytertc::RemoteStreamKey key, const bytertc::VideoFrameInfo& info) override;
	void onFirstRemoteVideoFrameDecoded(const bytertc::RemoteStreamKey key, const bytertc::VideoFrameInfo& info) override;

protected:
		//bytertc::IRTCRoomEventHandler
	void onRoomStateChanged(const char* room_id, const char* uid, int state, const char* extra_info) override;
	void onWarning(int warn) override;
	void onError(int err) override ;
	void onLeaveRoom(const bytertc::RtcRoomStats& stats) override;
	void onRoomStats(const bytertc::RtcRoomStats& stats) override;
	void onUserJoined(const bytertc::UserInfo& userInfo, int elapsed) override;
	void onUserLeave(const char* uid, bytertc::UserOfflineReason reason);
	void onUserPublishStream(const char* uid, bytertc::MediaStreamType type)override;
	void onUserUnpublishStream(const char* uid, bytertc::MediaStreamType type,bytertc::StreamRemoveReason reason)override;
	void onUserPublishScreen(const char* uid, bytertc::MediaStreamType type);
	void onUserUnpublishScreen(const char* uid, bytertc::MediaStreamType type, bytertc::StreamRemoveReason reason);
	void onStreamRemove(const bytertc::MediaStreamInfo& bs, bytertc::StreamRemoveReason reason) override;
	void onStreamAdd(const bytertc::MediaStreamInfo& stream) override;
	void onStreamSubscribed(bytertc::SubscribeState stateCode, const char* stream_id, const bytertc::SubscribeConfig& info) override;
	void onStreamPublishSuccess(const char* user_id, bool is_screen) override;
	void onRoomMessageReceived(const char* uid, const char* message) override;
	void onRoomBinaryMessageReceived(const char* uid, int size, const uint8_t* message) override;
	void onUserMessageReceived(const char* uid, const char* message) override;
	void onUserBinaryMessageReceived(const char* uid, int size, const uint8_t* message) override;
	void onRoomMessageSendResult(int64_t msgid, int error) override;
	void onUserMessageSendResult(int64_t message_id, int error) override;
private:
	bytertc::IRTCVideo *m_pVideoEngine = nullptr;
	bytertc::IRTCRoom  *m_pRtcRoom = nullptr;
	//audio 
	std::vector<uint8_t> m_vecAudioPCMData;
	int				     m_nCurrentAudioFrameIndex;
	int					 m_nTotalAudioFrames;
	//video	
	std::vector<uint8_t> m_vecVideoYUVData;
	int				     m_nCurrentVideoFrameIndex;
	int					 m_nTotalVideoFrames;
	std::unique_ptr<StuVideoCaptureConfig> m_customCaptureConfig;
	std::unique_ptr<ThreadLoop>    m_threadLoop;
	std::mutex						m_exitMutex;
};
