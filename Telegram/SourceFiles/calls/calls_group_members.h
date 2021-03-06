/*
This file is part of Telegram Desktop,
the official desktop application for the Telegram messaging service.

For license and copyright information please follow this link:
https://github.com/telegramdesktop/tdesktop/blob/master/LEGAL
*/
#pragma once

#include "boxes/peer_list_box.h"

namespace Ui {
class ScrollArea;
} // namespace Ui

namespace Data {
class GroupCall;
} // namespace Data

namespace Calls {

class GroupCall;

class GroupMembers final
	: public Ui::RpWidget
	, private PeerListContentDelegate {
public:
	GroupMembers(
		not_null<QWidget*> parent,
		not_null<GroupCall*> call);

	struct MuteRequest {
		not_null<UserData*> user;
		bool mute = false;
	};

	[[nodiscard]] int desiredHeight() const;
	[[nodiscard]] rpl::producer<int> desiredHeightValue() const override;
	[[nodiscard]] rpl::producer<MuteRequest> toggleMuteRequests() const;
	[[nodiscard]] auto kickMemberRequests() const
		-> rpl::producer<not_null<UserData*>>;
	[[nodiscard]] rpl::producer<> addMembersRequests() const {
		return _addMemberRequests.events();
	}

private:
	using ListWidget = PeerListContent;

	void visibleTopBottomUpdated(
		int visibleTop,
		int visibleBottom) override;

	void resizeEvent(QResizeEvent *e) override;

	// PeerListContentDelegate interface.
	void peerListSetTitle(rpl::producer<QString> title) override;
	void peerListSetAdditionalTitle(rpl::producer<QString> title) override;
	bool peerListIsRowChecked(not_null<PeerListRow*> row) override;
	int peerListSelectedRowsCount() override;
	void peerListScrollToTop() override;
	std::vector<not_null<PeerData*>> peerListCollectSelectedRows() override;
	void peerListAddSelectedPeerInBunch(
		not_null<PeerData*> peer) override;
	void peerListAddSelectedRowInBunch(
		not_null<PeerListRow*> row) override;
	void peerListFinishSelectedRowsBunch() override;
	void peerListSetDescription(
		object_ptr<Ui::FlatLabel> description) override;

	void setupHeader(not_null<GroupCall*> call);
	object_ptr<Ui::FlatLabel> setupTitle(not_null<GroupCall*> call);
	void setupList();
	void setupFakeRoundCorners();

	void setupButtons(not_null<GroupCall*> call);

	void updateHeaderControlsGeometry(int newWidth);

	const base::weak_ptr<GroupCall> _call;
	object_ptr<Ui::ScrollArea> _scroll;
	std::unique_ptr<PeerListController> _listController;
	object_ptr<Ui::RpWidget> _header = { nullptr };
	ListWidget *_list = { nullptr };
	rpl::event_stream<> _addMemberRequests;

	Ui::RpWidget *_titleWrap = nullptr;
	Ui::FlatLabel *_title = nullptr;
	Ui::IconButton *_addMember = nullptr;

};

} // namespace Calls
