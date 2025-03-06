#----------------------------------------------------------------
# Generated CMake target import file for configuration "Debug".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "PcapPlusPlus::Packet++" for configuration "Debug"
set_property(TARGET PcapPlusPlus::Packet++ APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(PcapPlusPlus::Packet++ PROPERTIES
  IMPORTED_LOCATION_DEBUG "${_IMPORT_PREFIX}/lib/libPacket++.so.24.09"
  IMPORTED_SONAME_DEBUG "libPacket++.so.24.09"
  )

list(APPEND _IMPORT_CHECK_TARGETS PcapPlusPlus::Packet++ )
list(APPEND _IMPORT_CHECK_FILES_FOR_PcapPlusPlus::Packet++ "${_IMPORT_PREFIX}/lib/libPacket++.so.24.09" )

# Import target "PcapPlusPlus::Pcap++" for configuration "Debug"
set_property(TARGET PcapPlusPlus::Pcap++ APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(PcapPlusPlus::Pcap++ PROPERTIES
  IMPORTED_LOCATION_DEBUG "${_IMPORT_PREFIX}/lib/libPcap++.so.24.09"
  IMPORTED_SONAME_DEBUG "libPcap++.so.24.09"
  )

list(APPEND _IMPORT_CHECK_TARGETS PcapPlusPlus::Pcap++ )
list(APPEND _IMPORT_CHECK_FILES_FOR_PcapPlusPlus::Pcap++ "${_IMPORT_PREFIX}/lib/libPcap++.so.24.09" )

# Import target "PcapPlusPlus::Common++" for configuration "Debug"
set_property(TARGET PcapPlusPlus::Common++ APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(PcapPlusPlus::Common++ PROPERTIES
  IMPORTED_LOCATION_DEBUG "${_IMPORT_PREFIX}/lib/libCommon++.so.24.09"
  IMPORTED_SONAME_DEBUG "libCommon++.so.24.09"
  )

list(APPEND _IMPORT_CHECK_TARGETS PcapPlusPlus::Common++ )
list(APPEND _IMPORT_CHECK_FILES_FOR_PcapPlusPlus::Common++ "${_IMPORT_PREFIX}/lib/libCommon++.so.24.09" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
