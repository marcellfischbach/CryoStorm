

add_definitions(-DSPICE_REFL_EXPORT)


add_library(SpiceRefl SHARED
  class.hh
  class.cc
  classregistry.hh
  classregistry.cc
  module.cc
  module.hh
  )


install(TARGETS SpiceRefl
		RUNTIME DESTINATION bin
		ARCHIVE DESTINATION lib
		)

install(DIRECTORY . DESTINATION include/SpiceRefl FILES_MATCHING PATTERN "*.hh")

