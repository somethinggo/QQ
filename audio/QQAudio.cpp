#include "QQAudio.h"

QQAudio::QQAudio(QWidget *parent)
	: QWidget(parent)
{
	m_inputCache.resize(2 * MAX_UI_DATA_SIZE, 0);
	m_outputCache.resize(MAX_UI_DATA_SIZE + 1, 0);
	m_plan = fftw_plan_dft_r2c_1d(m_inputCache.size(), m_inputCache.data(), reinterpret_cast<fftw_complex *>(m_outputCache.data()), FFTW_ESTIMATE);
	m_showData.resize(MAX_UI_DATA_SIZE, 0);

#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
	m_deviceInfo = QMediaDevices::defaultAudioInput();
	m_format.setSampleFormat(QAudioFormat::Int16); // 默认使用16位bit深度
	if (!m_deviceInfo.isFormatSupported(m_format))
	{
		m_format = nearestFormat(m_deviceInfo, m_format);
	}
	m_audioInput = new QAudioSource(m_deviceInfo, m_format, this);
	m_audioInput->setBufferSize(10 * 1024);
#else
	m_deviceInfo = QAudioDeviceInfo::defaultInputDevice();
	m_format.setSampleRate(44100);					   // 默认使用44100采样率
	m_format.setChannelCount(1);					   // 默认使用单声道
	m_format.setSampleSize(16);						   // 默认使用16位bit深度
	m_format.setByteOrder(QAudioFormat::LittleEndian); // 默认使用小端字节序
	m_format.setSampleType(QAudioFormat::SignedInt);   // 默认使用有符号整型
	if (!m_deviceInfo.isFormatSupported(m_format))
	{
		m_format = m_deviceInfo.nearestFormat(m_format);
	}
	m_audioInput = new QAudioInput(m_deviceInfo, m_format, this);
	m_audioInput->setBufferSize(10 * 1024);
	m_audioInput->setNotifyInterval(60);
	connect(m_audioInput, &QAudioInput::notify, this, &QQAudio::do_audioBufferReady);
#endif
}

QQAudio::~QQAudio()
{
	fftw_destroy_plan(m_plan);
}

void QQAudio::keyPressEvent(QKeyEvent *event)
{
	if (event->key() == Qt::Key_Space)
	{
		if (!event->isAutoRepeat() && m_audioInput->state() == QAudio::StoppedState)
		{
			m_audioData.clear();
			m_audioDevice = m_audioInput->start();
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
			connect(m_audioDevice, &QIODevice::readyRead, this, &QQAudio::do_audioBufferReady);
#endif
		}
	}
	return QWidget::keyPressEvent(event);
}

void QQAudio::keyReleaseEvent(QKeyEvent *event)
{
	if (event->key() == Qt::Key_Space)
	{
		if (!event->isAutoRepeat() && m_audioInput->state() == QAudio::ActiveState)
		{
			m_audioInput->stop();
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
			disconnect(m_audioDevice, &QIODevice::readyRead, this, &QQAudio::do_audioBufferReady);
#endif
			std::fill(m_showData.begin(), m_showData.end(), 0);
			this->update();
			QByteArray amrData = getAuioPCMToAMR(m_audioData);
			uint64_t duration = calculateAMRDuration(amrData);
			if (duration < m_minTime || duration > m_maxTime)
			{
				ElaMessageBar::warning(ElaMessageBarType::Bottom, QString(), QString::fromLocal8Bit("语音时长超过限制"), 3000);
			}
			else if (!amrData.isEmpty())
			{
				Q_MSEND_EVENT(QQEnums::sendaudio, amrData);
			}
			m_audioData.clear();
		}
	}
	else if (event->key() == Qt::Key_Escape)
	{
		this->close();
	}
	return QWidget::keyReleaseEvent(event);
}

