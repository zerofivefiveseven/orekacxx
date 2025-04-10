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

#ifndef __ORKAUDIOVERSIONMSG_H__
#define __ORKAUDIOVERSIONMSG_H__

#include "messages/SyncMessage.h"
#include "messages/AsyncMessage.h"


class OrkaudioVersionMsg : public SyncMessage
{
public:
	void Define(Serializer* s);
	inline void Validate() {};

	CStdString GetClassName();
	ObjectRef NewInstance();
	ObjectRef Process();
};

class OrkaudioVersionResponseMsg : public SimpleResponseMsg
{
public:
	void Define(Serializer* s);
	inline void Validate() {};

	CStdString GetClassName();
	ObjectRef NewInstance();
	inline ObjectRef Process() {return ObjectRef();};

	CStdString m_version;
};

void RegisterOrkaudioVersion(const char *ver);
#endif

