//
// Created by revyakin on 10.04.25.
//

#include "RecorderSender.h"
#include <vector>
#include <bitset>

#include "ConfigManager.h"
#include "CommandProcessing.h"
#include "audiofile/LibSndFileFile.h"
#include "Daemon.h"
#include "Filter.h"
#include "Reporting.h"
#include "Utils.h"

#pragma warning( disable: 4786 )

CStdString processorName("RecorderSender");
TapeProcessorRef RecorderSender::m_singleton;

void RecorderSender::Initialize()
{
    if(m_singleton.get() == nullptr)
    {
        m_singleton.reset(new RecorderSender());
        TapeProcessorRegistry::instance()->RegisterTapeProcessor(m_singleton);
    }
}




















