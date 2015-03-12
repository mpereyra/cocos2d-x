TARGET = libcocos2d.so

INCLUDES += \
			-I../platform/third_party/linux/libfreetype2 \
			-I../platform/third_party/common/etc \
			-I../../extensions \
			-I../../extensions/CCBReader \
			-I../../extensions/GUI/CCControlExtension \
			-I../../extensions/GUI/CCControlExtension \
			-I../../external/chipmunk/include/chipmunk \
			-I../../extensions/network \
			-I../platform/third_party/emscripten/libxml2 \
            -I../../extensions/Components

DEFINES += -D__CC_PLATFORM_FILEUTILS_CPP__
DEFINES += -DCC_ENABLE_CHIPMUNK_INTEGRATION

SOURCES = ../CCCamera.cpp \
../CCConfiguration.cpp \
../CCDirector.cpp \
../CCDrawingPrimitives.cpp \
../CCScheduler.cpp \
../actions/CCAction.cpp \
../actions/CCActionCamera.cpp \
../actions/CCActionCatmullRom.cpp \
../actions/CCActionEase.cpp \
../actions/CCActionGrid.cpp \
../actions/CCActionGrid3D.cpp \
../actions/CCActionInstant.cpp \
../actions/CCActionInterval.cpp \
../actions/CCActionManager.cpp \
../actions/CCActionPageTurn3D.cpp \
../actions/CCActionProgressTimer.cpp \
../actions/CCActionTiledGrid.cpp \
../actions/CCActionTween.cpp \
../base_nodes/CCAtlasNode.cpp \
../base_nodes/CCGLBufferedNode.cpp \
../base_nodes/CCNode.cpp \
../cocoa/CCAffineTransform.cpp \
../cocoa/CCArray.cpp \
../cocoa/CCAutoreleasePool.cpp \
../cocoa/CCDictionary.cpp \
../cocoa/CCGeometry.cpp \
../cocoa/CCNS.cpp \
../cocoa/CCObject.cpp \
../cocoa/CCSet.cpp \
../cocoa/CCString.cpp \
../cocoa/CCZone.cpp \
../cocos2d.cpp \
../effects/CCGrabber.cpp \
../effects/CCGrid.cpp \
../keypad_dispatcher/CCKeypadDelegate.cpp \
../keypad_dispatcher/CCKeypadDispatcher.cpp \
../label_nodes/CCLabelAtlas.cpp \
../label_nodes/CCLabelBMFont.cpp \
../label_nodes/CCLabelTTF.cpp \
../layers_scenes_transitions_nodes/CCLayer.cpp \
../layers_scenes_transitions_nodes/CCScene.cpp \
../layers_scenes_transitions_nodes/CCTransition.cpp \
../layers_scenes_transitions_nodes/CCTransitionPageTurn.cpp \
../layers_scenes_transitions_nodes/CCTransitionProgress.cpp \
../menu_nodes/CCMenu.cpp \
../menu_nodes/CCMenuItem.cpp \
../misc_nodes/CCMotionStreak.cpp \
../misc_nodes/CCProgressTimer.cpp \
../particle_nodes/CCParticleBatchNode.cpp \
../particle_nodes/CCParticleExamples.cpp \
../particle_nodes/CCParticleSystem.cpp \
../particle_nodes/CCParticleSystemQuad.cpp \
../platform/CCEGLViewProtocol.cpp \
../platform/CCThread.cpp \
../platform/platform.cpp \
../script_support/CCScriptSupport.cpp \
../shaders/CCGLProgram.cpp \
../shaders/CCShaderCache.cpp \
../shaders/ccGLStateCache.cpp \
../shaders/ccShaders.cpp \
../sprite_nodes/CCAnimation.cpp \
../sprite_nodes/CCAnimationCache.cpp \
../sprite_nodes/CCSprite.cpp \
../sprite_nodes/CCSpriteBatchNode.cpp \
../sprite_nodes/CCSpriteFrame.cpp \
../sprite_nodes/CCSpriteFrameCache.cpp \
../support/CCNotificationCenter.cpp \
../support/CCPointExtension.cpp \
../support/CCProfiling.cpp \
../support/CCVertex.cpp \
../support/TransformUtils.cpp \
../support/base64.cpp \
../support/ccUtils.cpp \
../support/data_support/ccCArray.cpp \
../support/image_support/TGAlib.cpp \
../text_input_node/CCIMEDispatcher.cpp \
../text_input_node/CCTextFieldTTF.cpp \
../textures/CCDDS.cpp \
../textures/CCTexture2D.cpp \
../textures/CCTextureASTC.cpp \
../textures/CCTextureATC.cpp \
../textures/CCTextureAtlas.cpp \
../textures/CCTextureCache.cpp \
../textures/CCTextureDXT.cpp \
../textures/CCTexturePVR.cpp \
../tilemap_parallax_nodes/CCParallaxNode.cpp \
../tilemap_parallax_nodes/CCTMXLayer.cpp \
../tilemap_parallax_nodes/CCTMXObjectGroup.cpp \
../tilemap_parallax_nodes/CCTMXTiledMap.cpp \
../tilemap_parallax_nodes/CCTMXXMLParser.cpp \
../tilemap_parallax_nodes/CCTileMapAtlas.cpp \
../touch_dispatcher/CCTouch.cpp \
../touch_dispatcher/CCTouchDispatcher.cpp \
../touch_dispatcher/CCTouchHandler.cpp \
../platform/emscripten/CCApplication.cpp \
../platform/emscripten/CCCommon.cpp \
../platform/emscripten/CCEGLView.cpp \
../platform/emscripten/CCFileUtilsEmscripten.cpp \
../platform/emscripten/CCImage.cpp \
../kazmath/src/aabb.c \
../kazmath/src/GL/mat4stack.c \
../kazmath/src/GL/matrix.c \
../kazmath/src/mat3.c \
../kazmath/src/mat4.c \
../kazmath/src/neon_matrix_impl.c \
../kazmath/src/plane.c \
../kazmath/src/quaternion.c \
../kazmath/src/ray2.c \
../kazmath/src/utility.c \
../kazmath/src/vec2.c \
../kazmath/src/vec3.c \
../kazmath/src/vec4.c \
../support/CCUserDefault.cpp \
../misc_nodes/CCRenderTexture.cpp \
../support/zip_support/unzip.cpp \
../support/zip_support/ZipUtils.cpp \
../support/zip_support/ioapi.cpp \
../platform/CCSAXParser.cpp \

ROB_IGNORED= \
../platform/emscripten/CCDevice.cpp \



ROB_NEED_TO_FIX=../platform/emscripten/CCTextureCacheEmscripten.cpp \


COCOS_ROOT = ../..

include cocos2dx.mk

CXXFLAGS += -Wno-sequence-point
CCFLAGS += -Wno-sequence-point

TARGET := $(LIB_DIR)/$(TARGET)

all: $(TARGET)

$(TARGET): $(OBJECTS) $(CORE_MAKEFILE_LIST)
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) $(OBJECTS) -shared -o $@ $(SHAREDLIBS) $(STATICLIBS) $(LIBS)

$(OBJ_DIR)/%.o: ../%.cpp $(CORE_MAKEFILE_LIST)
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) $(INCLUDES) $(DEFINES) -c $< -o $@

$(OBJ_DIR)/%.o: ../%.c $(CORE_MAKEFILE_LIST)
	@mkdir -p $(@D)
	$(CC) $(CCFLAGS) $(INCLUDES) $(DEFINES) -c $< -o $@
