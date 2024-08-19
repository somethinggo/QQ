#ifndef QQ_AUDIO_AUDIO_H
#define QQ_AUDIO_AUDIO_H

#include <qwidget.h>
#include <qpainter.h>
#include <qevent.h>
#include <qmath.h>
#include <qbuffer.h>
#include <qaudioformat.h>
#include <qfile.h>
#include <qjsonobject.h>
#include <qjsondocument.h>
#include <qapplication.h>
#include <complex>

#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
#include <qmediadevices.h>
#include <qaudiodevice.h>
#include <qaudiosource.h>
#include <qdatastream.h>
#else
#include <qaudiodeviceinfo.h>
#include <qaudioinput.h>
#endif

#include <ElaIcon.h>
#include <ElaMessageBar.h>
#include <fftw3.h>
/**
 * @brief Q_USE_FFMPEG 是否使用FFMPEG库--由于默认转为AMR格式,所以需要FFMPEG库并且支持AMR编码
 * @note 如果不使用FFMPEG库,则需要使用QProcess直接调用FFMPEG发布版本进行音频格式转换
 *  */
#ifdef Q_USE_FFMPEG
extern "C"
{
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
#include <libavutil/opt.h>
#include <libswresample/swresample.h>
}
#else
#include <qprocess.h>
#include <qtemporaryfile.h>
#endif

#include "qqglobal.h"
#include "qqfunction.h"

class QQAudio : public QWidget
{
	Q_OBJECT
	QQ_SINGLETON_CREATE(QQAudio)
public:
	explicit QQAudio(QWidget *parent = nullptr);
	virtual ~QQAudio();

protected:
	bool event(QEvent *event) override;
	void keyPressEvent(QKeyEvent *event) override;
	void keyReleaseEvent(QKeyEvent *event) override;
	void paintEvent(QPaintEvent *event) override;

private:
	void respondRequest(const QByteArray &data);
	void respondClose();
	/**
	 * @brief 从音频数据缓存中获取频域分析数据--频域分析,归一化,量化
	 */
	void frequencyDomainByFFTW();
	/**
	 * @brief 将PCM音频数据转换为AMR格式
	 * @param data PCM音频数据
	 * @return QByteArray AMR格式音频数据
	 */
	QByteArray getAuioPCMToAMR(const QByteArray &data);

	/**
	 *@brief 计算AMR音频数据的时长
	 *@param data AMR音频数据
	 *@return uint64_t  时长--单位秒
	 */
	uint64_t calculateAMRDuration(const QByteArray &data);
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
	/**
	 * @brief 获取最接近的音频格式
	 * @param device 音频设备
	 * @param format 音频格式
	 * @return QAudioFormat 最接近的音频格式
	 * @note Qt5提供了该API,但是Qt6没有提供
	 */
	QAudioFormat nearestFormat(const QAudioDevice &device, const QAudioFormat &format);
#endif
private slots:
	void do_audioBufferReady();

private:
	const int MAX_DATA_SIZE = 60; // 音频数据最大长度--60s
	const int MAX_UI_DATA_SIZE = 45;
	const int MAX_LINE_HEIGHT = 100;
	const int MAX_GRADIANT_COLOR = 10;
	QString m_senderID;
	QString m_receiverID;
	std::vector<double> m_inputCache;				 // 音频数据缓存,频域分析输入数据
	std::vector<std::complex<double>> m_outputCache; // 频域分析输出数据
	fftw_plan m_plan;
	std::vector<double> m_showData;
	QByteArray m_audioData;
	QAudioFormat m_format;
	QIODevice *m_audioDevice;
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
	QAudioDevice m_deviceInfo;
	QAudioSource *m_audioInput;
#else
	QAudioDeviceInfo m_deviceInfo;
	QAudioInput *m_audioInput;
#endif
};

#endif // QQ_AUDIO_VIEW_H
