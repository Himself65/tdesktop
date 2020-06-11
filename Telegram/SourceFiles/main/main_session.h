/*
This file is part of Telegram Desktop,
the official desktop application for the Telegram messaging service.

For license and copyright information please follow this link:
https://github.com/telegramdesktop/tdesktop/blob/master/LEGAL
*/
#pragma once

#include <rpl/event_stream.h>
#include <rpl/filter.h>
#include <rpl/variable.h>
#include "main/main_settings.h"
#include "base/timer.h"

class ApiWrap;

namespace Api {
class Updates;
} // namespace Api

namespace MTP {
class Instance;
} // namespace MTP

namespace Support {
class Helper;
class Templates;
} // namespace Support

namespace Data {
class Session;
} // namespace Data

namespace Storage {
class DownloadManagerMtproto;
class Uploader;
class Facade;
class Account;
} // namespace Storage

namespace Window {
namespace Notifications {
class System;
} // namespace Notifications
class SessionController;
} // namespace Window

namespace Calls {
class Instance;
} // namespace Calls

namespace Stickers {
class EmojiPack;
class DicePacks;
} // namespace Stickers;

namespace Core {
class Changelogs;
} // namespace Core

namespace Main {

class Account;

class Session final
	: public base::has_weak_ptr
	, private base::Subscriber {
public:
	Session(
		not_null<Main::Account*> account,
		const MTPUser &user,
		Settings &&other);
	~Session();

	Session(const Session &other) = delete;
	Session &operator=(const Session &other) = delete;

	[[nodiscard]] Main::Account &account() const;
	[[nodiscard]] Storage::Account &local() const;

	[[nodiscard]] UserId userId() const;
	[[nodiscard]] PeerId userPeerId() const;
	[[nodiscard]] not_null<UserData*> user() const {
		return _user;
	}
	bool validateSelf(const MTPUser &user);

	[[nodiscard]] Api::Updates &updates() const {
		return *_updates;
	}
	[[nodiscard]] Storage::DownloadManagerMtproto &downloader() const {
		return *_downloader;
	}
	[[nodiscard]] Storage::Uploader &uploader() const {
		return *_uploader;
	}
	[[nodiscard]] Storage::Facade &storage() const {
		return *_storage;
	}
	[[nodiscard]] Stickers::EmojiPack &emojiStickersPack() const {
		return *_emojiStickersPack;
	}
	[[nodiscard]] Stickers::DicePacks &diceStickersPacks() const {
		return *_diceStickersPacks;
	}

	[[nodiscard]] Window::Notifications::System &notifications() {
		return *_notifications;
	}

	[[nodiscard]] Data::Session &data() {
		return *_data;
	}
	[[nodiscard]] Settings &settings() {
		return _settings;
	}
	void saveSettingsDelayed(crl::time delay = kDefaultSaveDelay);
	void saveSettingsNowIfNeeded();

	void addWindow(not_null<Window::SessionController*> controller);
	[[nodiscard]] auto windows() const
		-> const base::flat_set<not_null<Window::SessionController*>> &;

	// Shortcuts.
	[[nodiscard]] base::Observable<void> &downloaderTaskFinished();
	[[nodiscard]] MTP::DcId mainDcId() const;
	[[nodiscard]] not_null<MTP::Instance*> mtp() const;
	[[nodiscard]] ApiWrap &api() {
		return *_api;
	}

	[[nodiscard]] Calls::Instance &calls() {
		return *_calls;
	}

	void termsDeleteNow();

	[[nodiscard]] rpl::lifetime &lifetime() {
		return _lifetime;
	}

	base::Observable<DocumentData*> documentUpdated;
	base::Observable<std::pair<not_null<HistoryItem*>, MsgId>> messageIdChanging;

	bool supportMode() const;
	Support::Helper &supportHelper() const;
	Support::Templates &supportTemplates() const;

private:
	static constexpr auto kDefaultSaveDelay = crl::time(1000);

	const not_null<Main::Account*> _account;

	Settings _settings;
	base::Timer _saveSettingsTimer;

	const std::unique_ptr<ApiWrap> _api;
	const std::unique_ptr<Api::Updates> _updates;
	const std::unique_ptr<Calls::Instance> _calls;
	const std::unique_ptr<Storage::DownloadManagerMtproto> _downloader;
	const std::unique_ptr<Storage::Uploader> _uploader;
	const std::unique_ptr<Storage::Facade> _storage;
	const std::unique_ptr<Window::Notifications::System> _notifications;

	// _data depends on _downloader / _uploader / _notifications.
	const std::unique_ptr<Data::Session> _data;
	const not_null<UserData*> _user;

	// _emojiStickersPack depends on _data.
	const std::unique_ptr<Stickers::EmojiPack> _emojiStickersPack;
	const std::unique_ptr<Stickers::DicePacks> _diceStickersPacks;

	// _changelogs depends on _data, subscribes on chats loading event.
	const std::unique_ptr<Core::Changelogs> _changelogs;

	const std::unique_ptr<Support::Helper> _supportHelper;

	base::flat_set<not_null<Window::SessionController*>> _windows;

	rpl::lifetime _lifetime;

};

} // namespace Main
