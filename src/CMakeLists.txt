find_package(BISON)
find_package(FLEX)

find_package(PkgConfig)

pkg_check_modules(GTKMM gtkmm-2.4) # look into FindPkgConfig.cmake, 
pkg_check_modules(GTKGLMM gtkglextmm-1.2) # look into FindPkgConfig.cmake, 

link_directories(
	${GTKMM_LIBRARY_DIRS}
	${GTKGLMM_LIBRARY_DIRS}
)

SET(objects
	objects/finish/finish.cpp
	objects/floor/floor.cpp
	objects/floor/movingfloor.cpp
	objects/simcam/simcam.cpp
	objects/player_ball/player_ball.cpp
	objects/wall/wall.cpp
	register/register.cpp
)


add_custom_command (
    DEPENDS ${objects}
    OUTPUT  objects/objects.hpp
    COMMAND cd objects && ./make_objects_hpp && cd ../
)


	execute_process(COMMAND cd objects && ./make_objects_hpp && cd ..)   
   
	BISON_TARGET(object_parser object-parser/obj.y
		${CMAKE_CURRENT_BINARY_DIR}/PARSE_OBJ.cpp COMPILE_FLAGS "-p OBJ_PARSE" )
	FLEX_TARGET(object_lexer object-parser/obj.l 
		${CMAKE_CURRENT_BINARY_DIR}/LEX_OBJ.cpp COMPILE_FLAGS "-POBJ_PARSE")
	ADD_FLEX_BISON_DEPENDENCY(object_lexer object_parser )

	BISON_TARGET(mtl_parser mtl-parser/mtl.y
		${CMAKE_CURRENT_BINARY_DIR}/PARSE_MTL.cpp COMPILE_FLAGS "-p mtl_")
	FLEX_TARGET(mtl_lexer mtl-parser/mtl.l 
		${CMAKE_CURRENT_BINARY_DIR}/LEX_MTL.cpp COMPILE_FLAGS "-Pmtl_")
	ADD_FLEX_BISON_DEPENDENCY( mtl_lexer mtl_parser)

	BISON_TARGET(level_parser level-parser/level.y
		${CMAKE_CURRENT_BINARY_DIR}/PARSE_LEVEL.cpp COMPILE_FLAGS "-p level_")
	FLEX_TARGET(level_lexer level-parser/level.l 
		${CMAKE_CURRENT_BINARY_DIR}/LEX_LEVEL.cpp COMPILE_FLAGS "-Plevel_")
	ADD_FLEX_BISON_DEPENDENCY( level_lexer level_parser)



	include_directories(.)
	include_directories(${GTKGLMM_INCLUDE_DIRS})
	include_directories(${GTKMM_INCLUDE_DIRS})
	include_directories(${CMAKE_CURRENT_BINARY_DIR})
	include_directories(/usr/lib/x86_64-linux-gnu/glib-2.0/include/)
	add_library(engine
		${BISON_object_parser_OUTPUTS}
		${BISON_mtl_parser_OUTPUTS}
		${FLEX_object_lexer_OUTPUTS}
		${FLEX_mtl_lexer_OUTPUTS}
		${BISON_level_parser_OUTPUTS}
		${FLEX_level_lexer_OUTPUTS}
		object-parser/parse.cpp
		mtl-parser/parse.cpp
		level-parser/parse.cpp

		engine/collision_test.cpp
		engine/collision_types.cpp
		engine/collision_data.cpp
		engine/engine_interface.cpp
		engine/engine.cpp
		engine/figure.cpp
		engine/flomath.cpp
	)

ADD_EXECUTABLE(ldes
	main_ldes/main.cpp 
	main_sbb/main_menu.cpp 
	main_ldes/main_window.cpp
	main_sbb/main_window.cpp
	${objects}
)
target_link_libraries(ldes
	${GTKMM_LIBRARIES}
	${GTKGLMM_LIBRARIES}
	boost_filesystem
	boost_system
	glibmm
	engine
)

ADD_EXECUTABLE(sbb 
	main_sbb/main.cpp 
	main_sbb/main_menu.cpp 
	main_sbb/main_window.cpp
	${objects}
)
target_link_libraries(sbb
	${GTKMM_LIBRARIES}
	${GTKGLMM_LIBRARIES}
	boost_filesystem
	boost_system
	engine
)
