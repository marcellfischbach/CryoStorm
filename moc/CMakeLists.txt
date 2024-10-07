


include_directories(${CryoStorm_SOURCE_DIR}/moc)

add_executable(csMOC
        ast.cc
        ast.hh
        cache.cc
        cache.hh
        main.cc
        generate/generator.cc
        generate/generator.hh
        generate/header.cc
        generate/header.hh
        generate/java.cc
        generate/java.hh
        generate/javaconverter.cc
        generate/javaconverter.hh
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

target_link_libraries(csMOC
        csXml)

install(TARGETS csMOC RUNTIME DESTINATION bin)
