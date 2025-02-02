CC=gcc
CFLAGS=`pkg-config --cflags --libs opencv4`
INCLUDEPATH=../3party-arboretum/include
LIBPATH=-L../3party-arboretum/lib
INCLUDE=-I$(INCLUDEPATH)
LIBS=-lstdc++ -lm -larboretum
SRC= main.cpp app.cpp image.cpp 
OBJS=$(subst .cpp,.o,$(SRC))


# Implicit Rules
%.o: %.cpp $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@ $(INCLUDE)

Cities: $(OBJS)
	$(CC) $(OBJS) -o App $(INCLUDE) $(LIBPATH) $(LIBS) $(CFLAGS) 
