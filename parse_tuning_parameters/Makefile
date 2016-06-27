opencv=pkg-config opencv --libs --cflags opencv
CC=gcc

PROJECT_NAME = parseCalibrationMap

all: $(PROJECT_NAME)

SOURCE_FILES_C = $(wildcard *.c) $(wildcard 3a_interface/*.c) $(wildcard json/json-builder/*.c) $(wildcard json/json-parser/*.c)
SOURCE_FILES_CPP = $(wildcard 3a_interface/*.cpp)
BUILD_DIR = ./build

OBJECTS_C = $(patsubst %.c,$(BUILD_DIR)/%.o,$(notdir $(SOURCE_FILES_C)))
OBJECTS_CPP = $(patsubst %.cpp,$(BUILD_DIR)/%.o,$(notdir $(SOURCE_FILES_CPP)))

LIBS = -lm

INCLUDES= -I./3a_interface -I./json/json-builder -I./json/json-parser

$(PROJECT_NAME): $(OBJECTS_C) $(OBJECTS_CPP)
	$(CC) -o $(PROJECT_NAME) $(OBJECTS_C) $(OBJECTS_CPP) $(LIBS)

$(OBJECTS_C): $(SOURCE_FILES_C)
	$(CC) -c $(SOURCE_FILES_C) $(INCLUDES)
	mkdir -p build
	mv *.o $(BUILD_DIR)/

$(OBJECTS_CPP) : $(SOURCE_FILES_CPP)
	$(CC) -c $(SOURCE_FILES_CPP) $(INCLUDES)
	mkdir -p build
	mv *.o $(BUILD_DIR)/


.PTHOY:

clean:
	@echo $(SOURCE_FILES)
	@echo $(OBJECTS)
	find $(BUILD_DIR) -name *.o -exec rm -rf {} \;
	rm $(PROJECT_NAME) -rf
