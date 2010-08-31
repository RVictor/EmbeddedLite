OPT= -O0 
DEBUG= -g 
CMP=g++  $(DEBUG) $(OPT) 
GCC=gcc  $(DEBUG) $(OPT) 

all: codelite_cppcheck
codelite_cppcheck_objects = \
	Debug/cppchecker_net_reply.o\
	Debug/cppchecker_protocol.o\
	Debug/cppchecker_request.o\
	Debug/named_pipe.o\
	Debug/named_pipe_client.o\
	Debug/named_pipe_server.o\
	Debug/np_connections_server.o\
	Debug/checkautovariables.o\
	Debug/checkbufferoverrun.o\
	Debug/checkclass.o\
	Debug/checkdangerousfunctions.o\
	Debug/checkexceptionsafety.o\
	Debug/checkheaders.o\
	Debug/checkmemoryleak.o\
	Debug/checkother.o\
	Debug/checkstl.o\
	Debug/checkunusedfunctions.o\
	Debug/cppcheck.o\
	Debug/cppcheckexecutor.o\
	Debug/cppcheckexecutornetwork.o\
	Debug/errorlogger.o\
	Debug/filelister.o\
	Debug/main.o\
	Debug/mathlib.o\
	Debug/preprocessor.o\
	Debug/settings.o\
	Debug/threadexecutor.o\
	Debug/token.o\
	Debug/tokenize.o\
	Debug/ethread_unix.o\
	Debug/ethread_win.o\
	Debug/utils.o\

Debug/cppchecker_net_reply.o: network/cppchecker_net_reply.cpp Debug/cppchecker_net_reply.o.d
	$(CMP)  -c network/cppchecker_net_reply.cpp -I. -o Debug/cppchecker_net_reply.o
Debug/cppchecker_net_reply.o.d:
	$(CMP) -I. -MTDebug/cppchecker_net_reply.o -MFDebug/cppchecker_net_reply.o.d  -MM network/cppchecker_net_reply.cpp
Debug/cppchecker_protocol.o: network/cppchecker_protocol.cpp Debug/cppchecker_protocol.o.d
	$(CMP)  -c network/cppchecker_protocol.cpp -I. -o Debug/cppchecker_protocol.o
Debug/cppchecker_protocol.o.d:
	$(CMP) -I. -MTDebug/cppchecker_protocol.o -MFDebug/cppchecker_protocol.o.d  -MM network/cppchecker_protocol.cpp
Debug/cppchecker_request.o: network/cppchecker_request.cpp Debug/cppchecker_request.o.d
	$(CMP)  -c network/cppchecker_request.cpp -I. -o Debug/cppchecker_request.o
Debug/cppchecker_request.o.d:
	$(CMP) -I. -MTDebug/cppchecker_request.o -MFDebug/cppchecker_request.o.d  -MM network/cppchecker_request.cpp
Debug/named_pipe.o: network/named_pipe.cpp Debug/named_pipe.o.d
	$(CMP)  -c network/named_pipe.cpp -I. -o Debug/named_pipe.o
Debug/named_pipe.o.d:
	$(CMP) -I. -MTDebug/named_pipe.o -MFDebug/named_pipe.o.d  -MM network/named_pipe.cpp
Debug/named_pipe_client.o: network/named_pipe_client.cpp Debug/named_pipe_client.o.d
	$(CMP)  -c network/named_pipe_client.cpp -I. -o Debug/named_pipe_client.o
Debug/named_pipe_client.o.d:
	$(CMP) -I. -MTDebug/named_pipe_client.o -MFDebug/named_pipe_client.o.d  -MM network/named_pipe_client.cpp
Debug/named_pipe_server.o: network/named_pipe_server.cpp Debug/named_pipe_server.o.d
	$(CMP)  -c network/named_pipe_server.cpp -I. -o Debug/named_pipe_server.o
Debug/named_pipe_server.o.d:
	$(CMP) -I. -MTDebug/named_pipe_server.o -MFDebug/named_pipe_server.o.d  -MM network/named_pipe_server.cpp
Debug/np_connections_server.o: network/np_connections_server.cpp Debug/np_connections_server.o.d
	$(CMP)  -c network/np_connections_server.cpp -I. -o Debug/np_connections_server.o
Debug/np_connections_server.o.d:
	$(CMP) -I. -MTDebug/np_connections_server.o -MFDebug/np_connections_server.o.d  -MM network/np_connections_server.cpp
Debug/checkautovariables.o: src/checkautovariables.cpp Debug/checkautovariables.o.d
	$(CMP)  -c src/checkautovariables.cpp -I. -o Debug/checkautovariables.o
Debug/checkautovariables.o.d:
	$(CMP) -I. -MTDebug/checkautovariables.o -MFDebug/checkautovariables.o.d  -MM src/checkautovariables.cpp
