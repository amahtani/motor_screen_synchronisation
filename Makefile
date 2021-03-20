CXX = g++
RM = rm

CFLAGS = `pkg-config --cflags opencv`
LIBS = `pkg-config --libs opencv`

TARGET = main

OBJS = Comportements.o Modele.o Simulation.o Vitesse.o monteeDescente.o

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) -o $@ $^ $(CFLAGS) $(LIBS)
	
%.o: %.cpp
	$(CXX) -c -Wall $(CFLAGS) $<

clean: 
	$(RM) -f *.o $(TARGET)