void QQAudio::paintEvent(QPaintEvent *event)
{
	QPainter painter(this);
	painter.setPen(Qt::NoPen);
	painter.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
	painter.fillRect(this->rect(), QColor("#F2F2F2"));
	int radius = 50;
	QRect ellipseRect = QRect(this->width() / 2 - radius, this->height() / 2 - radius, 2 * radius, 2 * radius);
	// 绘制频域分析数据
	double step = 2 * M_PI * radius / m_showData.size();
	double angle = 360.0 / m_showData.size();
	QPoint basePoint = QPoint(ellipseRect.x() + radius, ellipseRect.y() + radius);
	// 绘制基础频域数据指示器

	painter.save();
	QTransform transform;
	transform.translate(basePoint.x(), basePoint.y());
	painter.setTransform(transform);
	for (int i = 0; i < m_showData.size(); i++)
	{
		double value = m_showData.at(i);
		QPixmap baseIndicatorPixmap(step, radius + value + step / 2 - 5);
		baseIndicatorPixmap.fill(Qt::transparent);
		QPainter baseIndicatorPixmapPainter(&baseIndicatorPixmap);
		baseIndicatorPixmapPainter.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
		baseIndicatorPixmapPainter.setPen(Qt::NoPen);
		baseIndicatorPixmapPainter.setBrush(Qt::blue);
		QPainterPath path;
		path.moveTo(0, 0);
		path.lineTo(0, baseIndicatorPixmap.height() - step / 2);
		path.arcTo(QRectF(0, baseIndicatorPixmap.height() - step, step, step), 180, 180);
		path.lineTo(step, 0);
		path.closeSubpath();
		baseIndicatorPixmapPainter.fillPath(path, Qt::blue);

		transform.rotate(angle);
		painter.setTransform(transform);
		painter.drawPixmap(QPoint(-step / 2, 0), baseIndicatorPixmap);
	}
	painter.restore();

	// 绘制中心圆
	painter.setBrush(QColor("#00A5FF"));
	painter.drawEllipse(ellipseRect);
	// 绘制中心圆内部的图标
	QPixmap pixmap = ElaIcon::getInstance()->getElaIcon(ElaIconType::Microphone, radius, Qt::white).pixmap(radius, radius);
	painter.drawPixmap(ellipseRect.x() + ellipseRect.width() / 2 - pixmap.size().width() / 2, ellipseRect.y() + ellipseRect.height() / 2 - pixmap.size().height() / 2, pixmap);
	if (m_audioInput->state() == QAudio::StoppedState)
	{
		painter.setPen(Qt::black);
		painter.setFont(QFont("Microsoft YaHei", 10));
		QString text = QString::fromLocal8Bit("按住空格键说话,松开发送,按下Esc键退出");
		QRect rect = QQFunctions::getCalculateTextRects(text, painter.font()).first;
		rect.moveTopLeft(QPoint(this->width() / 2 - rect.width() / 2, this->height() / 2 + radius + 10));
		painter.drawText(rect, Qt::AlignCenter, text);
	}
}

void QQAudio::frequencyDomainByFFTW()
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