Debug/checkbufferoverrun.o: src/checkbufferoverrun.cpp Debug/checkbufferoverrun.o.d
	$(CMP)  -c src/checkbufferoverrun.cpp -I. -o Debug/checkbufferoverrun.o
Debug/checkbufferoverrun.o.d:
	$(CMP) -I. -MTDebug/checkbufferoverrun.o -MFDebug/checkbufferoverrun.o.d  -MM src/checkbufferoverrun.cpp
Debug/checkclass.o: src/checkclass.cpp Debug/checkclass.o.d
	$(CMP)  -c src/checkclass.cpp -I. -o Debug/checkclass.o
Debug/checkclass.o.d:
	$(CMP) -I. -MTDebug/checkclass.o -MFDebug/checkclass.o.d  -MM src/checkclass.cpp
Debug/checkdangerousfunctions.o: src/checkdangerousfunctions.cpp Debug/checkdangerousfunctions.o.d
	$(CMP)  -c src/checkdangerousfunctions.cpp -I. -o Debug/checkdangerousfunctions.o
Debug/checkdangerousfunctions.o.d:
	$(CMP) -I. -MTDebug/checkdangerousfunctions.o -MFDebug/checkdangerousfunctions.o.d  -MM src/checkdangerousfunctions.cpp
Debug/checkexceptionsafety.o: src/checkexceptionsafety.cpp Debug/checkexceptionsafety.o.d
	$(CMP)  -c src/checkexceptionsafety.cpp -I. -o Debug/checkexceptionsafety.o
Debug/checkexceptionsafety.o.d:
	$(CMP) -I. -MTDebug/checkexceptionsafety.o -MFDebug/checkexceptionsafety.o.d  -MM src/checkexceptionsafety.cpp
Debug/checkheaders.o: src/checkheaders.cpp Debug/checkheaders.o.d
	$(CMP)  -c src/checkheaders.cpp -I. -o Debug/checkheaders.o
Debug/checkheaders.o.d:
	$(CMP) -I. -MTDebug/checkheaders.o -MFDebug/checkheaders.o.d  -MM src/checkheaders.cpp
Debug/checkmemoryleak.o: src/checkmemoryleak.cpp Debug/checkmemoryleak.o.d
	$(CMP)  -c src/checkmemoryleak.cpp -I. -o Debug/checkmemoryleak.o
Debug/checkmemoryleak.o.d:
	$(CMP) -I. -MTDebug/checkmemoryleak.o -MFDebug/checkmemoryleak.o.d  -MM src/checkmemoryleak.cpp
Debug/checkother.o: src/checkother.cpp Debug/checkother.o.d
	$(CMP)  -c src/checkother.cpp -I. -o Debug/checkother.o
Debug/checkother.o.d:
	$(CMP) -I. -MTDebug/checkother.o -MFDebug/checkother.o.d  -MM src/checkother.cpp
Debug/checkstl.o: src/checkstl.cpp Debug/checkstl.o.d
	$(CMP)  -c src/checkstl.cpp -I. -o Debug/checkstl.o
Debug/checkstl.o.d:
	$(CMP) -I. -MTDebug/checkstl.o -MFDebug/checkstl.o.d  -MM src/checkstl.cpp
Debug/checkunusedfunctions.o: src/checkunusedfunctions.cpp Debug/checkunusedfunctions.o.d
	$(CMP)  -c src/checkunusedfunctions.cpp -I. -o Debug/checkunusedfunctions.o
Debug/checkunusedfunctions.o.d:
	$(CMP) -I. -MTDebug/checkunusedfunctions.o -MFDebug/checkunusedfunctions.o.d  -MM src/checkunusedfunctions.cpp
Debug/cppcheck.o: src/cppcheck.cpp Debug/cppcheck.o.d
	$(CMP)  -c src/cppcheck.cpp -I. -o Debug/cppcheck.o
Debug/cppcheck.o.d:
	$(CMP) -I. -MTDebug/cppcheck.o -MFDebug/cppcheck.o.d  -MM src/cppcheck.cpp
Debug/cppcheckexecutor.o: src/cppcheckexecutor.cpp Debug/cppcheckexecutor.o.d
	$(CMP)  -c src/cppcheckexecutor.cpp -I. -o Debug/cppcheckexecutor.o
Debug/cppcheckexecutor.o.d:
	$(CMP) -I. -MTDebug/cppcheckexecutor.o -MFDebug/cppcheckexecutor.o.d  -MM src/cppcheckexecutor.cpp
Debug/cppcheckexecutornetwork.o: src/cppcheckexecutornetwork.cpp Debug/cppcheckexecutornetwork.o.d
	$(CMP)  -c src/cppcheckexecutornetwork.cpp -I. -o Debug/cppcheckexecutornetwork.o
