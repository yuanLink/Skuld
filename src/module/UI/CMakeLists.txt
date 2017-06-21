set(LIBUI_SRC 
	${CMAKE_CURRENT_SOURCE_DIR}/UI.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/UIButton.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/UIContext.cpp 
	${CMAKE_CURRENT_SOURCE_DIR}/UIControl.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/UIImage.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/UICairoImage.h
	${CMAKE_CURRENT_SOURCE_DIR}/UICairoFont.h
	${CMAKE_CURRENT_SOURCE_DIR}/UICairoContext.h
	${CMAKE_CURRENT_SOURCE_DIR}/UIInternal.h
	${CMAKE_CURRENT_SOURCE_DIR}/UICairoFont.cpp
	${SKULD_INCLUDE_DIR}/Skuld/UI/UIFont.h 
	${SKULD_INCLUDE_DIR}/Skuld/UI/UIImage.h
	${SKULD_INCLUDE_DIR}/Skuld/UI/Displayable.h
	${SKULD_INCLUDE_DIR}/Skuld/UI/UI.h 
	${SKULD_INCLUDE_DIR}/Skuld/UI/UIButton.h
	${SKULD_INCLUDE_DIR}/Skuld/UI/UIContext.h
	${SKULD_INCLUDE_DIR}/Skuld/UI/UIControl.h
	${SKULD_INCLUDE_DIR}/Skuld/UI/UIImageDraw.h
	${SKULD_INCLUDE_DIR}/Skuld/UI/UIInput.h 
	${SKULD_INCLUDE_DIR}/Skuld/UI/UIScrollBar.h
	${SKULD_INCLUDE_DIR}/Skuld/UI/UITextbox.h)
if (SKULD_WIN32)
	set(LIBUI_SRC ${LIBUI_SRC} ${CMAKE_CURRENT_SOURCE_DIR}/Platform/WindowsDisplayable.cpp)
elseif (SKULD_ANDROID)
	set(LIBUI_SRC ${LIBUI_SRC} ${CMAKE_CURRENT_SOURCE_DIR}/Platform/AndroidDisplayable.cpp)
elseif (SKULD_UNIX)
	set(LIBUI_SRC ${LIBUI_SRC} ${CMAKE_CURRENT_SOURCE_DIR}/Platform/XDisplayable.cpp)
elseif (SKULD_MAC)
	set(LIBUI_SRC ${LIBUI_SRC} ${CMAKE_CURRENT_SOURCE_DIR}/Platform/macOSDisplayable.mm)
endif()
add_module(UI ${LIBUI_SRC})

target_include_directories(UI PRIVATE ${SKULD_SDK_DIR})

add_target_dependencies(UI cairo freetype harfbuzz)