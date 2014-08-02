BIN=bin
SRC=src
INCLUDE=include
SOURCES=$(notdir $(foreach dir,$(SRC),$(wildcard $(dir)/*.cpp)))
TARGET=$(addprefix $(BIN)/,gostunnel.exe)
OBJECTS=$(addprefix $(BIN)/,$(SOURCES:.cpp=.o))
OBJECTS+=$(addprefix $(BIN)/,gostunnel_res.o)

GCC_DIR = /c/mingw-w64/mingw64/bin
GCC = $(GCC_DIR)/g++

CXXFLAGS = -std=c++11 -O0
INCLUDES = -I/c/boost/include -I/c/openssl/include
DEFINES = -DSO_UPDATE_CONNECT_CONTEXT=0x7010 -DBOOST_SYSTEM_NO_DEPRECATED -D_WIN32_WINNT=0x0501

INC_LIBS = -L/c/openssl/lib -L/c/boost/lib
WIN_LIBS = -lws2_32 -lgdi32 -lmswsock -lcrypt32 -lcryptui -lsecurity
BOOST_LIBS = -lboost_log_setup -lboost_log -lboost_thread -lboost_filesystem -lboost_system -lboost_regex -lboost_program_options
SSL_LIBS = -lssl -lcrypto
LIBS = $(INC_LIBS) $(BOOST_LIBS) $(SSL_LIBS) $(WIN_LIBS)
LDFLAGS = -static

vpath %.cpp $(SRC)

all: $(TARGET)

$(BIN)/gostunnel_res.o:
	$(GCC_DIR)/windres -i gostunnel.rc -o $(BIN)/gostunnel_res.o

$(TARGET): $(OBJECTS)
	$(GCC) -o $@ $(LDFLAGS) $(OBJECTS) $(LIBS)

$(BIN)/%.o: %.cpp
	$(GCC) -o $@ -c $(CXXFLAGS) $(INCLUDES) $(DEFINES) $<

clean:
	-rm -f $(OBJECTS) $(TARGET)
	-rm -f $(BIN)/logs/*.log

.PHONY: all clean