Debug/cppcheckexecutornetwork.o.d:
	$(CMP) -I. -MTDebug/cppcheckexecutornetwork.o -MFDebug/cppcheckexecutornetwork.o.d  -MM src/cppcheckexecutornetwork.cpp
Debug/errorlogger.o: src/errorlogger.cpp Debug/errorlogger.o.d
	$(CMP)  -c src/errorlogger.cpp -I. -o Debug/errorlogger.o
Debug/errorlogger.o.d:
	$(CMP) -I. -MTDebug/errorlogger.o -MFDebug/errorlogger.o.d  -MM src/errorlogger.cpp
Debug/filelister.o: src/filelister.cpp Debug/filelister.o.d
	$(CMP)  -c src/filelister.cpp -I. -o Debug/filelister.o
Debug/filelister.o.d:
	$(CMP) -I. -MTDebug/filelister.o -MFDebug/filelister.o.d  -MM src/filelister.cpp
Debug/main.o: src/main.cpp Debug/main.o.d
	$(CMP)  -c src/main.cpp -I. -o Debug/main.o
Debug/main.o.d:
	$(CMP) -I. -MTDebug/main.o -MFDebug/main.o.d  -MM src/main.cpp
Debug/mathlib.o: src/mathlib.cpp Debug/mathlib.o.d
	$(CMP)  -c src/mathlib.cpp -I. -o Debug/mathlib.o
Debug/mathlib.o.d:
	$(CMP) -I. -MTDebug/mathlib.o -MFDebug/mathlib.o.d  -MM src/mathlib.cpp
Debug/preprocessor.o: src/preprocessor.cpp Debug/preprocessor.o.d
	$(CMP)  -c src/preprocessor.cpp -I. -o Debug/preprocessor.o
Debug/preprocessor.o.d:
	$(CMP) -I. -MTDebug/preprocessor.o -MFDebug/preprocessor.o.d  -MM src/preprocessor.cpp
Debug/settings.o: src/settings.cpp Debug/settings.o.d
	$(CMP)  -c src/settings.cpp -I. -o Debug/settings.o
Debug/settings.o.d:
	$(CMP) -I. -MTDebug/settings.o -MFDebug/settings.o.d  -MM src/settings.cpp
Debug/threadexecutor.o: src/threadexecutor.cpp Debug/threadexecutor.o.d
	$(CMP)  -c src/threadexecutor.cpp -I. -o Debug/threadexecutor.o
Debug/threadexecutor.o.d:
	$(CMP) -I. -MTDebug/threadexecutor.o -MFDebug/threadexecutor.o.d  -MM src/threadexecutor.cpp
Debug/token.o: src/token.cpp Debug/token.o.d
	$(CMP)  -c src/token.cpp -I. -o Debug/token.o
Debug/token.o.d:
	$(CMP) -I. -MTDebug/token.o -MFDebug/token.o.d  -MM src/token.cpp
Debug/tokenize.o: src/tokenize.cpp Debug/tokenize.o.d
	$(CMP)  -c src/tokenize.cpp -I. -o Debug/tokenize.o
Debug/tokenize.o.d:
	$(CMP) -I. -MTDebug/tokenize.o -MFDebug/tokenize.o.d  -MM src/tokenize.cpp
Debug/ethread_unix.o: utils/ethread_unix.cpp Debug/ethread_unix.o.d
	$(CMP)  -c utils/ethread_unix.cpp -I. -o Debug/ethread_unix.o
Debug/ethread_unix.o.d:
	$(CMP) -I. -MTDebug/ethread_unix.o -MFDebug/ethread_unix.o.d  -MM utils/ethread_unix.cpp
Debug/ethread_win.o: utils/ethread_win.cpp Debug/ethread_win.o.d
	$(CMP)  -c utils/ethread_win.cpp -I. -o Debug/ethread_win.o
Debug/ethread_win.o.d:
	$(CMP) -I. -MTDebug/ethread_win.o -MFDebug/ethread_win.o.d  -MM utils/ethread_win.cpp
Debug/utils.o: utils/utils.cpp Debug/utils.o.d
	$(CMP)  -c utils/utils.cpp -I. -o Debug/utils.o
Debug/utils.o.d:
	$(CMP) -I. -MTDebug/utils.o -MFDebug/utils.o.d  -MM utils/utils.cpp
-include Debug/*.d

codelite_cppcheck: make_dirs $(codelite_cppcheck_objects)
	$(CMP) -o codelite_cppcheck $(codelite_cppcheck_objects) -lpthread -ldl 

make_dirs:
	@echo
	@echo "----------Building project:[ codelite_cppcheck - Debug ]----------"
	@echo
	test -d Debug || mkdir Debug

clean: 
	$(RM) -fr Debug/*.o
	$(RM) -fr Debug/*.o.d
	$(RM) -fr codelite_cppcheck
