CXXFLAGS += -g -fpermissive -lpthread

object = webserver.o wrap.o http_handler.o sql_connection.o
server:$(object)
	g++ -o server $(object) -lmysqlclient

webserver.o:http_handler/http_handler.h socket_wrap/wrap.h CGImysql/sql_connection.h
	g++ -c webserver.cpp $(CXXFLAGS)
wrap.o:socket_wrap/wrap.h
	g++ -c socket_wrap/wrap.cpp $(CXXFLAGS)
http_handler.o:http_handler/http_handler.h socket_wrap/wrap.h CGImysql/sql_connection.h
	g++ -c http_handler/http_handler.cpp $(CXXFLAGS)
sql_connection.o:CGImysql/sql_connection.h
	g++ -c CGImysql/sql_connection.cpp $(CXXFLAGS)
#server: webserver.cpp
#	$(CXX) -o server $^ 
.PHONY : clean
clean:
	-rm -r server
	-rm server $(object)
