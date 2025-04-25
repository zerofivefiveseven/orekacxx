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

#ifndef __READLOGGINGPROPERITESMSG_H__
#define __READLOGGINGPROPERITESMSG_H__

#include "messages/SyncMessage.h"
#include "messages/AsyncMessage.h"


class ReadLoggingPropertiesMsg final : public SyncMessage
{
public:
	void Define(Serializer* s) override;
	inline void Validate() override {};

	CStdString GetClassName() override;
	ObjectRef NewInstance() override;
	ObjectRef Process() override;
};

class ListLoggingPropertiesMsg final : public SyncMessage
{
public:
	void Define(Serializer* s) override;
	inline void Validate() override {};

	CStdString GetClassName() override;
	ObjectRef NewInstance() override;
	ObjectRef Process() override;
};

class ListLoggingPropertiesResponseMsg final : public SimpleResponseMsg
{
public:
	void Define(Serializer* s) override;
	inline void Validate() override {};

	CStdString GetClassName() override;
	ObjectRef NewInstance() override;
	inline ObjectRef Process() override {return ObjectRef();};

	CStdString m_loggerInfo;
	int	m_count;
};


#endif

