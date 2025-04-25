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

#ifndef __PINGMSG_H__
#define __PINGMSG_H__

#include "messages/SyncMessage.h"
#include "messages/AsyncMessage.h"


class
PingResponseMsg : public AsyncMessage
{
public:
	void Define(Serializer* s) override;
	inline void Validate() override {};

	CStdString GetClassName() override;
	ObjectRef NewInstance() override;
	inline ObjectRef Process() override {return {};};

	bool m_success{};
};

class PingMsg : public SyncMessage
{
public:
	void Define(Serializer* s) override;
	inline void Validate() override {};

	CStdString GetClassName() override;
	ObjectRef NewInstance() override;
	ObjectRef Process() override;
};

class TcpPingMsg : public SyncMessage
{
public:
	TcpPingMsg();
	~TcpPingMsg() override { apr_pool_destroy(m_loc_pool); }


	void Define(Serializer* s) override;
	inline void Validate() override {};

	CStdString GetClassName() override;
	ObjectRef NewInstance() override;
	ObjectRef Process() override;

	CStdString m_hostname;
	int m_port;
	apr_pool_t * m_loc_pool;
	
};

#endif

