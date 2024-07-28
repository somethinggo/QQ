#include "QQChatAudioView.h"

QQChatAudioView::QQChatAudioView(QWidget *parent)
	: QDialog(parent)
{
	m_inputCache.resize(2 * MAX_UI_DATA_SIZE, 0);
	m_outputCache.resize(MAX_UI_DATA_SIZE + 1, 0);
	m_plan = fftw_plan_dft_r2c_1d(m_inputCache.size(), m_inputCache.data(), reinterpret_cast<fftw_complex *>(m_outputCache.data()), FFTW_ESTIMATE);
	m_showData.resize(MAX_UI_DATA_SIZE, 0);

	m_format.setSampleRate(44100);
	m_format.setChannelCount(1);
	m_format.setSampleSize(16);
	m_format.setCodec("audio/pcm");
	m_format.setByteOrder(QAudioFormat::LittleEndian);
	m_format.setSampleType(QAudioFormat::SignedInt);

	m_deviceInfo = QAudioDeviceInfo::defaultInputDevice();
	if (!m_deviceInfo.isFormatSupported(m_format))
	{
		m_format = m_deviceInfo.nearestFormat(m_format);
	}

	m_audioInput = new QAudioInput(m_deviceInfo, m_format, this);

	connect(m_audioDevice, &QIODevice::readyRead, this, &QQChatAudioView::do_audioBufferReady);
}

QQChatAudioView::~QQChatAudioView()
{
	fftw_destroy_plan(m_plan);
}

void QQChatAudioView::keyPressEvent(QKeyEvent *event)
{
	if (event->key() == Qt::Key_Escape)
	{
		m_audioDevice = m_audioInput->start();
	}
	return QDialog::keyPressEvent(event);
}

void QQChatAudioView::keyReleaseEvent(QKeyEvent *event)
{
	if (event->key() == Qt::Key_Escape)
	{
		m_audioInput->stop();
		QByteArray amrData = getAuioPCMToAMR(m_audioData);
		if (!amrData.isEmpty())
		{
			QFile file("audio.amr");
			if (file.open(QIODevice::WriteOnly))
			{
				file.write(amrData);
				file.close();
			}
		}
	}
	return QDialog::keyReleaseEvent(event);
}

void QQChatAudioView::paintEvent(QPaintEvent *event)
{
	QPainter painter(this);
	painter.setPen(Qt::NoPen);
	painter.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
	painter.fillRect(this->rect(), QColor("#F2F2F2"));
	int radius = 50;
	painter.setBrush(QColor("#00A5FF"));
	QRect ellipseRect = QRect(this->width() / 2 - radius, this->height() / 2 - radius, 2 * radius, 2 * radius);
	// 绘制频域分析数据
	double step = 2 * M_PI * radius / m_showData.size();
	double angle = 360.0 / m_showData.size();
	QPoint basePoint = QPoint(ellipseRect.x() + radius, ellipseRect.y() + radius);
	for (int i = 0; i < m_showData.size(); i++)
	{
		double value = m_showData.at(i);
		QPoint start = QPoint(basePoint.x() + radius * sin(i * angle), basePoint.y() - radius * cos(i * angle));
		QPoint end = QPoint(basePoint.x() + (radius + value) * sin(i * angle), basePoint.y() - (radius + value) * cos(i * angle));
		QLinearGradient linearGradient(0, 0, width(), 0);
		linearGradient.setColorAt(0.5, Qt::black);
		linearGradient.setColorAt(1, Qt::green);
		painter.setPen(QPen(linearGradient, step));
		painter.drawLine(start, end);
	}
	// 绘制中心圆
	painter.drawEllipse(ellipseRect);
	// 绘制中心圆内部的图标
	QPixmap pixmap = ElaIcon::getInstance()->getElaIcon(ElaIconType::Microphone, radius, Qt::white).pixmap(radius, radius);
	painter.drawPixmap(ellipseRect.x() + ellipseRect.width() / 2 - pixmap.size().width() / 2, ellipseRect.y() + ellipseRect.height() / 2 - pixmap.size().height() / 2, pixmap);
}

void QQChatAudioView::frequencyDomainByFFTW()
{
	fftw_execute(m_plan);
	double max = 0.0;
	for (const auto &complexValue : m_outputCache)
	{
		max = std::max(max, std::abs(complexValue));
	}
	if (max > 0)
	{
		for (size_t i = 0; i < m_showData.size(); ++i)
		{
			m_showData[i] = std::abs(m_outputCache[i]) / max * MAX_LINE_HEIGHT;
		}
	}
	else
	{
		std::fill(m_showData.begin(), m_showData.end(), 0);
	}
	this->update();
}

