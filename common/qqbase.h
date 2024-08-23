#ifndef QQ_COMMON_BASE_H
#define QQ_COMMON_BASE_H

#include <string>
#include <vector>
#include <chrono>
#include <optional>

namespace QQBaseConstValues
{
	// 用户限制
	uint64_t const MAX_FRIEND_COUNT = 1000;		  // 好友数量上限
	uint64_t const MAX_GROUP_COUNT = 1000;		  // 群聊数量上限
	uint64_t const MAX_GROUP_MEMBER_COUNT = 1000; // 群聊成员数量上限
	uint64_t const MAX_MSG_COUNT = 1000;		  // 单个聊天消息数量上限
	uint64_t const MAX_SKIN_COUNT = 1000;		  // 皮肤数量上限

	// 消息限制
	uint64_t const MAX_MSG_LENGTH = 100; // 消息长度上限--字数
	uint64_t const MAX_FILE_SIZE = 512;	 // 文件大小上限--MB
	uint64_t const MAX_IMAGE_SIZE = 16;	 // 图片大小上限--MB--不高于16MB转为base64,高于16MB转为文件
	uint64_t const MAX_VIDEO_SIZE = 64;	 // 视频大小上限--MB--不高于64MB转为base64,高于64MB转为文件
	uint64_t const MAX_AUDIO_SIZE = 16;	 // 音频大小上限--MB--不高于16MB转为base64,高于16MB转为文件
	uint64_t const MIN_AUDIO_TIME = 3;	 // 音频时长下限--秒
	uint64_t const MAX_AUDIO_TIME = 60;	 // 音频时长上限--秒
	uint64_t const MAX_BLOCK_SIZE = 16;	 // 分块大小上限--MB
	uint64_t const MAX_LINK_SIZE = 256;	 // 链接长度上限--字数
}

namespace QQBaseConfigs
{
	// 基础用户配置
	typedef struct UserBaseConfig
	{
		enum class UserStateType : char
		{
			online,
			offline,
			busy,
			leave,
			hidden,
			distrup
		};
		UserStateType m_state = UserStateType::online;
		std::string m_ID;
		std::string m_account;
		std::string m_name;
		std::string m_icon;
		std::string m_sign;
		std::vector<std::string> m_dynamicList;

	private:
		std::string m_password;
		std::vector<std::string> m_friendList;
		std::vector<std::string> m_groupList;
	} UserBaseConfig;

	// 基础群聊配置
	typedef struct GroupBaseConfig
	{
		enum class GroupStateType : char
		{
			normal,
			disnotice
		};
		GroupStateType m_state = GroupStateType::normal;
		std::string m_ID;
		std::string m_account;
		std::string m_name;
		std::string m_icon;
		std::string m_describe;
		uint64_t m_memberCount;
		std::vector<std::string> m_memberList;
	} GroupBaseConfig;

	// 基础消息配置
	typedef struct MessageBaseConfig
	{
		enum class MessageType : char
		{
			text = 0x01,
			image = 0x02,
			file = 0x04,
			audio = 0x08,
			video = 0x10,
			link = 0x20,
			emoji = 0x40,
			blend = text | image | link | emoji
		};
		enum class SenderType : char
		{
			me,
			she,
			system,
			time,
		};
		MessageType m_messageType = MessageType::text;
		SenderType m_senderType = SenderType::me;
		std::string m_ID;
		std::string m_senderID;
		std::string m_receiverID;
		std::string m_content;
		std::time_t m_time;
		bool m_isSended;
		std::optional<bool> m_isDownLoaded; // 当消息类型为文件时,属性有效
	} MessageBaseConfig;

	// 基础动态配置
	typedef struct DynamicBaseConfig
	{
		enum class DynamicType : char
		{
			text,
			image,
			video,
			link,
			emoji,
			blend = text | image | link | emoji
		};
		DynamicType m_type = DynamicType::text;
		std::string m_ID;
		std::string m_userID;
		std::string m_content;
		std::time_t m_time;
		std::vector<std::string> m_likeList;
		std::vector<std::string> m_commentList;
	} DynamicBaseConfig;

	// 基础皮肤配置
	typedef struct SkinBaseConfig
	{
		enum class SkinType : char
		{
			theme,
			widget,
		};
		SkinType m_type = SkinType::theme;
		std::string m_ID;
		std::string m_name;
		std::string m_describe;
		std::string m_icon;
		std::string m_path;
	} SkinBaseConfig;
}

#endif