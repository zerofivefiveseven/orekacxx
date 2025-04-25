/*
 * Oreka -- A media capture and retrieval platform
 *
 * Copyright (C) 2005, orecx LLC
 *
 * http://www.orecx.com
 *
 * This program is free software, distributed under the terms of
 * the GNU General Public License.
 * Please refer to http://www.gnu.org/copyleft/gpl.html
 *
 */

#ifndef __ADDTAGMSG_H__
#define __ADDTAGMSG_H__

#include "messages/SyncMessage.h"
#include "messages/AsyncMessage.h"

class AddTagMsg final : public SyncMessage, public IReportable
{
public:
	AddTagMsg();
	void Define(Serializer* s) override;
	void Validate() override;

	CStdString GetClassName() override;
	ObjectRef NewInstance() override;
	ObjectRef Process() override;

	//IReportable interface
	bool IsRealtime() override;
	MessageRef CreateResponse() override;
	void HandleResponse(MessageRef responseRef) override;
	MessageRef Clone() override;

	CStdString m_party;
	CStdString m_orkuid;
	CStdString m_tagType;
	CStdString m_tagText;
	CStdString m_dtagOffsetMs;
	bool m_success;

};
typedef oreka::shared_ptr<AddTagMsg> AddTagMsgRef;

#endif
