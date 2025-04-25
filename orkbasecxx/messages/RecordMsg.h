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

#ifndef __RECORDMSG_H__
#define __RECORDMSG_H__

#include "messages/SyncMessage.h"
#include "AudioCapture.h"

class RecordMsg final : public SyncMessage
{
public:
	void Define(Serializer* s) override;
	inline void Validate() override {};

	void EnsureValidSide();
	CStdString GetClassName() override;
	ObjectRef NewInstance() override;
	ObjectRef Process() override;

	CStdString m_party;
	CStdString m_orkuid;
	CStdString m_nativecallid;
	CStdString m_side;
};

class PauseMsg final : public SyncMessage
{
public:
	void Define(Serializer* s) override;
	inline void Validate() override {};

	CStdString GetClassName() override;
	ObjectRef NewInstance() override;
	ObjectRef Process() override;

	CStdString m_party;
	CStdString m_orkuid;
	CStdString m_nativecallid;
};

class StopMsg : public SyncMessage
{
public:
	void Define(Serializer* s) override;
	inline void Validate() override {};

	CStdString GetClassName() override;
	ObjectRef NewInstance() override;
	ObjectRef Process() override;

	CStdString m_party;
	CStdString m_orkuid;
	CStdString m_nativecallid;
};

#endif

