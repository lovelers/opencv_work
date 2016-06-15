opencv=pkg-config opencv --libs --cflags opencv
CC=g++

OPENCV_FLAGS_COMPILE = $(shell pkg-config opencv --cflags opencv)
OPENCV_FLAGS_LINK = $(shell pkg-config opencv --libs --cflags opencv)

PROJECT_NAME = sort

CFLAGS += -fpermissive
CFLAGS += -DOPENCV

all: $(PROJECT_NAME)

SOURCE_FILES = $(wildcard *.cpp) $(wildcard bayer_domain/*.cpp) $(wildcard rgb_domain/*.cpp) $(wildcard tuning/*.cpp)
BUILD_DIR = ./build

OBJECTS = $(patsubst %.cpp,$(BUILD_DIR)/%.o,$(notdir $(SOURCE_FILES)))

INCLUDES= -I./bayder_domain -I./rgb_domain -I./tuning

$(PROJECT_NAME): $(OBJECTS)
	$(CC) -o $(PROJECT_NAME) $(OBJECTS) $(OPENCV_FLAGS_LINK)

$(OBJECTS): $(SOURCE_FILES)
	$(CC) -c $(SOURCE_FILES) $(INCLUDES) $(OPENCV_FLAGS_COMPILE) $(CFLAGS)
	mkdir -p build
	mv *.o $(BUILD_DIR)/



.PTHOY:

clean:
	@echo $(SOURCE_FILES)
	@echo $(OBJECTS)
	find $(BUILD_DIR) -name *.o -exec rm -rf {} \;
