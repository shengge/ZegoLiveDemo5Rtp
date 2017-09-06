#ifndef ZEGOMIXSTREAMANCHORDIALOG_H
#define ZEGOMIXSTREAMANCHORDIALOG_H

#include <QtWidgets/QDialog>
#include <QStack>
#include <QVector>
#include <QLayout>
#include <QStringListModel>
#include <QTime>
#include <QTimer>
#include <QMouseEvent>
#include <QKeyEvent>
#include <QPaintEvent>
#include <QTextCodec>
#include <QJsonDocument>
#include <QVariantMap>
#include <QJsonObject>
#include <QFileDialog>
#include "ui_ZegoLiveRoomDialog.h"
#include "ZegoSettingsModel.h"
#include "ZegoRoomModel.h"
#include "ZegoAVView.h"
#include "LiveRoom.h"
#include "LiveRoom-IM.h"
#include "LiveRoom-Publisher.h"
#include "LiveRoom-Player.h"
#include "LiveRoomDefines.h"
#include "LiveRoomDefines-IM.h"
#include "ZegoRoomMsgModel.h"
#include "ZegoLiveDemo.h"
#include "NoFocusFrameDelegate.h"
#include "ZegoShareDialog.h"

#define MAX_VIEW_COUNT 12

//���������귶Χ
#define pos_min_y  0
#define pos_max_y  40

#define m_FirstAnchor "first"
#define m_MixStreamID "mixStreamID"
#define m_HlsKey "Hls"
#define m_RtmpKey "rtmp"

class ZegoMixStreamAnchorDialog : public QDialog
{
	Q_OBJECT

public:
	ZegoMixStreamAnchorDialog(QWidget *parent = 0);
	ZegoMixStreamAnchorDialog(SettingsPtr curSettings, RoomPtr room, QString curUserID, QString curUserName, /*bool isAnchor, int curMode,*/ QDialog *lastDialog, QDialog *parent = 0);
	~ZegoMixStreamAnchorDialog();
	void initDialog();

	protected slots:
	void OnLoginRoom(int errorCode, const QString& roomId, QVector<StreamPtr> vStreamList);
	void OnStreamUpdated(const QString& roomId, QVector<StreamPtr> vStreamList, LIVEROOM::ZegoStreamUpdateType type);
	void OnPublishStateUpdate(int stateCode, const QString& streamId, StreamPtr streamInfo);
	void OnPlayStateUpdate(int stateCode, const QString& streamId);
	void OnUserUpdate(QVector<QString> userIDs, QVector<QString> userNames, QVector<int> userFlags, QVector<int> userRoles, unsigned int userCount, LIVEROOM::ZegoUserUpdateType type);
	void OnDisconnect(int errorCode, const QString& roomId);
	void OnKickOut(int reason, const QString& roomId);
	void OnPlayQualityUpdate(const QString& streamId, int quality, double videoFPS, double videoKBS);
	void OnPublishQualityUpdate(const QString& streamId, int quality, double videoFPS, double videoKBS);
	void OnAVAuxInput(unsigned char *pData, int* pDataLen, int pDataLenValue, int *pSampleRate, int *pNumChannels);
	void OnSendRoomMessage(int errorCode, const QString& roomID, int sendSeq, unsigned long long messageId);
	void OnRecvRoomMessage(const QString& roomId, QVector<RoomMsgPtr> vRoomMsgList);
	void OnJoinLiveRequest(int seq, const QString& fromUserId, const QString& fromUserName, const QString& roomId);
	void OnAudioDeviceChanged(AV::AudioDeviceType deviceType, const QString& strDeviceId, const QString& strDeviceName, AV::DeviceState state);
	void OnVideoDeviceChanged(const QString& strDeviceId, const QString& strDeviceName, AV::DeviceState state);
	void OnMixStream(unsigned int errorCode, const QString& hlsUrl, const QString& rtmpUrl, const QString& mixStreamID, int seq);

protected:
	virtual void mousePressEvent(QMouseEvent *event);
	virtual void mouseMoveEvent(QMouseEvent *event);
	virtual void mouseReleaseEvent(QMouseEvent *event);
	virtual void mouseDoubleClickEvent(QMouseEvent *event);
	virtual void closeEvent(QCloseEvent *e);
	virtual bool eventFilter(QObject *target, QEvent *event);
	
signals:
	//��ֱ�����ڹر�ʱ�������ĵ���Ƶ���ô��ظ�MainDialog���磬����������ͷ����˷磩
	void sigSaveVideoSettings(SettingsPtr settings);

private slots:
	void OnClickTitleButton();
	void OnClose();
	void OnButtonSwitchPublish();
	void OnButtonSendMessage();
	void OnButtonSoundCapture();
	void OnButtonMircoPhone();
	void OnButtonSound();
	void OnProgChange();
	void OnShareLink();
	void OnButtonAux();
	//�л�����Ƶ�豸
	void OnSwitchAudioDevice(int id);
	void OnSwitchVideoDevice(int id);
private:
	void insertStringListModelItem(QStringListModel * model, QString name, int size);
	void removeStringListModelItem(QStringListModel * model, QString name);
	void EnumVideoAndAudioDevice();
	void initComboBox();
	void StartPublishStream();
	void StopPublishStream(const QString& streamID);
	void FreeAVView(StreamPtr stream);
	void StartPlayStream(StreamPtr stream);
	void StopPlayStream(const QString& streamID);
	void SetOperation(bool state);
	void GetOut();
	void BeginAux();
	void EndAux();
	QString encodeStringAddingEscape(QString str);

