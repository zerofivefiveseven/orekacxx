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

#ifndef __DELETETAPEMSG_H__
#define __DELETETAPEMSG_H__

#include "messages/SyncMessage.h"

class DeleteTapeMsg : public SyncMessage
{
public:
	void Define(Serializer* s) override;
	inline void Validate() override {};

	CStdString GetClassName() override;
	ObjectRef NewInstance() override;
	ObjectRef Process() override;

	CStdString m_filename;
};

#endif

