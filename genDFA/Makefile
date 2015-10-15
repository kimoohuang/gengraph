P_NAME          :=  nfatodfa
P_CXX_SRCS      :=  $(wildcard *.cpp)
P_CXX_OBJS      :=  ${P_CXX_SRCS:.cpp=.o}
P_INCLUDE_DIRS  :=  
P_LIBRARY_DIRS  :=  
P_LIBRARIES     :=  
CPPFLAGS        +=  $(foreach includedir,$(P_INCLUDE_DIRS),-I$(includedir))
LDFLAGS         +=  $(foreach librarydir,$(P_LIBRARY_DIRS),-L$(librarydir))
LDFLAGS         +=  $(foreach library,$(P_LIBRARIES),-l$(library))
CC              :=  g++ 
CCFLAGS         :=  -Wall -g 
CPPFLAGS        += -g

.PHONY:         all clean
all:            $(P_NAME)
$(P_NAME):      $(P_CXX_OBJS)
		$(CC) $(CCFLAGS) $(P_CXX_OBJS) -o $(P_NAME) $(LDFLAGS)
clean:
		@- $(RM) $(P_NAME)
		@- $(RM) $(P_CXX_OBJS)
		@- $(RM) core*
		@- $(RM) tags