QByteArray QQChatAudioView::getAuioPCMToAMR(const QByteArray &data)
{
	AVCodec *codec = const_cast<AVCodec *>(avcodec_find_encoder(AV_CODEC_ID_AMR_NB)); // 查找amr编码器
	if (codec == nullptr)
	{
		return QByteArray();
	}
	AVCodecContext *codecContext = avcodec_alloc_context3(codec);
	if (codecContext == nullptr)
	{
		return QByteArray();
	}
	// arm_nb的编码器参数设置
	codecContext->bit_rate = 12200;
	codecContext->sample_fmt = AV_SAMPLE_FMT_S16;
	codecContext->sample_rate = 8000;
	codecContext->channel_layout = AV_CH_LAYOUT_MONO;
	codecContext->channels = av_get_channel_layout_nb_channels(codecContext->channel_layout);

	if (avcodec_open2(codecContext, codec, nullptr) < 0)
	{
		avcodec_free_context(&codecContext);
		return QByteArray();
	}

	AVIOContext *ioContext = nullptr;
	QByteArray amrBuffer;
	uint8_t *buffer = nullptr;
	size_t bufferSize = 0;

	if (avio_open_dyn_buf(&ioContext) < 0)
	{
		avcodec_free_context(&codecContext);
		return QByteArray();
	}

	AVFormatContext *formatContext = avformat_alloc_context();
	if (formatContext == nullptr)
	{
		avio_close_dyn_buf(ioContext, &buffer);
		avcodec_free_context(&codecContext);
		return QByteArray();
	}

	// 创建音频流
	AVStream *stream = avformat_new_stream(formatContext, codec);
	if (stream == nullptr)
	{
		avio_close_dyn_buf(ioContext, &buffer);
		avcodec_free_context(&codecContext);
		avformat_free_context(formatContext);
		return QByteArray();
	}

	// 设置音频流信息
	stream->codecpar->codec_id = codec->id;
	stream->codecpar->codec_type = AVMEDIA_TYPE_AUDIO;
	stream->codecpar->bit_rate = codecContext->bit_rate;
	stream->codecpar->sample_rate = codecContext->sample_rate;
	stream->codecpar->channels = codecContext->channels;
	stream->codecpar->channel_layout = codecContext->channel_layout;
	stream->codecpar->format = codecContext->sample_fmt;

	// 写入头部信息
	if (avformat_write_header(formatContext, nullptr) < 0)
	{
		avio_close_dyn_buf(ioContext, &buffer);
		avcodec_free_context(&codecContext);
		avformat_free_context(formatContext);
		return QByteArray();
	}

	AVPacket pkt;
	av_init_packet(&pkt);
	pkt.data = nullptr;
	pkt.size = 0;

	AVFrame *frame = av_frame_alloc();
	if (frame == nullptr)
	{
		avio_close_dyn_buf(ioContext, &buffer);
		avcodec_free_context(&codecContext);
		avformat_free_context(formatContext);
		return QByteArray();
	}

	frame->nb_samples = codecContext->frame_size;
	frame->format = codecContext->sample_fmt;
	frame->channel_layout = codecContext->channel_layout;
	frame->sample_rate = codecContext->sample_rate;

	int bufferSizeFrame = av_samples_get_buffer_size(nullptr, codecContext->channels,
													 codecContext->frame_size, codecContext->sample_fmt, 0);
	uint8_t *frameBuffer = (uint8_t *)av_malloc(bufferSizeFrame);

	avcodec_fill_audio_frame(frame, codecContext->channels, codecContext->sample_fmt,
							 frameBuffer, bufferSizeFrame, 0);

	const uint8_t *ptr = (const uint8_t *)data.data();
	int offset = 0;
	int dataSize = data.size();

	while (offset < dataSize)
	{
		int copySize = std::min(bufferSizeFrame, dataSize - offset);
		memcpy(frameBuffer, ptr + offset, copySize);
		offset += copySize;
		int ret = avcodec_send_frame(codecContext, frame);
		if (ret < 0)
		{
			break;
		}
		while (ret >= 0)
		{
			ret = avcodec_receive_packet(codecContext, &pkt);
			if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF)
			{
				break;
			}
			else if (ret < 0)
			{
				return QByteArray();
			}
			av_interleaved_write_frame(formatContext, &pkt);
			av_packet_unref(&pkt);
		}
	}

	// 写入文件尾
	av_write_trailer(formatContext);

	// 将数据从AVIOContext中读取出来
	avio_close_dyn_buf(ioContext, &buffer);
	amrBuffer = QByteArray(reinterpret_cast<const char *>(buffer), bufferSize);

	av_free(buffer);
	av_frame_free(&frame);
	av_free(frameBuffer);
	avcodec_free_context(&codecContext);
	avformat_free_context(formatContext);

	return amrBuffer;
}

void QQChatAudioView::do_audioBufferReady()
{
	QByteArray data = m_audioDevice->readAll();
	m_audioData.append(data);
	std::fill(m_inputCache.begin(), m_inputCache.end(), 0);
	int sampleCount = data.size() / sizeof(int16_t);
	for (int i = 0; i < m_inputCache.size(); ++i)
	{
		if (i >= sampleCount)
		{
			frequencyDomainByFFTW();
			return;
		}
		int16_t sample = static_cast<int16_t>((data[2 * i + 1] << 8) | (data[2 * i] & 0xFF));
		m_inputCache[i] = sample;
	}
	frequencyDomainByFFTW();
}