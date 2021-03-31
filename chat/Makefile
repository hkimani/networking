compile:
	gcc source/client.c -o bin/client
	gcc source/server.c -o bin/server
	gcc source/udpclient.c -o bin/udpclient
	gcc source/udpserver.c -o bin/udpserver
	gcc source/server_concurrent.c -o bin/server_concurrent -pthread

clean:
	rm -r ./bin/server
	rm -r ./bin/client
	rm -r ./bin/udpserver
	rm -r ./bin/udpclient
	rm -r ./bin/server_concurrent