	//������Ҫ���õĺ���
	void StartMixStream();
	void MixStreamAdd(QVector<StreamPtr> vStreamList, const QString& roomId);
	void MixStreamDelete(QVector<StreamPtr> vStreamList, const QString& roomId);
	bool isStreamExisted(QString streamID);
	

	//��Ա�б���ɾ����
	void roomMemberAdd(QString userName);
	void roomMemberDelete(QString userName);

	int takeLeastAvaliableViewIndex();
private:
	Ui::ZegoLiveRoomDialog ui;

	QVector<unsigned int> m_avaliableView;
	bool m_bCKEnableMic;
	bool m_bCKEnableSpeaker;
	SettingsPtr m_pAVSettings;
	RoomPtr m_pChatRoom;
	//��������
	unsigned char* m_pAuxData;
	int m_nAuxDataLen;
	int m_nAuxDataPos;

	bool m_bSystemCapture = false;
	bool m_bIsPublishing = false;
	bool isMax = false;

	QString m_strPublishStreamID;
	QString m_strCurUserID;
	QString m_strCurUserName;

	QVector<QString> m_vecAudioDeviceIDs;
	QVector<QString> m_vecVideoDeviceIDs;

	QVector<QZegoAVView *> AVViews;


	QString m_strLastSendMsg;

	//List
	QStringList m_MircoPhoneList;
	QStringList m_CameraList;

	//�Զ����ComboBox����ʽҳ��
	QListView *m_cbMircoPhoneListView;
	QListView *m_cbCameraListView;

	//Model
	QStringListModel *m_cbMircoPhoneModel;
	QStringListModel *m_cbCameraModel;
	QStringListModel *m_chatModel;
	QStringListModel *m_memberModel;

	//ʵ���Զ�����������϶�
	bool isMousePressed;
	QPoint mousePosition;

	//ʵʱ�����˷�������С
	QTimer *timer;

	//������һ�������ָ�룬�����˳���ҳ��ʱ��ʾ��
	QDialog *m_lastDialog;

	//�������������Ϣ������
	QVector<ZEGO::AV::ZegoMixStreamConfig *> m_mixStreamInfos;

	//����ģʽ�����ڱ�����StreamPtr������
	QVector<StreamPtr> m_mixStreamList;

	//�����ص����
	int m_mixStreamRequestSeq;

	//����ʱ�Ļ���ID
	QString m_myMixStreamID;
	//����ʱ��һ�����Ļ���ID
	QString m_anchorMixStreamID;

	//������������ExtraInfo��Ϣ
	QString sharedHlsUrl;
	QString sharedRtmpUrl;

	//������ǰ��һ����������Ϣ
	StreamPtr m_anchorStreamInfo;

};

#endif