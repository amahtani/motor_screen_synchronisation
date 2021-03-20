CXX = g++
RM = rm

CFLAGS = `pkg-config --cflags opencv`
LIBS = `pkg-config --libs opencv`

TARGET = main

OBJS = main.o image.o codeSimuEcransAgites.o

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) -o $@ $^ $(CFLAGS) $(LIBS)
	
%.o: %.cpp
	$(CXX) -c $(CFLAGS) $<

clean: 
	$(RM) -f *.o $(TARGET)
