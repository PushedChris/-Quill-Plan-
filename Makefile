CXXFLAGS += -g -fpermissive -lpthread
object = webserver.o wrap.o http_handler.o
server:$(object)
	g++ -o server $(object) 

webserver.o:http_handler/http_handler.h socket_wrap/wrap.h
	g++ -c webserver.cpp $(CXXFLAGS)
wrap.o:socket_wrap/wrap.h
	g++ -c socket_wrap/wrap.cpp $(CXXFLAGS)
http_handler.o:http_handler/http_handler.h socket_wrap/wrap.h
	g++ -c http_handler/http_handler.cpp $(CXXFLAGS)

#server: webserver.cpp
#	$(CXX) -o server $^ 
.PHONY : clean
clean:
	-rm -r server
	-rm server $(object)
