#ifndef QQ_COMMON_FUNCTION_H
#define QQ_COMMON_FUNCTION_H

#include <qwidget.h>
#include <qpainter.h>
#include <qpainterpath.h>
#include <qbitmap.h>
#include <qbuffer.h>
#include <qimagereader.h>
#include <qimagewriter.h>
#include <qrandom.h>
#include <qjsonobject.h>
#include <qjsondocument.h>
#include <qjsonarray.h>
#include <functional>

#include "qqconfig.h"
#include "qqenums.h"

namespace QQFunctions
{
	void initSystem();
	void exitSystem();

	/**
	 * @brief 获取用户信息
	 * @param ID 用户ID
	 * @return 用户信息
	 */
	QQConfigs::FriendConfig *getUserInfo(const QString &ID);
	/**
	 * @brief 获取群信息
	 * @param ID 群ID
	 * @return 群信息
	 */
	QQConfigs::GroupConfig *getGroupInfo(const QString &ID);

	/**
	 * @brief 获取圆角图片
	 * @param src 源图片
	 * @param radius 圆角半径
	 * @return 圆角图片
	 */
	QPixmap getRoundedPixmap(const QPixmap &src, int radius);

	/**
	 * @brief 修改文本适应区域(基于宽度进行适应)
	 * @param text 文本
	 * @param font 字体
	 * @return 适应文本
	 */
	QString getCalculateText(const QString &text, const QFont &font, quint64 MAXW = UINT64_MAX, bool elided = false);

	/**
	 * @brief 获取文本的绘制矩形
	 * @param text 文本
	 * @param font 字体
	 * @param MAXW 最大宽度
	 * @param elided 是否省略
	 * @return 绘制矩形
	 */
	QRect getCalculateTextRect(const QString &text, const QFont &font);

	/**
	 * @brief 获取圆角蒙版
	 * @param size 尺寸
	 * @param radius 圆角半径
	 * @param types 圆角位置
	 * @return 圆角蒙版
	 */
	QBitmap getRoundedMask(const QSize &size, qreal radius, QQEnums::RoundPostionTypes types = QQEnums::RoundPostionType::all);

	/**
	 * @brief 获取旋转图片
	 * @param src 源图片
	 * @param angle 旋转角度
	 * @return 旋转图片
	 */
	QPixmap getRotatePixmap(const QPixmap &src, qreal angle);

	/**
	 * @brief 获取文件格式大小
	 * @param bytes 字节
	 * @return 文件格式大小
	 * @note 返回值.first为大小，.second为单位
	 */
	QPair<qreal, QString> getFileFormatSize(qint64 bytes);

	/**
	 * @brief 获取图片转为Base64编码
	 * @param image 图片
	 * @return 编码与图片类型
	 */
	QPair<QString, QString> getImageToBase64(const QImage &image);

	/**
	 * @brief 获取图片转为Base64编码
	 * @param fileName 文件名
	 * @return 编码与图片类型
	 */
	QPair<QString, QString> getImageToBase64(const QString &fileName);

	/**
	 * @brief 获取Base64编码转为图片
	 * @param base64 编码
	 * @return 图片
	 */
	QImage getBase64ToImage(const QString &base64);

	/**
	 * @brief 生成一段随机文本
	 * @param min 最小长度
	 * @param max 最大长度
	 * @return 随机文本
	 */
	QString getGenerateRandomText(int min, int max);

	/**
	 * @brief 获取鼠标是否在控件内
	 * @param widget 控件
	 * @param pos 位置
	 * @return 是否在控件内
	 */
	bool getMouseIsInWidget(QWidget *widget, QPoint pos = QPoint());

}

#endif