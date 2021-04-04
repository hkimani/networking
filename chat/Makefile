compile:
	gcc source/client.c -o bin/client
	gcc source/server.c -o bin/server
	gcc source/udpclient.c -o bin/udpclient
	gcc source/client_concurrent_udp.c -o bin/client_concurrent_udp 
	gcc source/udpserver.c -o bin/udpserver
	gcc source/server_concurrent.c -o bin/server_concurrent -pthread
	gcc source/server_concurrent_udp.c -o bin/server_concurrent_udp -pthread

clean:
	rm -r ./bin/server
	rm -r ./bin/client
	rm -r ./bin/cleint_concurrent_udp
	rm -r ./bin/udpserver
	rm -r ./bin/udpclient
	rm -r ./bin/server_concurrent
	rm -r ./bin/server_concurrent_udp
