


include_directories(${CrimsonEdge_SOURCE_DIR}/moc)

add_executable(ceMOC
	ast.cc
	ast.hh
	cache.cc
	cache.hh
	main.cc
	generate/generator.cc
	generate/generator.hh
	generate/header.cc
	generate/header.hh
	generate/master.cc
	generate/master.hh
	generate/source.cc
	generate/source.hh
	parser/parseexception.hh
	parser/parser.cc
	parser/parser.hh
	parser/sourcefile.cc
	parser/sourcefile.hh
	parser/token.cc
	parser/token.hh
	parser/tokenizer.cc
	parser/tokenizer.hh
	)



install(TARGETS ceMOC RUNTIME DESTINATION bin)
