/*
 * Oreka -- A media capture and retrieval platform
 *
 * Copyright (C) 2005, orecx LLC
 *
 * http://www.orecx.com
 *
 */
#ifndef __ORKTRACK_H__
#define __ORKTRACK_H__ 1

#include "OrkBase.h"
#include "StdString.h"
#include <list>
#include <vector>

class OrkTrack {
	public:
		static void Initialize(const std::list<CStdString>& hostnames, const CStdString defaultServiceName, const int defaultTcpPort, const int defaultTlsPort);
		static const std::vector<OrkTrack>& getTrackers() {
			return s_trackers;
		}
		CStdString ToString() {
			CStdString logMsg;
			logMsg.Format ("%s:%u/%s",m_hostname,m_port,m_servicename);
			return logMsg;
		}

		CStdString m_hostname;
		CStdString m_servicename;
		int m_port;
		bool m_https;

	private:
		static std::vector<OrkTrack> s_trackers;
};

#endif