QByteArray QQAudio::getAuioPCMToAMR(const QByteArray &data)
{
#ifdef Q_USE_FFMPEG
	AVCodec *codec = const_cast<AVCodec *>(avcodec_find_encoder(AV_CODEC_ID_AMR_NB)); // 查找amr编码器
	if (codec == nullptr)
	{
		qWarning() << "Can't find amr codec";
		return QByteArray();
	}
	AVCodecContext *codecContext = avcodec_alloc_context3(codec);
	if (codecContext == nullptr)
	{
		qWarning() << "Can't alloc amr codec context";
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
		qWarning() << "Can't open amr codec";
		avcodec_free_context(&codecContext);
		return QByteArray();
	}

	AVIOContext *ioContext = nullptr;
	QByteArray amrBuffer;
	uint8_t *buffer = nullptr;
	size_t bufferSize = 0;

	if (avio_open_dyn_buf(&ioContext) < 0)
	{
		qWarning() << "Can't open dynamic buffer";
		avcodec_free_context(&codecContext);
		return QByteArray();
	}

	AVFormatContext *formatContext = avformat_alloc_context();
	if (formatContext == nullptr)
	{
		qWarning() << "Can't alloc format context";
		avio_close_dyn_buf(ioContext, &buffer);
		avcodec_free_context(&codecContext);
		return QByteArray();
	}

	// 创建音频流
	AVStream *stream = avformat_new_stream(formatContext, codec);
	if (stream == nullptr)
	{
		qWarning() << "Can't create audio stream";
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
		qWarning() << "Can't write header";
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
		qWarning() << "Can't alloc frame";
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
#else
	QTemporaryFile tempFile;
	tempFile.setAutoRemove(true);
	if (!tempFile.open())
	{
		qWarning() << "Can't open temp file";
		return QByteArray();
	}
	tempFile.close();
	QProcess process;
	process.setStandardOutputFile(tempFile.fileName());
	QStringList arguments;
	arguments << "-f" << QString("s%1le").arg(8 * m_format.bytesPerFrame())
			  << "-ar" << QString::number(m_format.sampleRate())
			  << "-ac" << QString::number(m_format.channelCount())
			  << "-i" << "-"
			  << "-ar" << "8000"
			  << "-ac" << QString::number(m_format.channelCount())
			  << "-f" << "amr" << "-";
	process.start("ffmpeg", arguments);
	if (!process.waitForStarted())
	{
		qWarning() << "FFmpeg process failed to start";
		return QByteArray();
	}
	process.write(data);
	process.closeWriteChannel();
	if (!process.waitForFinished())
	{
		qWarning() << "FFmpeg process failed to finish";
		return QByteArray();
	}
	if (!tempFile.open())
	{
		qWarning() << "Can't open temp file";
		return QByteArray();
	}
	QByteArray amrData = tempFile.readAll();
	tempFile.close();
	return amrData;
#endif
}

uint64_t QQAudio::calculateAMRDuration(const QByteArray &data)
{
	const int amrNBFrameSizes[] = {13, 14, 16, 18, 20, 21, 27, 32};
	const QByteArray header = "#!AMR\n";
	if (!data.startsWith(header))
	{
		qWarning() << "Invalid AMR file";
		return -1.0;
	}
	int offset = header.size(); // 去除头部
	int total = 0;
	while (offset < data.size())
	{
		unsigned char frameHeader = static_cast<unsigned char>(data[offset]);
		int frameType = (frameHeader >> 3) & 0x0F;
		if (frameType >= 0 && frameType < 8)
		{
			int frameSize = amrNBFrameSizes[frameType];
			offset += frameSize;
			total++;
		}
		else
		{
			qWarning() << "Unknown AMR frame type";
			return -1.0;
		}
	}
	return 20 * total / 1000;
}

#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
QAudioFormat QQAudio::nearestFormat(const QAudioDevice &device, const QAudioFormat &format)
{
	QAudioFormat nearestFormat;
	for (auto sampleFormat : device.supportedSampleFormats())
	{
		if (sampleFormat == QAudioFormat::Unknown)
		{
			continue;
		}
		else if (sampleFormat == QAudioFormat::Float)
		{
			nearestFormat.setSampleFormat(QAudioFormat::Float);
		}
		else if (sampleFormat == QAudioFormat::Int32)
		{
			nearestFormat.setSampleFormat(QAudioFormat::Int32);
		}
		else if (sampleFormat == QAudioFormat::Int16)
		{
			nearestFormat.setSampleFormat(QAudioFormat::Int16);
		}
		else if (sampleFormat == QAudioFormat::UInt8)
		{
			nearestFormat.setSampleFormat(QAudioFormat::UInt8);
		}
	}
	return nearestFormat;
}
#endif

void QQAudio::do_audioBufferReady()
{
	QByteArray data = m_audioDevice->readAll();
	m_audioData.append(data);
	std::fill(m_inputCache.begin(), m_inputCache.end(), 0);
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
	switch (m_format.sampleFormat())
	{
	case QAudioFormat::UInt8:
	{
		int sampleCount = data.size();
		for (int i = 0; i < m_inputCache.size(); ++i)
		{
			if (i >= sampleCount)
			{
				frequencyDomainByFFTW();
				return;
			}
			uint8_t sample = static_cast<uint8_t>(data[i]);
			m_inputCache[i] = sample;
		}
		break;
	}
	case QAudioFormat::Int16:
	{
		int sampleCount = data.size() / sizeof(int16_t);
		for (int i = 0; i < m_inputCache.size(); ++i)
		{
			if (i >= sampleCount)
			{
				frequencyDomainByFFTW();
				return;
			}
			int16_t sample = *reinterpret_cast<const int16_t *>(data.data() + 2 * i);
			m_inputCache[i] = sample;
		}
		break;
	}
	case QAudioFormat::Int32:
	{
		int sampleCount = data.size() / sizeof(int32_t);
		for (int i = 0; i < m_inputCache.size(); ++i)
		{
			if (i >= sampleCount)
			{
				frequencyDomainByFFTW();
				return;
			}
			int32_t sample = *reinterpret_cast<const int32_t *>(data.data() + 4 * i);
			m_inputCache[i] = sample;
		}
		break;
	}
	case QAudioFormat::Float:
	{
		int sampleCount = data.size() / sizeof(float);
		for (int i = 0; i < m_inputCache.size(); ++i)
		{
			if (i >= sampleCount)
			{
				frequencyDomainByFFTW();
				return;
			}
			float sample = *reinterpret_cast<const float *>(data.data() + 4 * i);
			m_inputCache[i] = sample;
		}
		break;
	}
	default:
		return;
	}
#else
	if (m_format.sampleType() != QAudioFormat::SignedInt)
	{
		return;
	}
	switch (m_format.sampleSize())
	{
	case 8:
	{
		int sampleCount = data.size();
		for (int i = 0; i < m_inputCache.size(); ++i)
		{
			if (i >= sampleCount)
			{
				frequencyDomainByFFTW();
				return;
			}
			int8_t sample = static_cast<int8_t>(data[i]);
			m_inputCache[i] = sample;
		}
		break;
	}
	case 16:
	{
		int sampleCount = data.size() / sizeof(int16_t);
		for (int i = 0; i < m_inputCache.size(); ++i)
		{
			if (i >= sampleCount)
			{
				frequencyDomainByFFTW();
				return;
			}
			int16_t sample = *reinterpret_cast<const int16_t *>(data.data() + 2 * i);
			m_inputCache[i] = sample;
		}
		break;
	}
	case 32:
	{
		int sampleCount = data.size() / sizeof(int32_t);
		for (int i = 0; i < m_inputCache.size(); ++i)
		{
			if (i >= sampleCount)
			{
				frequencyDomainByFFTW();
				return;
			}
			int32_t sample = *reinterpret_cast<const int32_t *>(data.data() + 4 * i);
			m_inputCache[i] = sample;
		}
		break;
	}

	default:
		break;
	}
#endif
	frequencyDomainByFFTW();
}